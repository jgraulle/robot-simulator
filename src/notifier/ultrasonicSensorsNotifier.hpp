#ifndef ULTRASONIC_SENSORS_NOTIFIER_HPP
#define ULTRASONIC_SENSORS_NOTIFIER_HPP

#include "sensorsNotifier.hpp"
#include "../ultrasonicSensor.hpp"


class UltrasonicSensorsNotifier : public SensorsNotifier<UltrasonicSensor, std::size_t>
{
public:
    UltrasonicSensorsNotifier(JsonRpcTcpServer * server) : SensorsNotifier(server) {}

    std::size_t getValue(UltrasonicSensor * sensor) const override {return sensor->getDistanceDetected();}

    std::string getNotificationName() const override {return "ultrasonicDistanceDetected";}
};


#endif
