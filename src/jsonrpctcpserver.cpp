#include "jsonrpctcpserver.hpp"

#include <utility>
#include <memory>
#include <json/json.h>
#include <iostream>
#include <asio/streambuf.hpp>
#include <asio/write.hpp>
#include <asio/read_until.hpp>
#include <asio/read.hpp>
#include <asio/buffer.hpp>
#include <thread>

const int max_length = 1024;

JsonRpcTcpServer::JsonRpcTcpServer(unsigned short tcpPort)
    : _ioc()
    , _tcpPort(tcpPort)
{
}

JsonRpcTcpServer::~JsonRpcTcpServer()
{
}

void JsonRpcTcpServer::startListening()
{
    std::thread([](JsonRpcTcpServer * thus){thus->listen();}, this).detach();
}

void JsonRpcTcpServer::stopListening()
{
    {
        const std::lock_guard<std::mutex> lock(_methodsMutex);
        _methods.clear();
    }
    {
        const std::lock_guard<std::mutex> lock(_notificationsMutex);
        _notifications.clear();
    }
}

void JsonRpcTcpServer::bindMethod(const std::string & methodName, const Method & method)
{
    const std::lock_guard<std::mutex> lock(_methodsMutex);
    _methods.insert(std::make_pair(methodName, method));
}

void JsonRpcTcpServer::bindNotification(const std::string & notificationName, const Notification & notification)
{
    const std::lock_guard<std::mutex> lock(_notificationsMutex);
    _notifications.insert(std::make_pair(notificationName, notification));
}

void JsonRpcTcpServer::listen()
{
    std::cout << "Listen on TCP port " << _tcpPort << std::endl;
    asio::ip::tcp::acceptor acceptor(_ioc, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), _tcpPort));
    while (true)
    {
        asio::ip::tcp::socket socket(_ioc);
        acceptor.accept(socket);
        std::thread([](JsonRpcTcpServer * thus, asio::ip::tcp::socket s){thus->session(std::move(s));}, this, std::move(socket)).detach();
    }
}

void JsonRpcTcpServer::session(asio::ip::tcp::socket socket)
{
    asio::streambuf tcpStreambuf;
    std::istream tcpInStream(&tcpStreambuf);
    Json::StreamWriterBuilder jsonStreamWriterBuilder;
    jsonStreamWriterBuilder["indentation"] = "";
    std::unique_ptr<Json::StreamWriter> jsonStreamWriter(jsonStreamWriterBuilder.newStreamWriter());

    while (socket.is_open())
    {
        // Wait receive complete message
#ifdef JSONRPC_MSG_DEBUG
        std::cout << "wait message..." << std::endl;
#endif
        try {
            asio::read_until(socket, tcpStreambuf, 0x0A);
        }
        catch (asio::system_error & e)
        {
            if (e.code().value() == asio::error::eof)
                break;
            throw e;
        }

        // Extract one json message
        std::string messageJsonStr;
        std::getline(tcpInStream, messageJsonStr, static_cast<char>(0x0A));

        // Parse this json message
        Json::CharReaderBuilder builder;
        JSONCPP_STRING errs;
        Json::Value messageJson;
        const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
        if (!reader->parse(messageJsonStr.c_str(), messageJsonStr.c_str()+messageJsonStr.size(),
                &messageJson, &errs))
            throw std::runtime_error(errs);

#ifdef JSONRPC_MSG_DEBUG
        // Print receive message
        std::cout << "Receive ";
        jsonStreamWriter->write(messageJson, &std::cout);
        std::cout << std::endl;
#endif

        std::string methodName = messageJson["method"].asString();
        Json::Value params = messageJson["params"];

        // If notification
        if (!messageJson.isMember("id"))
        {
            // Find it and execute it
            const std::lock_guard<std::mutex> lock(_notificationsMutex);
            Notification * notification = nullptr;
            try {
                notification = &_notifications.at(methodName);
            }
            catch (std::out_of_range & e)
            {
                if (!_methods.empty() || !_notifications.empty())
                    std::cerr << "method \"" << methodName << "\" not supported!" << std::endl;
                break;
            }
            (*notification)(params);
            notification = nullptr;
        }
        // If method
        else
        {
            // Find it and execute it
            Json::Value responseParams;
            {
                const std::lock_guard<std::mutex> lock(_methodsMutex);
                Method * method = nullptr;
                try {
                    method = &_methods.at(methodName);
                }
                catch (std::out_of_range & e)
                {
                    if (!_methods.empty() || !_notifications.empty())
                        std::cerr << "method \"" << methodName << "\" not supported!" << std::endl;
                    break;
                }
                responseParams = (*method)(params);
                method = nullptr;
            }

            // Prepare reponse
            Json::Value response;
            response["jsonrpc"] = "2.0";
            response["result"] = responseParams;
            response["id"] = messageJson["id"];

#ifdef JSONRPC_MSG_DEBUG
            // Print response
            std::cout << "Send response ";
            jsonStreamWriter->write(response, &std::cout);
            std::cout << std::endl;
#endif

            // Send response
            std::ostream tcpOutStream(&tcpStreambuf);
            jsonStreamWriter->write(response, &tcpOutStream);
            tcpOutStream << static_cast<char>(0x0A);
            asio::write(socket, tcpStreambuf);
        }
    }
    socket.close();
#ifdef JSONRPC_CONNECT_DEBUG
    std::cout << "Client disconnected" << std::endl;
#endif
}
