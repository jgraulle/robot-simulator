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
    //! @param position: in pixel
    //! @param size: in pixel
    //! @param orientation: in degree
    SwitchSensor(const sf::Vector2f & position, std::size_t size, float orientation, const Map & map);

    inline bool isDetected() const {return _isDetected;}

    //! @param elapsedTime: in seconds
    void update(float elapsedTime, const sf::Transform & parentWorldTransform) override;

    void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:
    std::size_t _size;
    const Map & _map;
    bool _isDetected;
    sf::VertexArray _line;
};

#endif