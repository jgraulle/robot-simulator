#ifndef SPEED_SENSOR_HPP
#define SPEED_SENSOR_HPP

#include "sensor.hpp"

class SpeedSensor : public Sensor
{
public:
    //! @param position: in pixel
    //! @param wheelDiameter: in pixel
    explicit SpeedSensor(const sf::Vector2f & position, float wheelDiameter,
            std::size_t latticeCount);

    //! @param elapsedTime: in seconds
    void update(float elapsedTime, const sf::Transform & parentWorldTransform) override;

    void draw(sf::RenderTarget &, sf::RenderStates) const override {}

    std::size_t getValue() const {return _value;}

private:
    const sf::Vector2f _position;
    const float _latticeStep;
    sf::Vector2f _lastGlobalPosition;
    float _distance;
    std::size_t _value;
};

#endif
