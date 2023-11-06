#ifndef SWITCH_SENSORS_NOTIFIER_HPP
#define SWITCH_SENSORS_NOTIFIER_HPP

#include "sensorsNotifier.hpp"
#include "../switchSensor.hpp"


class SwitchSensorsNotifier : public SensorsNotifier<SwitchSensor, bool>
{
public:
    SwitchSensorsNotifier(JsonRpcTcpServer * server) : SensorsNotifier(server) {}

    bool getValue(SwitchSensor * sensor) const override {return sensor->isDetected();}

    std::string getNotificationName() const override {return "switchIsDetected";}
};


#endif
