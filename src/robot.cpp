#include "robot.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


Robot::Robot(const sf::Vector2f & size)
    : _shape(std::make_unique<sf::RectangleShape>(size))
    , _linearVelocity(0.0)
    , _angularVelocity(0.0)
    , _irProximitySensors()
{
    _shape->setFillColor(sf::Color::Green);
    _shape->setOrigin(size.x/2.0, size.y/2.0);
}

void Robot::update(float elapsedTime, const sf::Transform & parentWorldTransform)
{
    // Move robot
    sf::Transform orientationTransform(sf::Transform().rotate(getRotation()));
    // TODO Step1 Replace the stub below to compute linearStep and angularStep from _linearVelocity,
    // _angularVelocity, orientationTransform and elapsedTime
    // Begin stub
    float linearStep = 0.0;
    float angularStep = 0.0;
    // End stub

    // Update sensors
    auto wolrdTransform = parentWorldTransform*getTransform();
    for (auto & irProximitySensor : _irProximitySensors)
        irProximitySensor.update(elapsedTime, wolrdTransform);
    for (auto & lineTrackSensor : _lineTrackSensors)
        lineTrackSensor.update(elapsedTime, wolrdTransform);
}

void Robot::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(*_shape, states);
    for (const auto & irProximitySensor : _irProximitySensors)
        irProximitySensor.draw(target, states);
    for (const auto & lineTrackSensor : _lineTrackSensors)
        lineTrackSensor.draw(target, states);
}
