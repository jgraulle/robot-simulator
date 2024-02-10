#include "robotCommand.hpp"

#include "robot.hpp"
#include "irProximitySensor.hpp"
#include "notifier/irProximitySensorsNotifier.hpp"
#include "lineTrackSensor.hpp"
#include "notifier/lineTrackSensorsNotifier.hpp"
#include "encoderWheelSensor.hpp"
#include "notifier/encoderWheelSensorsNotifier.hpp"
#include "switchSensor.hpp"
#include "notifier/switchSensorsNotifier.hpp"
#include "ultrasoundSensor.hpp"
#include "notifier/ultrasoundSensorsNotifier.hpp"

#include <iostream>
#include <json/value.h>
#include <SFML/Graphics/RenderWindow.hpp>


RobotCommand::RobotCommand(Robot & robot, uint16_t tcpPort)
    : JsonRpcTcpServer(tcpPort)
    , _robot(robot)
    , _keyboardSpeed(1.0)
    , _sensorsNotifier()
{
    auto irProximitySensorsNotifier = std::make_unique<IrProximitySensorNotifier>(this);
    auto lineTrackSensorsIsDetectedNotifier = std::make_unique<LineTrackSensorsIsDetectedNotifier>(this);
    auto lineTrackSensorsValueNotifier = std::make_unique<LineTrackSensorsValueNotifier>(this);
    auto encoderWheelSensorsNotifier = std::make_unique<EncoderWheelSensorsNotifier>(this);
    auto switchSensorsNotifier = std::make_unique<SwitchSensorsNotifier>(this);
    auto ultrasoundSensorsNotifier = std::make_unique<UltrasoundSensorsNotifier>(this);

    for (auto & sensor : _robot.getSensors())
    {
        IrProximitySensor * irProximitySensor = dynamic_cast<IrProximitySensor *>(sensor.get());
        if (irProximitySensor != nullptr)
            irProximitySensorsNotifier->add(irProximitySensor);
        LineTrackSensor * lineTrackSensor = dynamic_cast<LineTrackSensor *>(sensor.get());
        if (lineTrackSensor != nullptr)
        {
            lineTrackSensorsIsDetectedNotifier->add(lineTrackSensor);
            lineTrackSensorsValueNotifier->add(lineTrackSensor);
        }
        EncoderWheelSensor * encoderWheelSensor = dynamic_cast<EncoderWheelSensor *>(sensor.get());
        if (encoderWheelSensor != nullptr)
            encoderWheelSensorsNotifier->add(encoderWheelSensor);
        SwitchSensor * switchSensor = dynamic_cast<SwitchSensor *>(sensor.get());
        if (switchSensor != nullptr)
            switchSensorsNotifier->add(switchSensor);
        UltrasoundSensor * ultrasoundSensor = dynamic_cast<UltrasoundSensor *>(sensor.get());
        if (ultrasoundSensor != nullptr)
            ultrasoundSensorsNotifier->add(ultrasoundSensor);
    }

    _sensorsNotifier.push_back(std::move(irProximitySensorsNotifier));
    _sensorsNotifier.push_back(std::move(lineTrackSensorsIsDetectedNotifier));
    _sensorsNotifier.push_back(std::move(lineTrackSensorsValueNotifier));
    _sensorsNotifier.push_back(std::move(encoderWheelSensorsNotifier));
    _sensorsNotifier.push_back(std::move(switchSensorsNotifier));
    _sensorsNotifier.push_back(std::move(ultrasoundSensorsNotifier));

    for (auto & sensorNotifier : _sensorsNotifier)
        sensorNotifier->bind();

    bindNotification("setMotorPower", [this](const Json::Value & params){
        const std::lock_guard<std::mutex> lock(_mutex);
        _robot.setMotorPower(Robot::motorIndexFromString(params["motorIndex"].asString()),
                params["value"].asFloat());
    });
    bindNotification("setMotorsPower", [this](const Json::Value & params){
        const std::lock_guard<std::mutex> lock(_mutex);
        _robot.setMotorsPower(params["rightValue"].asFloat(), params["leftValue"].asFloat());
    });
    bindOnConnectSendNotification("setIsReady", [](){return Json::Value();});
}

RobotCommand::~RobotCommand()
{
}

void RobotCommand::update(float elapsedTime)
{
    const std::lock_guard<std::mutex> lock(_mutex);

    std::vector<bool> isLineTrackDetectedOld;
    _robot.update(elapsedTime);
    for (auto & sensorNotifier : _sensorsNotifier)
        sensorNotifier->notifyChanged();
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
            _robot.setMotorPower(Robot::MotorIndex::LEFT, 0.0);
            _robot.setMotorPower(Robot::MotorIndex::RIGHT, _keyboardSpeed/4);
            break;
        case sf::Keyboard::Left:
        case sf::Keyboard::Q:
            _robot.setMotorPower(Robot::MotorIndex::LEFT, -_keyboardSpeed/4);
            _robot.setMotorPower(Robot::MotorIndex::RIGHT, _keyboardSpeed/4);
            break;
        case sf::Keyboard::W:
            _robot.setMotorPower(Robot::MotorIndex::LEFT, 0.0);
            _robot.setMotorPower(Robot::MotorIndex::RIGHT, -_keyboardSpeed/4);
            break;
        case sf::Keyboard::Up:
        case sf::Keyboard::Z:
            _robot.setMotorPower(Robot::MotorIndex::LEFT, _keyboardSpeed);
            _robot.setMotorPower(Robot::MotorIndex::RIGHT, _keyboardSpeed);
            break;
        case sf::Keyboard::S:
            _robot.setMotorPower(Robot::MotorIndex::LEFT, 0.0);
            _robot.setMotorPower(Robot::MotorIndex::RIGHT, 0.0);
            break;
        case sf::Keyboard::Down:
        case sf::Keyboard::X:
            _robot.setMotorPower(Robot::MotorIndex::LEFT, -_keyboardSpeed);
            _robot.setMotorPower(Robot::MotorIndex::RIGHT, -_keyboardSpeed);
            break;
        case sf::Keyboard::E:
            _robot.setMotorPower(Robot::MotorIndex::LEFT, _keyboardSpeed/4);
            _robot.setMotorPower(Robot::MotorIndex::RIGHT, 0.0);
            break;
        case sf::Keyboard::Right:
        case sf::Keyboard::D:
            _robot.setMotorPower(Robot::MotorIndex::LEFT, _keyboardSpeed/4);
            _robot.setMotorPower(Robot::MotorIndex::RIGHT, -_keyboardSpeed/4);
            break;
        case sf::Keyboard::C:
            _robot.setMotorPower(Robot::MotorIndex::LEFT, -_keyboardSpeed/4);
            _robot.setMotorPower(Robot::MotorIndex::RIGHT, 0.0);
            break;
        case sf::Keyboard::PageUp:
            _keyboardSpeed *= 1.1;
            _robot.setMotorPower(Robot::MotorIndex::LEFT, _robot.getMotorPower(Robot::MotorIndex::LEFT)*1.1);
            _robot.setMotorPower(Robot::MotorIndex::RIGHT, _robot.getMotorPower(Robot::MotorIndex::RIGHT)*1.1);
            break;
        case sf::Keyboard::PageDown:
            _keyboardSpeed *= 0.9;
            _robot.setMotorPower(Robot::MotorIndex::LEFT, _robot.getMotorPower(Robot::MotorIndex::LEFT)*0.9);
            _robot.setMotorPower(Robot::MotorIndex::RIGHT, _robot.getMotorPower(Robot::MotorIndex::RIGHT)*0.9);
            break;
        case sf::Keyboard::Space:
            _robot.reset();
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
            _robot.setMotorPower(Robot::MotorIndex::LEFT, 0.0);
            _robot.setMotorPower(Robot::MotorIndex::RIGHT, 0.0);
        default:
            break;
        }
        break;
    default:
        break;
    }
}
