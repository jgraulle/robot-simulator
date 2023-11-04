#ifndef JSONRPCCLIENT_HPP
#define JSONRPCCLIENT_HPP

#include <asio/ip/tcp.hpp>
#include <asio/streambuf.hpp>
#include <asio/io_context.hpp>
#include <functional>
#include <map>

namespace Json
{
    class Value;
    class StreamWriter;
}


class JsonRpcTcpServer
{
public:
    JsonRpcTcpServer(unsigned short tcpPort);
    virtual ~JsonRpcTcpServer();

    void startListening();
    void stopListening();

protected:
    using Method = std::function<Json::Value(Json::Value)>;
    using Notification = std::function<void(Json::Value)>;
    void bindMethod(const std::string & methodName, const Method & method);
    void bindNotification(const std::string & notificationName, const Notification & notification);

private:
    void listen();
    void session(asio::ip::tcp::socket sock);

    asio::io_context _ioc;
    unsigned short _tcpPort;
    std::map<std::string, Method> _methods;
    std::mutex _methodsMutex;
    std::map<std::string, Notification> _notifications;
    std::mutex _notificationsMutex;
};

#endif