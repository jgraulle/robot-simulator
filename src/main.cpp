#include "robot.hpp"
#include "robotCommand.hpp"
#include "map.hpp"
#include "irProximitySensor.hpp"
#include "lineTrackSensor.hpp"
#include "switchSensor.hpp"
#include "ultrasoundSensor.hpp"
#include "encoderWheelSensor.hpp"

#include <SFML/Graphics.hpp>
#include <sstream>
#include <memory>
#include <algorithm>
#include <iostream>


constexpr float ROBOT_SIZE = 20.0; //!< The size of the robot in pixel.
constexpr float WHEEL_DISTANCE = ROBOT_SIZE; //!< The distance between the both well of the robot in pixel.
constexpr float WHEEL_DIAMETER = 6.5; //!< The diameter of the driving wheel in pixel.
constexpr std::size_t LATICE_COUNT = 20.0; //!< The number of hole in this encoder wheel.
constexpr float IR_DISTANCE_MIN = 8.0; //!< The minimal distance of IR sensor can detect in pixel.
constexpr float IR_DISTANCE_MAX = 80.0; //!< The maximal distance of IR sensor can detect in pixel.
constexpr float ULTRASOUND_DISTANCE_MIN = 2.0; //!< The minimal distance of ultrasound sensor can detect in pixel.
constexpr float ULTRASOUND_DISTANCE_MAX = 400.0; //!< The maximal distance of ultrasound sensor can detect in pixel.
constexpr float ULTRASOUND_BEAM_ANGLE = 30.0; //!< The beam angle in degree of the ultrasound cone detection.
constexpr std::size_t SWITCH_SIZE = 3u; //!< The size of the line to simulate switch sensor in pixel.

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

int main(int argc, char** argv)
{
    // Parse args for TCP server port
    uint16_t tcpPort = 6543;

    std::string mapFile = "map.png";
    if (argc == 2)
    {
        mapFile = argv[1];
        if (!mapFile.ends_with(".png"))
            mapFile += ".png";
    }
    if (mapFile.find('/') == std::string::npos)
        mapFile = "../data/" + mapFile;
    std::cout << mapFile << std::endl;
    Map map(mapFile, sf::Color::White);

    sf::RenderWindow window(sf::VideoMode(map.getSize().x, map.getSize().y), "robot-simulator",
            sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    //window.setFramerateLimit(60);

    // Build a robot
    Robot robot(sf::Vector2f(90.0, 90.0), sf::Vector2f(ROBOT_SIZE, ROBOT_SIZE), ROBOT_SIZE, map);
    robot.addSensor(std::make_unique<IrProximitySensor>(IR_DISTANCE_MIN, IR_DISTANCE_MAX,
            sf::Vector2f(ROBOT_SIZE/2.0, ROBOT_SIZE/2.0), 90.0, map));
    robot.addSensor(std::make_unique<IrProximitySensor>(IR_DISTANCE_MIN, IR_DISTANCE_MAX,
            sf::Vector2f(ROBOT_SIZE/2.0, -ROBOT_SIZE/2.0), -90.0, map));
    robot.addSensor(std::make_unique<IrProximitySensor>(IR_DISTANCE_MIN, IR_DISTANCE_MAX,
            sf::Vector2f(-ROBOT_SIZE/2.0, ROBOT_SIZE/2.0), 90.0, map));
    robot.addSensor(std::make_unique<IrProximitySensor>(IR_DISTANCE_MIN, IR_DISTANCE_MAX,
            sf::Vector2f(-ROBOT_SIZE/2.0, -ROBOT_SIZE/2.0), -90.0, map));
    robot.addSensor(std::make_unique<LineTrackSensor>(sf::Vector2f(ROBOT_SIZE/4.0, 0.0), map, 128u));
    robot.addSensor(std::make_unique<SwitchSensor>(sf::Vector2f(ROBOT_SIZE/2.0+1.0, ROBOT_SIZE/2.0),
            SWITCH_SIZE, -90.0, map));
    robot.addSensor(std::make_unique<SwitchSensor>(sf::Vector2f(ROBOT_SIZE/2.0+1.0, -ROBOT_SIZE/2.0),
            SWITCH_SIZE, 90.0, map));
    robot.addSensor(std::make_unique<UltrasoundSensor>(ULTRASOUND_DISTANCE_MIN,
            ULTRASOUND_DISTANCE_MAX, ULTRASOUND_BEAM_ANGLE, sf::Vector2f(ROBOT_SIZE/2.0, 0.0), 0.0, map));
    robot.addSensor(std::make_unique<EncoderWheelSensor>(sf::Vector2f(0.0, WHEEL_DISTANCE/2.0),
            WHEEL_DIAMETER, LATICE_COUNT));
    robot.addSensor(std::make_unique<EncoderWheelSensor>(sf::Vector2f(0.0, -WHEEL_DISTANCE/2.0),
            WHEEL_DIAMETER, LATICE_COUNT));
    RobotCommand robotCommand(robot, tcpPort);
    robotCommand.startListening();

    auto lastTime = std::chrono::steady_clock::now();
    int fpsCount = 0;
    float fpsTime = 0.0;
    while (window.isOpen())
    {
        // Update from sf event
        event(window, robotCommand);

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
        robotCommand.update(elapsedTime);

        // Draw map and robot
        window.clear();
        window.draw(map);
        window.draw(robot);
        window.display();
    }

    robotCommand.stopListening();

    return 0;
}
