#include "robotCommand.hpp"

#include "robot.hpp"
#include "irProximitySensor.hpp"
#include "notifier/irProximitySensorsNotifier.hpp"
#include "lineTrackSensor.hpp"
#include "notifier/lineTrackSensorsNotifier.hpp"
#include "speedSensor.hpp"
#include "notifier/speedSensorsNotifier.hpp"
#include "switchSensor.hpp"
#include "notifier/switchSensorsNotifier.hpp"
#include "ultrasonicSensor.hpp"
#include "notifier/ultrasonicSensorsNotifier.hpp"

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
    auto speedSensorsNotifier = std::make_unique<SpeedSensorsNotifier>(this);
    auto switchSensorsNotifier = std::make_unique<SwitchSensorsNotifier>(this);
    auto ultrasonicSensorsNotifier = std::make_unique<UltrasonicSensorsNotifier>(this);

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
        SpeedSensor * speedSensor = dynamic_cast<SpeedSensor *>(sensor.get());
        if (speedSensor != nullptr)
            speedSensorsNotifier->add(speedSensor);
        SwitchSensor * switchSensor = dynamic_cast<SwitchSensor *>(sensor.get());
        if (switchSensor != nullptr)
            switchSensorsNotifier->add(switchSensor);
        UltrasonicSensor * ultrasonicSensor = dynamic_cast<UltrasonicSensor *>(sensor.get());
        if (ultrasonicSensor != nullptr)
            ultrasonicSensorsNotifier->add(ultrasonicSensor);
    }

    _sensorsNotifier.push_back(std::move(irProximitySensorsNotifier));
    _sensorsNotifier.push_back(std::move(lineTrackSensorsIsDetectedNotifier));
    _sensorsNotifier.push_back(std::move(lineTrackSensorsValueNotifier));
    _sensorsNotifier.push_back(std::move(speedSensorsNotifier));
    _sensorsNotifier.push_back(std::move(switchSensorsNotifier));
    _sensorsNotifier.push_back(std::move(ultrasonicSensorsNotifier));

    for (auto & sensorNotifier : _sensorsNotifier)
        sensorNotifier->bind();

    bindNotification("setMotorSpeed", [this](const Json::Value & params){
        const std::lock_guard<std::mutex> lock(_mutex);
        _robot.setMotorSpeed(Robot::motorIndexFromString(params["motorIndex"].asString()),
                params["value"].asFloat());
    });
    bindNotification("setMotorsSpeed", [this](const Json::Value & params){
        const std::lock_guard<std::mutex> lock(_mutex);
        _robot.setMotorsSpeed(params["rightValue"].asFloat(), params["leftValue"].asFloat());
    });
    bindOnConnectSendNotification("setIsReady", [](){return Json::Value(true);});
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
            _robot.setMotorSpeed(Robot::MotorIndex::LEFT, 0.0);
            _robot.setMotorSpeed(Robot::MotorIndex::RIGHT, _keyboardSpeed/4);
            break;
        case sf::Keyboard::Left:
        case sf::Keyboard::Q:
            _robot.setMotorSpeed(Robot::MotorIndex::LEFT, -_keyboardSpeed/4);
            _robot.setMotorSpeed(Robot::MotorIndex::RIGHT, _keyboardSpeed/4);
            break;
        case sf::Keyboard::W:
            _robot.setMotorSpeed(Robot::MotorIndex::LEFT, 0.0);
            _robot.setMotorSpeed(Robot::MotorIndex::RIGHT, -_keyboardSpeed/4);
            break;
        case sf::Keyboard::Up:
        case sf::Keyboard::Z:
            _robot.setMotorSpeed(Robot::MotorIndex::LEFT, _keyboardSpeed);
            _robot.setMotorSpeed(Robot::MotorIndex::RIGHT, _keyboardSpeed);
            break;
        case sf::Keyboard::S:
            _robot.setMotorSpeed(Robot::MotorIndex::LEFT, 0.0);
            _robot.setMotorSpeed(Robot::MotorIndex::RIGHT, 0.0);
            break;
        case sf::Keyboard::Down:
        case sf::Keyboard::X:
            _robot.setMotorSpeed(Robot::MotorIndex::LEFT, -_keyboardSpeed);
            _robot.setMotorSpeed(Robot::MotorIndex::RIGHT, -_keyboardSpeed);
            break;
        case sf::Keyboard::E:
            _robot.setMotorSpeed(Robot::MotorIndex::LEFT, _keyboardSpeed/4);
            _robot.setMotorSpeed(Robot::MotorIndex::RIGHT, 0.0);
            break;
        case sf::Keyboard::Right:
        case sf::Keyboard::D:
            _robot.setMotorSpeed(Robot::MotorIndex::LEFT, _keyboardSpeed/4);
            _robot.setMotorSpeed(Robot::MotorIndex::RIGHT, -_keyboardSpeed/4);
            break;
        case sf::Keyboard::C:
            _robot.setMotorSpeed(Robot::MotorIndex::LEFT, -_keyboardSpeed/4);
            _robot.setMotorSpeed(Robot::MotorIndex::RIGHT, 0.0);
            break;
        case sf::Keyboard::PageUp:
            _keyboardSpeed *= 1.1;
            _robot.setMotorSpeed(Robot::MotorIndex::LEFT, _robot.getMotorSpeed(Robot::MotorIndex::LEFT)*1.1);
            _robot.setMotorSpeed(Robot::MotorIndex::RIGHT, _robot.getMotorSpeed(Robot::MotorIndex::RIGHT)*1.1);
            break;
        case sf::Keyboard::PageDown:
            _keyboardSpeed *= 0.9;
            _robot.setMotorSpeed(Robot::MotorIndex::LEFT, _robot.getMotorSpeed(Robot::MotorIndex::LEFT)*0.9);
            _robot.setMotorSpeed(Robot::MotorIndex::RIGHT, _robot.getMotorSpeed(Robot::MotorIndex::RIGHT)*0.9);
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
