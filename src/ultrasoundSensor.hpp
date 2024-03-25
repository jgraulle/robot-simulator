#ifndef ULTRASOUND_SENSOR_HPP
#define ULTRASOUND_SENSOR_HPP

#include "sensor.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

class Map;


class UltrasoundSensor : public Sensor, public sf::Transformable
{
public:
    //! @brief Create a new distance sensor using ultrasound.
    //! @param distanceMin The minimal distance of this sensor can detect in pixel.
    //! @param distanceMax The maximal distance of this sensor can detect in pixel.
    //! @param beamAngle The beam angle in degree of the ultrasound cone detection.
    //! @param position The position (x, y) of this sensor (on the robot) in pixel.
    //! @param orientation The orientation of this sensor (on the robot) in degree.
    //! @param map The global map to get information about the world (here wall).
    UltrasoundSensor(std::size_t distanceMin, std::size_t distanceMax, float beamAngle,
            const sf::Vector2f & position, float orientation, const Map & map);

    //! @return The last distance detected in pixel.
    //! @note This distance is computed in the update() function.
    inline std::size_t getDistanceDetected() const {return _distanceDetected;}

    //! @brief Compute the detected distance.
    //! @param elapsedTime The time elapsed since the last call of this function in seconds.
    //! @param parentWorldTransform The transform of this sensor to global map.
    void update(float elapsedTime, const sf::Transform & parentWorldTransform) override;

    //! @brief Draw this sensor.
    void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:
    std::size_t distanceRandomHelper() const;

    const float _distanceMin; //!< The minimal distance of this sensor can detect in pixel.
    const float _distanceMax; //!< The maximal distance of this sensor can detect in pixel.
    const float _beamAngle; //!< The beam angle in degree of the ultrasound cone detection.
    const Map & _map; //!< To get some information about the world (here wall).
    std::size_t _distanceDetected; //!< The last computed distance detected by update().
#ifdef COLLISION_DEBUG
    sf::VertexArray _points; //!< To draw computed point in debug mode
#endif
};

#endif