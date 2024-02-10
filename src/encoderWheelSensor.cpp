#include "encoderWheelSensor.hpp"

#include <cmath>
#include <numbers>
#include <iostream>


EncoderWheelSensor::EncoderWheelSensor(const sf::Vector2f & position, float wheelDiameter,
        std::size_t latticeCount)
    : _position(position)
    , _latticeStep(wheelDiameter*std::numbers::pi/latticeCount)
    , _lastGlobalPosition()
    , _distance(0.0)
    , _value(0u)
{
}

void EncoderWheelSensor::update(float, const sf::Transform & parentWorldTransform)
{
    auto newPosition = parentWorldTransform.transformPoint(_position);
    sf::Vector2f vector(_lastGlobalPosition.x-newPosition.x, _lastGlobalPosition.y-newPosition.y);
    _lastGlobalPosition = newPosition;
    float distance = std::sqrt(vector.x*vector.x+vector.y*vector.y);
    _distance += distance;
    while (_distance>_latticeStep)
    {
        _distance-=_latticeStep;
        _value++;
    }
}
