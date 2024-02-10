#include "robot.hpp"
#include "map.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <numeric>
#include <numbers>
#include <iostream>
#include <iomanip>
#include <cmath>


Robot::Robot(const sf::Vector2f position, const sf::Vector2f & size, float wheelsDistance, const Map & map)
    : _size(size)
    , _wheelsDistance(wheelsDistance)
    , _motorsPower({0.0, 0.0})
    , _motorsSpeed({0.0, 0.0})
    , _shape(std::make_unique<sf::RectangleShape>(size))
    , _sensors()
    , _map(map)
    , _collisionDetectors({
            SwitchSensor(sf::Vector2f(size.x/2.0, size.y/2.0), size.x, -90.0, map),
            SwitchSensor(sf::Vector2f(-size.x/2.0, -size.y/2.0), size.x, 90.0, map)})
    , _positionInit(position)
{
    _shape->setFillColor(sf::Color::Green);
    _shape->setOrigin(size.x/2.0, size.y/2.0);
    setPosition(position);
}

Robot::MotorIndex Robot::motorIndexFromString(const std::string & str)
{
    if (str == "RIGHT")
        return MotorIndex::RIGHT;
    if (str == "LEFT")
        return MotorIndex::LEFT;
    throw std::invalid_argument(std::string("Cannot convert ") + str + " into Robot::MotorIndex");
}

void Robot::setMotorsPower(float rightValue, float leftValue)
{
    if (!checkMotorPowerValue(rightValue))
        return;
    if (!checkMotorPowerValue(leftValue))
        return;
    _motorsPower = {rightValue, leftValue};
}

void Robot::setMotorPower(MotorIndex motorIndex, float value)
{
    if (!checkMotorPowerValue(value))
        return;
    _motorsPower[static_cast<std::size_t>(motorIndex)] = value;
}

void Robot::update(float elapsedTime)
{
    // Compute new speed of each motor
    for (auto motorIndex = 0u; motorIndex<MOTORS_COUNT; motorIndex++)
    {
        float motorForce = _motorsPower.at(motorIndex)*MOTOR_FORCE_MAX;
        // Cap speed max from current PWM by motor force
        if (std::abs(_motorsSpeed.at(motorIndex))>std::abs(MOTOR_SPEED_MAX*_motorsPower.at(motorIndex)))
            motorForce = 0.0;
        // Compute frictional force
        double frictionalForce = 0.0;
        if (_motorsSpeed.at(motorIndex) == 0.0 && std::abs(_motorsPower.at(motorIndex)) < MOTOR_POWER_MIN_TO_START)
            frictionalForce = motorForce;
        else if (motorForce == 0.0)
            frictionalForce = std::copysign(MASS * GRAVITY * FRICTION_DYNAMIC_COEF, _motorsSpeed.at(motorIndex));
        // Compute net force
        double netForce = motorForce - frictionalForce;
        // Compute acceleration
        double acceleration = netForce / MASS;
        // Compute speed
        double newSpeed = _motorsSpeed.at(motorIndex) + acceleration*elapsedTime;
        // Frictional cannot change speed sign
        if (motorForce==0 && std::signbit(newSpeed)!=std::signbit(_motorsSpeed.at(motorIndex)))
            newSpeed = 0.0;
#ifdef MOTOR_DEBUG
        if (motorIndex == 0u)
            std::cout << std::fixed << std::setprecision(3)
                    << "speed=" << std::setw(8) << _motorsSpeed.at(motorIndex) << " pwm=" << _motorsPower.at(motorIndex)
                    << " power=" << std::setw(8) << motorForce << " frictionalForce=" << std::setw(8) << frictionalForce
                    << " netForce=" << std::setw(8) << netForce << " newSpeed=" << std::setw(8) << newSpeed << std::endl;
#endif
        _motorsSpeed.at(motorIndex) = newSpeed;
    }

    // Compute new position and orientation
    sf::Transform orientationTransform(sf::Transform().rotate(getRotation()));
    // TODO Step1 Replace the stub below to compute linearStep and angularStep from
    // getMotorSpeed(RIGHT), getMotorSpeed(LEFT), orientationTransform and elapsedTime
    // Begin stub
    float linearStep = 0.0;
    float angularStep = 0.0;
    // End stub
    Transformable newTransform;
    newTransform.setPosition(getPosition() + orientationTransform.transformPoint(sf::Vector2f(linearStep, 0.0)));
    newTransform.setRotation(getRotation() + angularStep);

    // TODO Step2 Detect robot collision with Wall and do not move or rotate if collision using _map
    // and _collisionDetectors
    // Begin stub
    bool isCollision = false;
    // End stub

    // Move robot if no collision
    if (!isCollision)
    {
        setPosition(newTransform.getPosition());
        setRotation(newTransform.getRotation());
    }

    // Update sensors
    for (auto & sensor : _sensors)
        sensor->update(elapsedTime, getTransform());
}

void Robot::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    // Display shape
    target.draw(*_shape, states);

    // Display sensor
    for (const auto & sensor : _sensors)
        target.draw(*sensor, states);

#ifdef COLLISION_DEBUG
    // Display collision detection debug
    for (const auto & collisionDetector : _collisionDetectors)
        collisionDetector.draw(target, states);
#endif
}

void Robot::reset()
{
    setPosition(_positionInit);
    setRotation(0.0);
    for (auto motorIndex = 0u; motorIndex<MOTORS_COUNT; motorIndex++)
    {
        _motorsPower[motorIndex] = 0.0;
        _motorsSpeed[motorIndex] = 0.0;
    }
}

bool Robot::checkMotorPowerValue(float value)
{
    if (value < -1.0 || 1.0 < value)
    {
        std::cerr << "Try to set motor power with invalid value " << value
                << " value must be between " << -1.0 << " and " << 1.0
                << " command ignored";
        return false;
    }
    return true;
}
