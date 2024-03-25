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
    //! @brief Create a new sensor to get information about the raw color on the floor.
    //! @param position The position of this sensor (on the robot) in pixel.
    //! @param threshold The value between 0 and 255 from this sensor return true when convert a
    //! raw color into the boolean see isDetected().
    //! @param map The global map to get information about the world (here line color).
    LineTrackSensor(const sf::Vector2f & position, const Map & map, std::uint8_t threshold);

    //! @return True if the last raw color read on the floor (in the last update() call) is more
    //! than threshold.
    inline bool isDetected() const {return _value>=_threshold;}

    //! @return The last raw color read on the floor (in the last update() call) between 0 to 255.
    inline std::uint8_t getValue() const {return _value;}

    //! @brief Compute the raw color read on the floor.
    //! @param elapsedTime The time elapsed since the last call of this function in seconds.
    //! @param parentWorldTransform The transform of this sensor to global map.
    void update(float elapsedTime, const sf::Transform & parentWorldTransform) override;

    //! @brief Draw this sensor.
    void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

    //! @brief The size in pixel to draw this sensor.
    static constexpr float DISPLAY_SIZE = 4.0;

private:
    const Map & _map; //!< To get some information about the world (here wall).
    std::unique_ptr<sf::Shape> _shape; //!< To draw this sensor
    //! @brief The value between 0 and 255 from this sensor return true when convert a raw color
    //! into the boolean see isDetected().
    const std::uint8_t _threshold;
    //! @brief The last raw color read on the floor (in the last update() call) between 0 to 255.
    std::uint8_t _value;
};

#endif
