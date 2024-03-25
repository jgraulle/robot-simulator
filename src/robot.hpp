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
    static constexpr float MASS = 0.5; //!< The total mass of the robot in kg
    static constexpr float GRAVITY = 9.81; //!< The gravity to simulate friction in m/s^2
    static constexpr float FRICTION_DYNAMIC_COEF = 40.0; //!< To simulate friction
    //! @brief The minimal motor power to be able to move, PWM between -1.0 and 1.0.
    static constexpr float MOTOR_POWER_MIN_TO_START = 0.1;
    static constexpr float MOTOR_FORCE_MAX = 500.0; // The maximum motor force in In kg.m/s^2
    //! @brief The maximum motor speed of the wheel on the ground in pixel / second.
    static constexpr float MOTOR_SPEED_MAX = 200.0;
    enum class MotorIndex {RIGHT = 0, LEFT = 1};
    static constexpr std::size_t MOTORS_COUNT = 2;

    //! @brief Create a new robot.
    //! @param position Starting position in pixel.
    //! @param size Robot size use to draw and detect colision (with wall) in pixel.
    //! @param wheelsDistance The distance between the both well of the robot in pixel.
    //! @param map The global map to get information about the world.
    Robot(const sf::Vector2f position, const sf::Vector2f & size, float wheelsDistance, const Map & wall);

    //! @brief Add a new sensor on this robot.
    inline void addSensor(std::unique_ptr<Sensor> && toAdd)
                {_sensors.push_back(std::move(toAdd));}

    //! @return All sensors of this robot.
    inline const auto & getSensors() const
            {return _sensors;}

    //! @brief Convert a string into motor index (Use by json comunication).
    static MotorIndex motorIndexFromString(const std::string & str);

    //! @return The current motor(s) power(s), PWM between -1.0 and 1.0.
    //! \{
    inline const std::array<float, 2> & getMotorsPower() const {return _motorsPower;}
    inline float getMotorPower(MotorIndex motorIndex) const
            {return _motorsPower.at(static_cast<std::size_t>(motorIndex));}
    //! \}

    //! @brief Set current motor(s) power(s).
    //! @param value PWM between -1.0 and 1.0.
    //! \{
    void setMotorsPower(float rightValue, float leftValue);
    void setMotorPower(MotorIndex motorIndex, float value);
    //! \}

    //! @brief Update the current position of the robot from current motors powers.
    //! @param elapsedTime The time elapsed since the last call of this function in seconds.
    void update(float elapsedTime);

    //! @brief Draw this robot.
    void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

    //! @brief Reset this robot at the stating position and orientation with no speed and motors
    //! powers.
    void reset();

private:
    static bool checkMotorPowerValue(float value);
    inline float getMotorSpeed(MotorIndex motorIndex) const
            {return _motorsSpeed.at(static_cast<std::size_t>(motorIndex));}

    const sf::Vector2f _size; //!< The robot size in pixel.
    const float _wheelsDistance; //!< THe distance between the both wheel in pixel.
    std::array<float, MOTORS_COUNT> _motorsPower; //!< The last motor power, PWM between -1 and 1.
    //! @brief The last computed speed by update() in pixel / second.
    //! @note We dirrectly use speed on the ground, we do not simulate the rotation speed of the
    //! wheel, we do not need the wheel diameter.
    std::array<float, MOTORS_COUNT> _motorsSpeed;
    std::unique_ptr<sf::Shape> _shape; //!< To draw the robot.
    std::vector<std::unique_ptr<Sensor>> _sensors; //!< The sensors list.
    const Map & _map; //!< To get some information about the world.
    std::vector<SwitchSensor> _collisionDetectors; //! To detect wall.
    const sf::Vector2f _positionInit; //! To reset start position.
};

#endif
