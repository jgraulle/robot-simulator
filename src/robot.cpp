#include "robot.hpp"
#include "map.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


Robot::Robot(const sf::Vector2f & size, const Map & map)
    : _size(size)
    , _shape(std::make_unique<sf::RectangleShape>(size))
    , _sensors()
    , _map(map)
    , _linearVelocity(0.0)
    , _angularVelocity(0.0)
    , _collisionDetectors({
            SwitchSensor(sf::Vector2f(size.x/2.0, size.y/2.0), size.x, -90.0, map),
            SwitchSensor(sf::Vector2f(-size.x/2.0, -size.y/2.0), size.x, 90.0, map)})
{
    _shape->setFillColor(sf::Color::Green);
    _shape->setOrigin(size.x/2.0, size.y/2.0);
}

void Robot::update(float elapsedTime, const sf::Transform & parentWorldTransform)
{
    // Compute new position and orientation
    sf::Transform orientationTransform(sf::Transform().rotate(getRotation()));
    // TODO Step1 Replace the stub below to compute linearStep and angularStep from _linearVelocity,
    // _angularVelocity, orientationTransform and elapsedTime
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
    auto wolrdTransform = parentWorldTransform*getTransform();
    for (auto & sensor : _sensors)
        sensor->update(elapsedTime, wolrdTransform);
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
