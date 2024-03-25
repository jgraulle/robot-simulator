#ifndef ENCODER_WHEEL_SENSOR_HPP
#define ENCODER_WHEEL_SENSOR_HPP

#include "sensor.hpp"

class EncoderWheelSensor : public Sensor
{
public:
    //! @brief Create a new encoder wheel sensor.
    //! @param position The position (x, y) of this sensor (on the robot) in pixel.
    //! @param wheelDiameter: The diameter of the driving wheel in pixel.
    //! @param latticeCount: The number of hole in this encoder wheel.
    explicit EncoderWheelSensor(const sf::Vector2f & position, float wheelDiameter,
            std::size_t latticeCount);

    //! @brief Compute the distance measure by this encoder wheel.
    //! @param elapsedTime The time elapsed since the last call of this function in seconds.
    //! @param parentWorldTransform The transform of this sensor to global map.
    void update(float elapsedTime, const sf::Transform & parentWorldTransform) override;

    //! @brief Draw this sensor, here nothing to draw.
    void draw(sf::RenderTarget &, sf::RenderStates) const override {}

    //! @return The last distance measure by this encoder wheel computed in the update() function.
    std::size_t getValue() const {return _value;}

private:
    const sf::Vector2f _position; //!< The position (x, y) of this sensor (on the robot) in pixel.
    //! @brief The distance of the driving wheel in pixel between 2 holes detection.
    const float _latticeStep;
    //! @brief The global position (x, y) of the sensor after last update() in pixel.
    sf::Vector2f _lastGlobalPosition;
    std::size_t _value; //!< The number of hole detected by update() from start.
    float _distance; //!< The distance left over in pixel after conversion in lattice value.
};

#endif
