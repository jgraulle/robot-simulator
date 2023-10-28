#include "lineTrackSensor.hpp"

#include "map.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


LineTrackSensor::LineTrackSensor(const sf::Vector2f & position, const Map & map,
        std::uint8_t threshold)
    : _map(map)
    , _shape(std::make_unique<sf::RectangleShape>(sf::Vector2f(DISPLAY_SIZE, DISPLAY_SIZE)))
    , _threshold(threshold)
    , _value(0)
{
    _shape->setOrigin(DISPLAY_SIZE/2.0, DISPLAY_SIZE/2.0);
    setPosition(position);
}

void LineTrackSensor::update(float, const sf::Transform & parentWorldTransform)
{
    auto worldTranform = parentWorldTransform * getTransform();
    _shape->setPosition(worldTranform.transformPoint(sf::Vector2f(0.0, 0.0)));
    // TODO Step1 Replace the stub below to compute _value from _map and _shape->getPosition()
    // Begin stub
    _value = 0u;
    // End stub
}

void LineTrackSensor::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    _shape->setFillColor(sf::Color(_value, _value, _value));
    states.transform = sf::Transform::Identity;
    target.draw(*_shape, states);
}
