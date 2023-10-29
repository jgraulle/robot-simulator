#ifndef ROBOT_COMMAND_HPP
#define ROBOT_COMMAND_HPP

#include "SFML/Window/Event.hpp"

#include <vector>

class Robot;
class IrProximitySensor;
class LineTrackSensor;
class SwitchSensor;
class UltrasonicSensor;
class SpeedSensor;

class RobotCommand
{
public:
    RobotCommand(Robot & robot);
    ~RobotCommand();

    void update();
    void keyEvent(sf::Event::EventType eventType, sf::Keyboard::Key keyboardCode);

private:
    Robot & _robot;
    bool _isLineTrackEnabled;
    float _speed;
    std::vector<IrProximitySensor*> _irProximitySensors;
    std::vector<LineTrackSensor *> _lineTrackSensors;
    std::vector<SwitchSensor *> _switchSensors;
    std::vector<UltrasonicSensor *> _ultrasonicSensors;
    std::vector<SpeedSensor *> _speedSensors;
};

#endif
