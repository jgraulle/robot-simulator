#ifndef LINE_TRACK_SENSOR_HPP
#define LINE_TRACK_SENSOR_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <memory>

class Map;

namespace sf
{
    class Shape;
}


class LineTrackSensor : public sf::Drawable, public sf::Transformable
{
public:
    //! @param position: in pixel
    LineTrackSensor(const sf::Vector2f & position, const Map & map);

    inline bool getDetected() const {return _detected;}

    //! @param elapsedTime: in second
    void update(float elapsedTime, const sf::Transform & parentWorldTransform);

    void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

    //! in pixel
    static constexpr float DISPLAY_SIZE = 4.0;

private:
    const Map & _map;
    std::unique_ptr<sf::Shape> _shape;
    bool _detected;
};

#endif
