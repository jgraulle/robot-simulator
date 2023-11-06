#ifndef LINE_TRACK_SENSORS_NOTIFIER_HPP
#define LINE_TRACK_SENSORS_NOTIFIER_HPP

#include "sensorsNotifier.hpp"
#include "../lineTrackSensor.hpp"


class LineTrackSensorsIsDetectedNotifier : public SensorsNotifier<LineTrackSensor, bool>
{
public:
    LineTrackSensorsIsDetectedNotifier(JsonRpcTcpServer * server) : SensorsNotifier(server) {}

    bool getValue(LineTrackSensor * sensor) const override {return sensor->isDetected();}

    std::string getNotificationName() const override {return "lineTrackIsDetected";}
};

class LineTrackSensorsValueNotifier : public SensorsNotifier<LineTrackSensor, std::uint8_t>
{
public:
    LineTrackSensorsValueNotifier(JsonRpcTcpServer * server) : SensorsNotifier(server) {}

    std::uint8_t getValue(LineTrackSensor * sensor) const override {return sensor->getValue();}

    std::string getNotificationName() const override {return "lineTrackValue";}
};

#endif
