#include "robot.hpp"
#include "map.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <numeric>
#include <numbers>


Robot::Robot(const sf::Vector2f position, const sf::Vector2f & size, float wheelsDistance, const Map & map)
    : _size(size)
    , _wheelsDistance(wheelsDistance)
    , _motorsPower({0.0, 0.0})
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

void Robot::update(float elapsedTime)
{
    // Compute new position and orientation
    sf::Transform orientationTransform(sf::Transform().rotate(getRotation()));
    float linearStep = (getMotorPower(MotorIndex::LEFT) + getMotorPower(MotorIndex::RIGHT))
            / 2.0 * elapsedTime * MOTOR_POWER_TO_SPEED;
    float angularStep = (getMotorPower(MotorIndex::LEFT) - getMotorPower(MotorIndex::RIGHT))
            / _wheelsDistance * elapsedTime * MOTOR_POWER_TO_SPEED * 180.0 / std::numbers::pi;
    Transformable newTransform;
    newTransform.setPosition(getPosition() + orientationTransform.transformPoint(sf::Vector2f(linearStep, 0.0)));
    newTransform.setRotation(getRotation() + angularStep);

    // Check collision
    bool isCollision = false;
    for (auto & collisionDetector : _collisionDetectors)
    {
        collisionDetector.update(elapsedTime, newTransform.getTransform());
        isCollision |= collisionDetector.isDetected();
        if (isCollision)
            break;
    }

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
    setMotorsPower(0.0, 0.0);
}
