#ifndef ENCODER_WHEEL_SENSORS_NOTIFIER_HPP
#define ENCODER_WHEEL_SENSORS_NOTIFIER_HPP

#include "sensorsNotifier.hpp"
#include "../encoderWheelSensor.hpp"


class EncoderWheelSensorsNotifier : public SensorsNotifier<EncoderWheelSensor, std::size_t>
{
public:
    EncoderWheelSensorsNotifier(JsonRpcTcpServer * server) : SensorsNotifier(server) {}

    std::size_t getValue(EncoderWheelSensor * sensor) const override {return sensor->getValue();}

    std::string getNotificationName() const override {return "encoderWheelValue";}
};


#endif
