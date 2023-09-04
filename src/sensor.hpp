#ifndef SENSOR_HPP
#define SENSOR_HPP

#include <SFML/Graphics/Drawable.hpp>


class Sensor : public sf::Drawable
{
public:
    //! @param elapsedTime: in second
    virtual void update(float elapsedTime, const sf::Transform & parentWorldTransform) = 0;

    virtual ~Sensor() {}
};

#endif
