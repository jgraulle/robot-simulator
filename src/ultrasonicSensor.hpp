#ifndef ULTRASONIC_SENSOR_HPP
#define ULTRASONIC_SENSOR_HPP

#include "sensor.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

class Map;


class UltrasonicSensor : public Sensor, public sf::Transformable
{
public:
    //! @param distanceMin: in pixel
    //! @param distanceMax: in pixel
    //! @param beamAngle: in degree
    //! @param position: in pixel
    //! @param orientation: in degree
    UltrasonicSensor(std::size_t distanceMin, std::size_t distanceMax, float beamAngle,
            const sf::Vector2f & position, float orientation, const Map & map);

    inline std::size_t getDistanceDetected() const {return _distanceDetected;}

    //! @param elapsedTime: in seconds
    void update(float elapsedTime, const sf::Transform & parentWorldTransform) override;

    void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:
    std::size_t distanceRandomHelper() const;

    const std::size_t _distanceMin;
    const std::size_t _distanceMax;
    const float _beamAngle;
    const Map & _map;
    std::size_t _distanceDetected;
#ifdef COLLISION_DEBUG
    sf::VertexArray _points;
#endif
};

#endif