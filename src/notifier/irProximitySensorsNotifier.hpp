#ifndef IR_PROXIMITY_SENSORS_NOTIFIER_HPP
#define IR_PROXIMITY_SENSORS_NOTIFIER_HPP

#include "sensorsNotifier.hpp"
#include "../irProximitySensor.hpp"


class IrProximitySensorNotifier : public SensorsNotifier<IrProximitySensor, std::size_t>
{
public:
    IrProximitySensorNotifier(JsonRpcTcpServer * server) : SensorsNotifier(server) {}

    std::size_t getValue(IrProximitySensor * sensor) const override {return sensor->getDistanceDetected();}
    std::string getNotificationName() const override {return "irProximityDistanceDetected";}
};

#endif
