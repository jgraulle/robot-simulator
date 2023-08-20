#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "irProximitySensor.hpp"
#include "lineTrackSensor.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
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
    Robot(const sf::Vector2f & size);

    inline void addIrProximitySensor(IrProximitySensor && toAdd)
            {_irProximitySensors.push_back(std::move(toAdd));}

    inline void addLineTrackSensor(LineTrackSensor && toAdd)
            {_lineTrackSensors.push_back(std::move(toAdd));}

    inline const auto & getIrProximitySensors() const
            {return _irProximitySensors;}
    inline const auto & getLineTrackSensors() const
            {return _lineTrackSensors;}

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
    std::unique_ptr<sf::Shape> _shape;
    float _linearVelocity;
    float _angularVelocity;
    std::vector<IrProximitySensor> _irProximitySensors;
    std::vector<LineTrackSensor> _lineTrackSensors;
};

#endif
