#include "robot.hpp"
#include "robotCommand.hpp"
#include "map.hpp"
#include "irProximitySensor.hpp"
#include "lineTrackSensor.hpp"
#include "switchSensor.hpp"
#include "ultrasonicSensor.hpp"
#include "speedSensor.hpp"

#include <SFML/Graphics.hpp>
#include <sstream>
#include <memory>
#include <algorithm>


void event(sf::RenderWindow & window, RobotCommand & robotCommand)
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
        robotCommand.keyEvent(event.type, event.key.code);
    }
}

int main()
{
    Map map("../data/map.png", sf::Color::White);

    sf::RenderWindow window(sf::VideoMode(map.getSize().x, map.getSize().y), "robot-simulator",
            sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    //window.setFramerateLimit(60);

    // Build a robot
    Robot robot(sf::Vector2f(20.0, 20.0), 20.0, map);
    robot.setPosition(window.getView().getSize().x/2.0, 225.0);
    robot.addSensor(std::make_unique<IrProximitySensor>(10.0, 80.0, sf::Vector2f(10.0, 10.0), 0.0, map));
    robot.addSensor(std::make_unique<IrProximitySensor>(10.0, 80.0, sf::Vector2f(10.0, -10.0), 0.0, map));
    robot.addSensor(std::make_unique<LineTrackSensor>(sf::Vector2f(5.0, 0.0), map, 128u));
    robot.addSensor(std::make_unique<SwitchSensor>(sf::Vector2f(11.0, -10.0), 3, 90.0, map));
    robot.addSensor(std::make_unique<SwitchSensor>(sf::Vector2f(11.0, 10.0), 3, -90.0, map));
    robot.addSensor(std::make_unique<UltrasonicSensor>(10.0, 400.0, 30.0, sf::Vector2f(10.0, 0.0), 0.0, map));
    robot.addSensor(std::make_unique<SpeedSensor>(sf::Vector2f(0.0, 5.0), 6.5, 20u));
    robot.addSensor(std::make_unique<SpeedSensor>(sf::Vector2f(0.0, -5.0), 6.5, 20u));
    RobotCommand robotCommand(robot);

    auto lastTime = std::chrono::steady_clock::now();
    int fpsCount = 0;
    float fpsTime = 0.0;
    while (window.isOpen())
    {
        // Update from sf event
        event(window, robotCommand);

        // Update robot command
        robotCommand.update();

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
        window.draw(map);
        window.draw(robot);
        window.display();
    }

    return 0;
}
