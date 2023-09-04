#ifndef IR_PROXIMITY_SENSOR_HPP
#define IR_PROXIMITY_SENSOR_HPP

#include "sensor.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

class Map;


class IrProximitySensor : public Sensor, public sf::Transformable
{
public:
    //! @param distanceMin: in pixel
    //! @param distanceMax: in pixel
    //! @param position: in pixel
    //! @param orientation: in degree
    IrProximitySensor(float distanceMin, float distanceMax, const sf::Vector2f & position,
            float orientation, const Map & map);

    inline std::size_t getDistanceDetected() const {return _distanceDetected;}

    //! @param elapsedTime: in seconds
    void update(float elapsedTime, const sf::Transform & parentWorldTransform) override;

    void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:
    const float _distanceMin;
    const float _distanceMax;
    const Map & _map;
    std::size_t _distanceDetected;
    sf::VertexArray _line;
};

#endif
