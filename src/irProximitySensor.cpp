#include "irProximitySensor.hpp"

#include "map.hpp"

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <assert.h>


IrProximitySensor::IrProximitySensor(float distanceMax, const sf::Vector2f & position, const Map & map)
    : _distanceMax(distanceMax)
    , _map(map)
    , _distanceDetected(0u)
    , _line(sf::Lines, 2)
{
    setPosition(position);

    _line[0].color = sf::Color::Red;
    _line[1].color = sf::Color::Red;
}

void IrProximitySensor::update(float, const sf::Transform & parentWorldTransform)
{
    auto worldTranform = parentWorldTransform * getTransform();
    _line[0].position = worldTranform.transformPoint(sf::Vector2f(0.0, 0.0));
    // TODO Step1 Replace the stub below to compute _distanceDetected and _line[1].position from
    // _distanceMax, _map, worldTranform, position
    // Begin stub
    _distanceDetected = _distanceMax;
    _line[1].position = worldTranform.transformPoint(sf::Vector2f(_distanceDetected, 0.0));
    // End stub
}

void IrProximitySensor::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    states.transform = sf::Transform::Identity;
    target.draw(_line, states);
}
