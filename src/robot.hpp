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
    static constexpr float MASS = 0.5; // In kg
    static constexpr float GRAVITY = 9.81; // In m/s^2
    static constexpr float FRICTION_DYNAMIC_COEF = 40.0;
    static constexpr float MOTOR_POWER_MIN_TO_START = 0.1; // PWM between -1.0 and 1.0
    static constexpr float MOTOR_FORCE_MAX = 500.0; // In kg.m/s^2
    static constexpr float MOTOR_SPEED_MAX = 200.0; // In pixel / second
    enum class MotorIndex {RIGHT = 0, LEFT = 1};
    static constexpr std::size_t MOTORS_COUNT = 2;

    //! @param size: in pixel
    Robot(const sf::Vector2f position, const sf::Vector2f & size, float wheelsDistance, const Map & wall);

    inline void addSensor(std::unique_ptr<Sensor> && toAdd)
                {_sensors.push_back(std::move(toAdd));}

    inline const auto & getSensors() const
            {return _sensors;}

    static MotorIndex motorIndexFromString(const std::string & str);

    //! @return PWM between -1.0 and 1.0
    //! \{
    inline const std::array<float, 2> & getMotorsPower() const {return _motorsPower;}
    inline float getMotorPower(MotorIndex motorIndex) const
            {return _motorsPower.at(static_cast<std::size_t>(motorIndex));}
    //! \}

    //! @param value: PWM between -1.0 and 1.0
    //! \{
    void setMotorsPower(float rightValue, float leftValue);
    void setMotorPower(MotorIndex motorIndex, float value);
    //! \}

    //! @param elapsedTime: in seconds
    void update(float elapsedTime);

    void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

    void reset();

private:
    static bool checkMotorPowerValue(float value);
    inline float getMotorSpeed(MotorIndex motorIndex) const
            {return _motorsSpeed.at(static_cast<std::size_t>(motorIndex));}

    const sf::Vector2f _size;
    const float _wheelsDistance;
    std::array<float, MOTORS_COUNT> _motorsPower;
    std::array<float, MOTORS_COUNT> _motorsSpeed;
    std::unique_ptr<sf::Shape> _shape;
    std::vector<std::unique_ptr<Sensor>> _sensors;
    const Map & _map;
    std::vector<SwitchSensor> _collisionDetectors;
    const sf::Vector2f _positionInit;
};

#endif
