#include "robotCommand.hpp"

#include "robot.hpp"
#include "lineTrackSensor.hpp"
#include "irProximitySensor.hpp"
#include "ultrasonicSensor.hpp"
#include "speedSensor.hpp"


RobotCommand::RobotCommand(Robot & robot)
    : _robot(robot), _isLineTrackEnabled(false), _speed(1.0)
{
    for (auto & sensor : _robot.getSensors())
    {
        LineTrackSensor * lineTrackSensor = dynamic_cast<LineTrackSensor *>(sensor.get());
        if (lineTrackSensor != nullptr)
            _lineTrackSensors.push_back(lineTrackSensor);
        IrProximitySensor * irProximitySensor = dynamic_cast<IrProximitySensor *>(sensor.get());
        if (irProximitySensor != nullptr)
            _irProximitySensors.push_back(irProximitySensor);
        SwitchSensor * switchSensor = dynamic_cast<SwitchSensor *>(sensor.get());
        if (switchSensor != nullptr)
            _switchSensors.push_back(switchSensor);
        UltrasonicSensor * ultrasonicSensor = dynamic_cast<UltrasonicSensor *>(sensor.get());
        if (ultrasonicSensor != nullptr)
            _ultrasonicSensors.push_back(ultrasonicSensor);
        SpeedSensor * speedSensor = dynamic_cast<SpeedSensor *>(sensor.get());
        if (speedSensor != nullptr)
            _speedSensors.push_back(speedSensor);
    }
}

RobotCommand::~RobotCommand()
{
}

void RobotCommand::update()
{
    if (_isLineTrackEnabled && !_lineTrackSensors.empty())
    {
        if (_lineTrackSensors.at(0)->isDetected())
            _robot.setMotorsSpeed({0.5, 1.0});
        else
            _robot.setMotorsSpeed({1.0, 0.5});
    }
}

void RobotCommand::keyEvent(sf::Event::EventType eventType, sf::Keyboard::Key keyboardCode)
{
    switch(eventType)
    {
    case sf::Event::KeyPressed:
        switch (keyboardCode)
        {
        case sf::Keyboard::A:
            _robot.setMotorSpeed(Robot::MotorIndex::LEFT, 0.0);
            _robot.setMotorSpeed(Robot::MotorIndex::RIGHT, _speed/4);
            break;
        case sf::Keyboard::Left:
        case sf::Keyboard::Q:
            _robot.setMotorSpeed(Robot::MotorIndex::LEFT, -_speed/4);
            _robot.setMotorSpeed(Robot::MotorIndex::RIGHT, _speed/4);
            break;
        case sf::Keyboard::W:
            _robot.setMotorSpeed(Robot::MotorIndex::LEFT, 0.0);
            _robot.setMotorSpeed(Robot::MotorIndex::RIGHT, -_speed/4);
            break;
        case sf::Keyboard::Up:
        case sf::Keyboard::Z:
            _robot.setMotorSpeed(Robot::MotorIndex::LEFT, _speed);
            _robot.setMotorSpeed(Robot::MotorIndex::RIGHT, _speed);
            break;
        case sf::Keyboard::S:
            _robot.setMotorSpeed(Robot::MotorIndex::LEFT, 0.0);
            _robot.setMotorSpeed(Robot::MotorIndex::RIGHT, 0.0);
            break;
        case sf::Keyboard::Down:
        case sf::Keyboard::X:
            _robot.setMotorSpeed(Robot::MotorIndex::LEFT, -_speed);
            _robot.setMotorSpeed(Robot::MotorIndex::RIGHT, -_speed);
            break;
        case sf::Keyboard::E:
            _robot.setMotorSpeed(Robot::MotorIndex::LEFT, _speed/4);
            _robot.setMotorSpeed(Robot::MotorIndex::RIGHT, 0.0);
            break;
        case sf::Keyboard::Right:
        case sf::Keyboard::D:
            _robot.setMotorSpeed(Robot::MotorIndex::LEFT, _speed/4);
            _robot.setMotorSpeed(Robot::MotorIndex::RIGHT, -_speed/4);
            break;
        case sf::Keyboard::C:
            _robot.setMotorSpeed(Robot::MotorIndex::LEFT, -_speed/4);
            _robot.setMotorSpeed(Robot::MotorIndex::RIGHT, 0.0);
            break;
        case sf::Keyboard::PageUp:
            _speed *= 1.1;
            _robot.setMotorSpeed(Robot::MotorIndex::LEFT, _robot.getMotorSpeed(Robot::MotorIndex::LEFT)*1.1);
            _robot.setMotorSpeed(Robot::MotorIndex::RIGHT, _robot.getMotorSpeed(Robot::MotorIndex::RIGHT)*1.1);
            break;
        case sf::Keyboard::PageDown:
            _speed *= 0.9;
            _robot.setMotorSpeed(Robot::MotorIndex::LEFT, _robot.getMotorSpeed(Robot::MotorIndex::LEFT)*0.9);
            _robot.setMotorSpeed(Robot::MotorIndex::RIGHT, _robot.getMotorSpeed(Robot::MotorIndex::RIGHT)*0.9);
            break;
        case sf::Keyboard::I:
        case sf::Keyboard::Space:
            _isLineTrackEnabled = !_isLineTrackEnabled;
            _robot.setMotorsSpeed({0.0, 0.0});
            break;
        default:
            break;
        }
        break;
    case sf::Event::KeyReleased:
        switch (keyboardCode)
        {
        case sf::Keyboard::Up:
        case sf::Keyboard::Down:
        case sf::Keyboard::Left:
        case sf::Keyboard::Right:
            _robot.setMotorSpeed(Robot::MotorIndex::LEFT, 0.0);
            _robot.setMotorSpeed(Robot::MotorIndex::RIGHT, 0.0);
        default:
            break;
        }
        break;
    default:
        break;
    }
}