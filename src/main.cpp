#include "robot.hpp"
#include "map.hpp"
#include "irProximitySensor.hpp"
#include "lineTrackSensor.hpp"
#include "switchSensor.hpp"
#include "ultrasonicSensor.hpp"
#include "speedSensor.hpp"

#include <SFML/Graphics.hpp>
#include <sstream>
#include <memory>


void event(sf::RenderWindow & window, Robot & robot, bool & isIaEnabled, float & speed)
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
                robot.setLinearVelocity(100.0*speed);
                break;
            case sf::Keyboard::S:
            case sf::Keyboard::Down:
                robot.setLinearVelocity(-100.0*speed);
                break;
            case sf::Keyboard::Q:
            case sf::Keyboard::Left:
                robot.setAngularVelocity(-90.0*speed);
                break;
            case sf::Keyboard::D:
            case sf::Keyboard::Right:
                robot.setAngularVelocity(90.0*speed);
                break;
            case sf::Keyboard::I:
            case sf::Keyboard::Space:
                isIaEnabled = !isIaEnabled;
                robot.setLinearVelocity(0.0);
                robot.setAngularVelocity(0.0);
                break;
            case sf::Keyboard::PageUp:
                speed *= 1.1;
                robot.setLinearVelocity(robot.getLinearVelocity()*1.1);
                robot.setAngularVelocity(robot.getAngularVelocity()*1.1);
                break;
            case sf::Keyboard::PageDown:
                speed *= 0.9;
                robot.setLinearVelocity(robot.getLinearVelocity()*0.9);
                robot.setAngularVelocity(robot.getAngularVelocity()*0.9);
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
    Map map("../data/map.png", sf::Color::White);

    sf::RenderWindow window(sf::VideoMode(map.getSize().x, map.getSize().y), "robot-simulator",
            sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    //window.setFramerateLimit(60);

    // Build a robot
    Robot robot(sf::Vector2f(20.0, 20.0), map);
    robot.setPosition(window.getView().getSize().x/2.0, 225.0);
    robot.addSensor(std::make_unique<IrProximitySensor>(10.0, 80.0, sf::Vector2f(10.0, 10.0), 0.0, map));
    robot.addSensor(std::make_unique<IrProximitySensor>(10.0, 80.0, sf::Vector2f(10.0, -10.0), 0.0, map));
    robot.addSensor(std::make_unique<LineTrackSensor>(sf::Vector2f(5.0, 0.0), map, 128u));
    const LineTrackSensor * lineTrackSensor = dynamic_cast<const LineTrackSensor *>(robot.getSensors().back().get());
    robot.addSensor(std::make_unique<SwitchSensor>(sf::Vector2f(11.0, -10.0), 3, 90.0, map));
    robot.addSensor(std::make_unique<SwitchSensor>(sf::Vector2f(11.0, 10.0), 3, -90.0, map));
    robot.addSensor(std::make_unique<UltrasonicSensor>(10.0, 400.0, 30.0, sf::Vector2f(10.0, 0.0), 0.0, map));
    robot.addSensor(std::make_unique<SpeedSensor>(sf::Vector2f(0.0, 5.0), 6.5, 20u));
    robot.addSensor(std::make_unique<SpeedSensor>(sf::Vector2f(0.0, -5.0), 6.5, 20u));

    auto lastTime = std::chrono::steady_clock::now();
    int fpsCount = 0;
    float fpsTime = 0.0;
    bool isIaEnabled = false;
    float speed = 1.0;
    while (window.isOpen())
    {
        event(window, robot, isIaEnabled, speed);

        // Update robot IA
        if (isIaEnabled && lineTrackSensor!=nullptr)
        {
            robot.setLinearVelocity(40.0);
            if (lineTrackSensor->isDetected())
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
        window.draw(map);
        window.draw(robot);
        window.display();
    }

    return 0;
}
