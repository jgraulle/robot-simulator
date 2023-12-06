#include "ultrasoundSensor.hpp"

#include "map.hpp"

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <assert.h>
#include <algorithm>
#include <map>
#include <utility>
#include <ranges>
#include <random>


UltrasoundSensor::UltrasoundSensor(std::size_t distanceMin, std::size_t distanceMax, float beamAngle,
        const sf::Vector2f & position, float orientation, const Map & map)
    : _distanceMin(distanceMin)
    , _distanceMax(distanceMax)
    , _beamAngle(beamAngle)
    , _map(map)
    , _distanceDetected(0u)
#ifdef COLLISION_DEBUG
    , _points(sf::Points)
#endif
{
    setPosition(position);
    setRotation(orientation);
}

void UltrasoundSensor::update(float, const sf::Transform & parentWorldTransform)
{
    // To speed computation we only compute transformation for each edge
    auto worldTranform = parentWorldTransform * getTransform();
    auto transformEdge1 = worldTranform * sf::Transform().rotate(_beamAngle/2.0);
    auto transformEdge2 = worldTranform * sf::Transform().rotate(-_beamAngle/2.0);
    // For distance min to distance max from the sensor
#ifdef COLLISION_DEBUG
    _points.clear();
#endif
    std::size_t stepCount = 1u;
    std::map<std::size_t, std::size_t> collideDistances;
    for (std::size_t distanceSensor = 1u; distanceSensor<=_distanceMax; distanceSensor++)
    {
        // Compute edge points coords
        sf::Vector2f edge1 = transformEdge1.transformPoint(sf::Vector2f(distanceSensor, 0.0));
        sf::Vector2f edge2 = transformEdge2.transformPoint(sf::Vector2f(distanceSensor, 0.0));
        // Compute distance between edge, to speed computation we use Manhattan distance instead
        // of euclidien distance (square root)
        sf::Vector2f edge1to2Vector(edge2.x - edge1.x, edge2.y - edge1.y);
        float distanceManhattanEdge = std::abs(edge1to2Vector.x)+std::abs(edge1to2Vector.y);
        // If not enought step to have less than STEP_DISTANCE_MAX (with a Manhattan distance)
        // between to tested collision points
        constexpr std::size_t STEP_DISTANCE_MAX = 20u;
        if (distanceManhattanEdge/stepCount>STEP_DISTANCE_MAX)
        {
            // Double the tested collision points
            stepCount *= 2u;
            decltype(collideDistances) collideTemp;
            // Update collision distances container
            for (const auto & [stepIndex, stepDistance] : collideDistances)
            {
                collideTemp.insert({stepIndex*2u, stepDistance});
                // If next new point in beam angle
                if (stepIndex*2u+1u<=stepCount)
                    // Assume this point is also in collision
                    collideTemp.insert({stepIndex*2u+1u, stepDistance});
                // If previous new point in beam angle and not already mark in collision
                if (stepIndex*2u>=1u && !collideTemp.contains(stepIndex*2u-1u))
                    // Assume this point is also in collision
                    collideTemp.insert({stepIndex*2u-1u, stepDistance});
            }
            collideDistances = std::move(collideTemp);
        }
        // For each point to test
        sf::Vector2f stepVector(edge1to2Vector.x/stepCount, edge1to2Vector.y/stepCount);
        for (std::size_t stepIndex = 0u; stepIndex <= stepCount; ++stepIndex)
        {
            // If no collision for this step
            if (!collideDistances.contains(stepIndex))
            {
                // If no new collision
                sf::Vector2f point(edge1.x+stepIndex*stepVector.x, edge1.y+stepIndex*stepVector.y);
                if (_map.getIsCollision(point.x, point.y))
                    // Add this point in collision container
                    collideDistances.insert({stepIndex, distanceSensor});
#ifdef COLLISION_DEBUG
                else
                    // Add this point to display
                    _points.append(sf::Vertex(point, sf::Color::Blue));
#endif
            }
        }
        // If all point in collision
        assert(collideDistances.size() <= stepCount+1u);
        if (collideDistances.size() == stepCount+1u)
            // Stop
            break;
    }
    // If no enought collision
    if (collideDistances.size() <= (stepCount+1u) / 2u)
        // No distance detected
        _distanceDetected = 0;
    else
    {
        // Compute distance detected
        _distanceDetected = _distanceMax * (stepCount+1u-collideDistances.size());
        for (const auto & stepDistance : collideDistances | std::views::values)
            _distanceDetected += stepDistance;
        _distanceDetected /= (stepCount+1u);
        // If distance detected to small
        if (_distanceDetected<_distanceMin)
            _distanceDetected = distanceRandomHelper();
    }
}

void UltrasoundSensor::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    if (_distanceDetected != 0.0)
    {
        // Display cone
        auto transformEdge1 = getTransform() * sf::Transform().rotate(_beamAngle/2.0);
        auto transformEdge2 = getTransform() * sf::Transform().rotate(-_beamAngle/2.0);
        sf::Vector2f start = getTransform().transformPoint(sf::Vector2f(0.0, 0.0));
        sf::Vector2f edge1 = transformEdge1.transformPoint(sf::Vector2f(_distanceDetected, 0.0));
        sf::Vector2f edge2 = transformEdge2.transformPoint(sf::Vector2f(_distanceDetected, 0.0));
        sf::VertexArray lines(sf::LineStrip, 4);
        lines[0] = sf::Vertex(start, sf::Color::Blue);
        lines[1] = sf::Vertex(edge1, sf::Color::Blue);
        lines[2] = sf::Vertex(edge2, sf::Color::Blue);
        lines[3] = lines[0];
        target.draw(lines, states);
    }
#ifdef COLLISION_DEBUG
    // Display each tested points
    states.transform = sf::Transform::Identity;
    target.draw(_points, states);
#endif
}

std::size_t UltrasoundSensor::distanceRandomHelper() const
{
    static std::mt19937 randomGenerator((std::random_device())());
    static std::uniform_int_distribution<std::size_t> distanceRandom(_distanceMin, _distanceMax);

    return distanceRandom(randomGenerator);
}
