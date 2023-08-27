#ifndef IR_PROXIMITY_SENSOR_HPP
#define IR_PROXIMITY_SENSOR_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

class Map;


class IrProximitySensor : public sf::Drawable, public sf::Transformable
{
public:
    //! @param distanceMax: in pixel
    //! @param position: in pixel
    //! @param orientation: in degree
    IrProximitySensor(float distanceMax, const sf::Vector2f & position, float orientation,
            const Map & map);

    inline std::size_t getDistanceDetected() const {return _distanceDetected;}

    //! @param elapsedTime: in seconds
    void update(float elapsedTime, const sf::Transform & parentWorldTransform);

    void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:
    const float _distanceMax;
    const Map & _map;
    std::size_t _distanceDetected;
    sf::VertexArray _line;
};

#endif
