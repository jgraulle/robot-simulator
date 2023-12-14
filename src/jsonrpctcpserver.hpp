#ifndef JSONRPCCLIENT_HPP
#define JSONRPCCLIENT_HPP

#include <asio/ip/tcp.hpp>
#include <asio/streambuf.hpp>
#include <asio/io_context.hpp>
#include <functional>
#include <set>
#include <map>

namespace Json
{
    class Value;
}


class JsonRpcTcpServer
{
public:
    JsonRpcTcpServer(unsigned short tcpPort);
    virtual ~JsonRpcTcpServer();

    void startListening();
    void stopListening();

    void sendNotification(const std::string & methodName, const Json::Value & params);

    using Method = std::function<Json::Value(Json::Value)>;
    void bindMethod(const std::string & methodName, const Method & method);

    using Notification = std::function<void(Json::Value)>;
    void bindNotification(const std::string & notificationName, const Notification & notification);

    using ParamsGenerator = std::function<Json::Value()>;
    void bindOnConnectSendNotification(const std::string & methodName, const ParamsGenerator & paramsGenerator);

private:
    void listen();
    void session(std::unique_ptr<asio::ip::tcp::socket> socket);

    asio::io_context _ioc;
    unsigned short _tcpPort;
    std::map<std::string, Method> _methods;
    std::mutex _methodsMutex;
    std::map<std::string, Notification> _notifications;
    std::mutex _notificationsMutex;
    std::set<asio::ip::tcp::socket *> _clientSockets;
    std::mutex _clientSocketsMutex;
    std::multimap<std::string, ParamsGenerator> _onConnectSendNotification;
    std::mutex _onConnectSendNotificationMutex;
};

#endif