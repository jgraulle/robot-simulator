#include "robot.hpp"
#include "map.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>


void event(sf::RenderWindow & window, Robot & robot)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch(event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
            case sf::Keyboard::Escape:
                window.close();
                break;
            case sf::Keyboard::Z:
            case sf::Keyboard::Up:
                robot.setLinearVelocity(100.0);
                break;
            case sf::Keyboard::S:
            case sf::Keyboard::Down:
                robot.setLinearVelocity(-100.0);
                break;
            case sf::Keyboard::Q:
            case sf::Keyboard::Left:
                robot.setAngularVelocity(-90.0);
                break;
            case sf::Keyboard::D:
            case sf::Keyboard::Right:
                robot.setAngularVelocity(90.0);
                break;
            default:
                break;
            }
            break;
        case sf::Event::KeyReleased:
            switch (event.key.code)
            {
            case sf::Keyboard::Z:
            case sf::Keyboard::Up:
            case sf::Keyboard::S:
            case sf::Keyboard::Down:
                robot.setLinearVelocity(0.0);
                break;
            case sf::Keyboard::Q:
            case sf::Keyboard::Left:
            case sf::Keyboard::D:
            case sf::Keyboard::Right:
                robot.setAngularVelocity(0.0);
                break;
            default:
                break;
            }
            break;
        case sf::Event::Resized:
        {
            sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
            window.setView(sf::View(visibleArea));
            break;
        }
        default:
            break;
        }
    }
}

int main()
{
    Map wallMap("../data/wall.png");
    Map roadMap("../data/road.png");

    sf::RenderWindow window(sf::VideoMode(wallMap.getSize().x, wallMap.getSize().y), "robot-simulator",
            sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    //window.setFramerateLimit(60);

    // Build a robot
    Robot robot(sf::Vector2f(20.0, 20.0), wallMap);
    robot.setPosition(window.getView().getSize().x/2.0, 225.0);
    robot.addIrProximitySensor(IrProximitySensor(150.0, sf::Vector2f(10.0, 10.0), wallMap));
    robot.addIrProximitySensor(IrProximitySensor(150.0, sf::Vector2f(10.0, -10.0), wallMap));
    robot.addLineTrackSensor(LineTrackSensor(sf::Vector2f(5.0, 0.0), roadMap));

    auto lastTime = std::chrono::steady_clock::now();
    int fpsCount = 0;
    float fpsTime = 0.0;
    while (window.isOpen())
    {
        event(window, robot);

        // Update robot IA
        if (!robot.getLineTrackSensors().empty())
        {
            robot.setLinearVelocity(40.0);
            if (robot.getLineTrackSensors()[0].getDetected())
                robot.setAngularVelocity(-90.0);
            else
                robot.setAngularVelocity(90.0);
        }

        // Compute elapsedTime from last update
        auto currentTime = std::chrono::steady_clock::now();
        float elapsedTime = std::chrono::duration<float>(currentTime-lastTime).count();
        lastTime = currentTime;
        fpsTime += elapsedTime;
        fpsCount++;
        if (fpsTime > 1.0)
        {
            std::ostringstream oss;
            oss << "robot-simulator - " << static_cast<int>(fpsCount) << " fps";
            window.setTitle(oss.str());
            fpsTime -= 1.0;
            fpsCount = 0;
        }

        // Update robot
        robot.update(elapsedTime, sf::Transform::Identity);

        // Draw map and robot
        window.clear();
        window.draw(wallMap);
        window.draw(roadMap);
        window.draw(robot);
        window.display();
    }

    return 0;
}
