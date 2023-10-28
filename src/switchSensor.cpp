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
    // Compute line begin and end position
    _line[0].position = worldTranform.transformPoint(sf::Vector2f(0.0, 0.0));
    _line[1].position = worldTranform.transformPoint(sf::Vector2f(_size, 0.0));
    // Detect collision with wall
    _isDetected = _map.getIsCollision(_line[0].position.x, _line[0].position.y)
        || _map.getIsCollision(_line[1].position.x, _line[1].position.y);
    if (!_isDetected)
    {
        for (std::size_t pointIndex = 1u; pointIndex<_size-1; pointIndex++)
        {
            auto pos = worldTranform.transformPoint(sf::Vector2f(pointIndex, 0.0));
            if (_map.getIsCollision(pos.x, pos.y))
            {
                _isDetected = true;
                break;
            }
        }
    }

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
