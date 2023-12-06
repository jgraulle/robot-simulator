#ifndef ULTRASOUND_SENSORS_NOTIFIER_HPP
#define ULTRASOUND_SENSORS_NOTIFIER_HPP

#include "sensorsNotifier.hpp"
#include "../ultrasoundSensor.hpp"


class UltrasoundSensorsNotifier : public SensorsNotifier<UltrasoundSensor, std::size_t>
{
public:
    UltrasoundSensorsNotifier(JsonRpcTcpServer * server) : SensorsNotifier(server) {}

    std::size_t getValue(UltrasoundSensor * sensor) const override {return sensor->getDistanceDetected();}

    std::string getNotificationName() const override {return "ultrasoundDistanceDetected";}
};


#endif
