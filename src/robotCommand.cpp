#include "robotCommand.hpp"

#include "robot.hpp"
#include "lineTrackSensor.hpp"
#include "irProximitySensor.hpp"
#include "ultrasonicSensor.hpp"
#include "speedSensor.hpp"

#include <iostream>
#include <jsonrpccpp/server/connectors/tcpsocketserver.h>

RobotCommand::RobotCommand(Robot & robot, jsonrpc::TcpSocketServer & tcpServer)
    : AbstractServer<RobotCommand>(tcpServer)
    , _robot(robot), _speed(1.0)
{
    bindAndAddMethod(jsonrpc::Procedure("isLineTrackDetected", jsonrpc::PARAMS_BY_NAME,
            jsonrpc::JSON_BOOLEAN,
            "index", jsonrpc::JSON_INTEGER,
            NULL), &RobotCommand::isLineTrackDetected);
    bindAndAddNotification(jsonrpc::Procedure("setMotorSpeed", jsonrpc::PARAMS_BY_NAME,
            "motorIndex", jsonrpc::JSON_STRING,
            "value", jsonrpc::JSON_REAL,
            NULL), &RobotCommand::setMotorSpeed);
    bindAndAddNotification(jsonrpc::Procedure("setMotorsSpeed", jsonrpc::PARAMS_BY_NAME,
            "rightValue", jsonrpc::JSON_REAL,
            "leftValue", jsonrpc::JSON_REAL,
            NULL), &RobotCommand::setMotorsSpeed);

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

void RobotCommand::update(float elapsedTime)
{
    const std::lock_guard<std::mutex> lock(_mutex);

    _robot.update(elapsedTime);
}

void RobotCommand::keyEvent(sf::Event::EventType eventType, sf::Keyboard::Key keyboardCode)
{
    const std::lock_guard<std::mutex> lock(_mutex);

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

void RobotCommand::isLineTrackDetected(const Json::Value & request, Json::Value & response)
{
    const std::lock_guard<std::mutex> lock(_mutex);
    response = _lineTrackSensors.at(request["index"].asInt())->isDetected();
}

void RobotCommand::setMotorSpeed(const Json::Value & request)
{
    const std::lock_guard<std::mutex> lock(_mutex);
    _robot.setMotorSpeed(Robot::motorIndexFromString(request["motorIndex"].asString()),
            request["value"].asFloat());
}

void RobotCommand::setMotorsSpeed(const Json::Value & request)
{
    const std::lock_guard<std::mutex> lock(_mutex);
    _robot.setMotorsSpeed(request["rightValue"].asFloat(), request["leftValue"].asFloat());
}
