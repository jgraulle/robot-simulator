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

void JsonRpcTcpServer::sendNotification(const std::string & methodName, const Json::Value & params)
{
    // Prepare message
    Json::Value message;
    message["jsonrpc"] = "2.0";
    message["method"] = methodName;
    message["params"] = params;

    Json::StreamWriterBuilder jsonStreamWriterBuilder;
    jsonStreamWriterBuilder["indentation"] = "";
    std::unique_ptr<Json::StreamWriter> jsonStreamWriter(jsonStreamWriterBuilder.newStreamWriter());
    asio::streambuf tcpStreambuf;
    std::ostream tcpOutStream(&tcpStreambuf);
    {
        const std::lock_guard<std::mutex> lock(_clientSocketsMutex);
        for (auto * socket : _clientSockets)
        {
            jsonStreamWriter->write(message, &tcpOutStream);
            tcpOutStream << static_cast<char>(0x0A);
            asio::write(*socket, tcpStreambuf);
            assert(tcpStreambuf.size() == 0);
        }
    }

#ifdef JSONRPC_MSG_DEBUG
    // Print notification
    std::cout << "Send notification to all client ";
    jsonStreamWriter->write(message, &std::cout);
    std::cout << std::endl;
#endif
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

void JsonRpcTcpServer::bindOnConnectSendNotification(const std::string & methodName, const ParamsGenerator & paramsGenerator)
{
    const std::lock_guard<std::mutex> lock(_onConnectSendNotificationMutex);
    _onConnectSendNotification.insert(std::make_pair(methodName, paramsGenerator));
}

void JsonRpcTcpServer::listen()
{
    std::cout << "Listen on TCP port " << _tcpPort << std::endl;
    asio::ip::tcp::acceptor acceptor(_ioc, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), _tcpPort));
    while (true)
    {
        auto socket = std::make_unique<asio::ip::tcp::socket>(_ioc);
        acceptor.accept(*socket);
        {
            const std::lock_guard<std::mutex> lock(_clientSocketsMutex);
            _clientSockets.insert(socket.get());
        }
#ifdef JSONRPC_CONNECT_DEBUG
        std::cout << "New client connected" << std::endl;
#endif
        std::thread([](JsonRpcTcpServer * thus, std::unique_ptr<asio::ip::tcp::socket> s){thus->session(std::move(s));}, this, std::move(socket)).detach();
    }
}

void JsonRpcTcpServer::session(std::unique_ptr<asio::ip::tcp::socket> socket)
{
    asio::streambuf tcpInStreambuf;
    std::istream tcpInStream(&tcpInStreambuf);
    asio::streambuf tcpOutStreambuf;
    std::ostream tcpOutStream(&tcpOutStreambuf);
    Json::StreamWriterBuilder jsonStreamWriterBuilder;
    jsonStreamWriterBuilder["indentation"] = "";
    std::unique_ptr<Json::StreamWriter> jsonStreamWriter(jsonStreamWriterBuilder.newStreamWriter());

    // Send on connect notifications
    {
        const std::lock_guard<std::mutex> lock(_onConnectSendNotificationMutex);
        for (auto & sendNotification : _onConnectSendNotification)
        {
            // Prepare message
            Json::Value message;
            message["jsonrpc"] = "2.0";
            message["method"] = sendNotification.first;
            message["params"] = sendNotification.second();
            // Send message
            jsonStreamWriter->write(message, &tcpOutStream);
            tcpOutStream << static_cast<char>(0x0A);
            {
                const std::lock_guard<std::mutex> lockSockets(_clientSocketsMutex);
                asio::write(*socket, tcpOutStreambuf);
            }
            assert(tcpOutStreambuf.size() == 0);
        }
    }

    while (socket->is_open())
    {
        // Wait receive complete message
#ifdef JSONRPC_MSG_DEBUG
        std::cout << "wait message..." << std::endl;
#endif
        try {
            asio::read_until(*socket, tcpInStreambuf, 0x0A);
        }
        catch (asio::system_error & e)
        {
            if (   e.code().value() == asio::error::eof
                || e.code().value() == asio::error::basic_errors::connection_reset)
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
            jsonStreamWriter->write(response, &tcpOutStream);
            tcpOutStream << static_cast<char>(0x0A);
            {
                const std::lock_guard<std::mutex> lock(_clientSocketsMutex);
                asio::write(*socket, tcpOutStreambuf);
            }
            assert(tcpOutStreambuf.size() == 0);
        }
    }

    {
        const std::lock_guard<std::mutex> lock(_clientSocketsMutex);
        _clientSockets.erase(socket.get());
    }
#ifdef JSONRPC_CONNECT_DEBUG
    std::cout << "Client disconnected" << std::endl;
#endif
    socket->close();
    socket.release();
}
