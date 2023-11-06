#ifndef SPEED_SENSORS_NOTIFIER_HPP
#define SPEED_SENSORS_NOTIFIER_HPP

#include "sensorsNotifier.hpp"
#include "../speedSensor.hpp"


class SpeedSensorsNotifier : public SensorsNotifier<SpeedSensor, std::size_t>
{
public:
    SpeedSensorsNotifier(JsonRpcTcpServer * server) : SensorsNotifier(server) {}

    std::size_t getValue(SpeedSensor * sensor) const override {return sensor->getValue();}

    std::string getNotificationName() const override {return "speedValue";}
};


#endif
