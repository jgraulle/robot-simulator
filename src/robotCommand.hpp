#ifndef ROBOT_COMMAND_HPP
#define ROBOT_COMMAND_HPP

#include "jsonrpctcpserver.hpp"
#include "notifier/sensorsNotifier.hpp"

#include <vector>
#include <SFML/Window/Event.hpp>
#include <mutex>

class Robot;
namespace sf {
    class RenderWindow;
}


class RobotCommand : public JsonRpcTcpServer
{
public:
    RobotCommand(Robot & robot, uint16_t tcpPort);
    virtual ~RobotCommand();

    //! @param elapsedTime: in seconds
    void update(float elapsedTime);

    void keyEvent(sf::Event::EventType eventType, sf::Keyboard::Key keyboardCode);

private:
    Robot & _robot;
    float _keyboardSpeed;
    std::vector<std::unique_ptr<SensorsNotifierBase>> _sensorsNotifier;
    std::mutex _mutex; // Robot and sensors can be used in simulator thread (main) and json RCP thread
};

#endif
