#include "speedSensor.hpp"

#include <cmath>
#include <numbers>
#include <iostream>


SpeedSensor::SpeedSensor(const sf::Vector2f & position, float wheelDiameter,
        std::size_t latticeCount)
    : _position(position)
    , _latticeStep(wheelDiameter*std::numbers::pi/latticeCount)
    , _lastGlobalPosition()
    , _distance(0.0)
    , _value(0u)
{
}

void SpeedSensor::update(float, const sf::Transform & parentWorldTransform)
{
    auto newPosition = parentWorldTransform.transformPoint(_position);
    // TODO Step1 Replace the stub below to compute _distance and _value from
    // _lastGlobalPosition, newPosition and _latticeStep
    // Begin stub
    _distance += 0.0;
    _value += 0u;
    // End stub
}
