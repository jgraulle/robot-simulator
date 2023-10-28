#include "switchSensor.hpp"

#include "map.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


SwitchSensor::SwitchSensor(const sf::Vector2f & position, std::size_t size, float orientation, const Map & map)
    : _size(size)
    , _map(map)
    , _isDetected(false)
    , _line(sf::Lines, 2)
{
    setPosition(position);
    setRotation(orientation);
}

void SwitchSensor::update(float, const sf::Transform & parentWorldTransform)
{
    // Compute world tranform
    auto worldTranform = parentWorldTransform * getTransform();
    // Detect collision with wall
    _line[0].position = worldTranform.transformPoint(sf::Vector2f(0.0, 0.0));
    // TODO Step1 Replace the stub below to compute _isDetected and _line[1].position from
    // _size, worldTranform and _map
    // Begin stub
    _isDetected = false;
    _line[1].position = worldTranform.transformPoint(sf::Vector2f(_size, 0.0));
    // End stub

    // Update color
    sf::Color color = _isDetected ? sf::Color::Red : sf::Color::Blue;
    _line[0].color = color;
    _line[1].color = color;
}

void SwitchSensor::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    states.transform = sf::Transform::Identity;
    target.draw(_line, states);
}
