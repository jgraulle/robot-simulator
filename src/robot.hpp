#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "map.hpp"
#include "sensor.hpp"
#include "switchSensor.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <memory>
#include <chrono>

namespace sf
{
    class Shape;
}


class Robot : public sf::Drawable, public sf::Transformable
{
public:
    //! @param size: in pixel
    Robot(const sf::Vector2f & size, const Map & wall);

    inline void addSensor(std::unique_ptr<Sensor> && toAdd)
                {_sensors.push_back(std::move(toAdd));}

    inline const auto & getSensors() const
            {return _sensors;}

    //! @return in pixel/seconds
    inline float getLinearVelocity() const {return _linearVelocity;}

    //! @param v: in pixel/seconds
    inline void setLinearVelocity(float v) {_linearVelocity = v;}

    //! @return in degree/seconds
    inline float getAngularVelocity() const {return _angularVelocity;}

    //! @param v: in degree/seconds
    inline void setAngularVelocity(float v) {_angularVelocity = v;}

    //! @param elapsedTime: in seconds
    void update(float elapsedTime, const sf::Transform & parentWorldTransform);

    void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:
    sf::Vector2f _size;
    std::unique_ptr<sf::Shape> _shape;
    std::vector<std::unique_ptr<Sensor>> _sensors;
    const Map & _map;
    float _linearVelocity;
    float _angularVelocity;
    std::vector<SwitchSensor> _collisionDetectors;
};

#endif
