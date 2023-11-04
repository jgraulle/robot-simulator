#ifndef ROBOT_COMMAND_HPP
#define ROBOT_COMMAND_HPP

#include "jsonrpctcpserver.hpp"

#include <vector>
#include <SFML/Window/Event.hpp>
#include <mutex>

class Robot;
class IrProximitySensor;
class LineTrackSensor;
class SwitchSensor;
class UltrasonicSensor;
class SpeedSensor;


class RobotCommand : public JsonRpcTcpServer
{
public:
    RobotCommand(Robot & robot, uint16_t tcpPort);
    virtual ~RobotCommand();

    //! @param elapsedTime: in seconds
    void update(float elapsedTime);

    void keyEvent(sf::Event::EventType eventType, sf::Keyboard::Key keyboardCode);

private:
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
