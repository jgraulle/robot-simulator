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
    Robot(const sf::Vector2f & size, float wheelsDistance, const Map & wall);

    inline void addSensor(std::unique_ptr<Sensor> && toAdd)
                {_sensors.push_back(std::move(toAdd));}

    inline const auto & getSensors() const
            {return _sensors;}

    enum class MotorIndex {RIGHT = 0, LEFT = 1};
    static MotorIndex motorIndexFromString(const std::string & str);

    //! @return PWM between -1.0 and 1.0
    //! \{
    inline const std::array<float, 2> & getMotorsSpeed() const {return _motorsSpeed;}
    inline float getMotorSpeed(MotorIndex motorIndex) const
            {return _motorsSpeed.at(static_cast<std::size_t>(motorIndex));}
    //! \}

    //! @param value: PWM between -1.0 and 1.0
    //! \{
    inline void setMotorsSpeed(float rightValue, float leftValue) {_motorsSpeed = {rightValue, leftValue};}
    inline void setMotorSpeed(MotorIndex motorIndex, float value)
            {_motorsSpeed[static_cast<std::size_t>(motorIndex)] = value;}
    //! \}

    //! @param elapsedTime: in seconds
    void update(float elapsedTime);

    void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:
    const sf::Vector2f _size;
    const float _wheelsDistance;
    std::array<float, 2> _motorsSpeed;
    float MOTOR_SPEED_MAX = 100.0;
    std::unique_ptr<sf::Shape> _shape;
    std::vector<std::unique_ptr<Sensor>> _sensors;
    const Map & _map;
    std::vector<SwitchSensor> _collisionDetectors;
};

#endif
