#ifndef ROBOT_COMMAND_HPP
#define ROBOT_COMMAND_HPP

#include "SFML/Window/Event.hpp"

#include <vector>
#include <jsonrpccpp/server/abstractserver.h>
#include <mutex>

class Robot;
class IrProximitySensor;
class LineTrackSensor;
class SwitchSensor;
class UltrasonicSensor;
class SpeedSensor;
namespace jsonrpc
{
    class TcpSocketServer;
}


class RobotCommand : public jsonrpc::AbstractServer<RobotCommand>
{
public:
    RobotCommand(Robot & robot, jsonrpc::TcpSocketServer & tcpServer);
    virtual ~RobotCommand();

    //! @param elapsedTime: in seconds
    void update(float elapsedTime);

    void keyEvent(sf::Event::EventType eventType, sf::Keyboard::Key keyboardCode);

private:
    void isLineTrackDetected(const Json::Value & request, Json::Value & response);
    void setMotorSpeed(const Json::Value & request);
    void setMotorsSpeed(const Json::Value & request);

    Robot & _robot;
    float _speed;
    std::vector<IrProximitySensor*> _irProximitySensors;
    std::vector<LineTrackSensor *> _lineTrackSensors;
    std::vector<SwitchSensor *> _switchSensors;
    std::vector<UltrasonicSensor *> _ultrasonicSensors;
    std::vector<SpeedSensor *> _speedSensors;
    std::mutex _mutex; // Robot and sensors can be used in simulator thread (main) and json RCP thread
};

#endif
