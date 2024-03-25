#ifndef SWITCH_SENSOR_HPP
#define SWITCH_SENSOR_HPP

#include "sensor.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <memory>

class Map;


class SwitchSensor : public Sensor, public sf::Transformable
{
public:
    //! @brief Create a new switch sensor to detect collision with obstacle (wall).
    //! @param position The position of this sensor (on the robot) in pixel.
    //! @param size The size of the line to simulate this sensor in pixel.
    //! @param orientation The orientation of the swith (on the robot) in degree.
    SwitchSensor(const sf::Vector2f & position, std::size_t size, float orientation, const Map & map);

    //! @return True if this switch (the line) is in collision with a obstacle (wall) from the last
    //! update() call.
    inline bool isDetected() const {return _isDetected;}

    //! @brief Compute if this sensor is actually in collision with an obstacle.
    //! @param elapsedTime The time elapsed since the last call of this function in seconds.
    //! @param parentWorldTransform The transform of this sensor to global map.
    void update(float elapsedTime, const sf::Transform & parentWorldTransform) override;

    //! @brief Draw this sensor.
    void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:
    std::size_t _size; //!< The size of the line to simulate this sensor in pixel.
    const Map & _map; //!< To get some information about the world (here wall).
    bool _isDetected; //!< The last computed collision detected by update().
    sf::VertexArray _line; //!< To draw this sensor.
};

#endif