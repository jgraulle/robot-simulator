#ifndef LINE_TRACK_SENSOR_HPP
#define LINE_TRACK_SENSOR_HPP

#include "sensor.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <memory>
#include <cstdint>

class Map;

namespace sf
{
    class Shape;
}


class LineTrackSensor : public Sensor, public sf::Transformable
{
public:
    //! @param position: in pixel
    LineTrackSensor(const sf::Vector2f & position, const Map & map, std::uint8_t threshold);

    inline bool isDetected() const {return _value>=_threshold;}
    inline std::uint8_t getValue() const {return _value;}

    //! @param elapsedTime: in second
    void update(float elapsedTime, const sf::Transform & parentWorldTransform) override;

    void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

    //! in pixel
    static constexpr float DISPLAY_SIZE = 4.0;

private:
    const Map & _map;
    std::unique_ptr<sf::Shape> _shape;
    const std::uint8_t _threshold;
    std::uint8_t _value;
};

#endif
