#ifndef SENSORS_NOTIFIER_HPP
#define SENSORS_NOTIFIER_HPP

#include <cstdint>
#include <vector>
#include <memory>
#include <json/value.h>
#include "../jsonrpctcpserver.hpp"


class SensorsNotifierBase
{
public:
    ~SensorsNotifierBase() {}
    virtual void notifyChanged() = 0;
    virtual void bind() = 0;
};


template <typename Sensor, typename Value>
class SensorsNotifier : public SensorsNotifierBase
{
public:
    SensorsNotifier(JsonRpcTcpServer * server)
        : _server(server)
        , _sensorsAndValues()
        , _sensorsAndValuesMutex(std::make_unique<std::mutex>())
    {}

    ~SensorsNotifier() {}

    void add(Sensor * sensor)
    {
        const std::lock_guard<std::mutex> lock(*_sensorsAndValuesMutex);
        _sensorsAndValues.emplace_back(sensor, getValue(sensor), 0);
    }

    inline Sensor * getSensor(int index) {return _sensorsAndValues.at(index)._sensor;}

    void notifyChanged() override
    {
        const std::lock_guard<std::mutex> lock(*_sensorsAndValuesMutex);
        for (std::size_t index = 0; index<_sensorsAndValues.size(); index++)
        {
            SensorAndValue & sensor = _sensorsAndValues.at(index);
            if (sensor._value != getValue(sensor._sensor))
            {
                sensor._value = getValue(sensor._sensor);
                sensor._valueChangedCount++;
                _server->sendNotification(getNotificationName(), valueParams(index));
            }
        }
    }

    void bind() override
    {
        const std::lock_guard<std::mutex> lock(*_sensorsAndValuesMutex);
        for (std::size_t index = 0; index<_sensorsAndValues.size(); index++)
        {
            _server->bindOnConnectSendNotification(getNotificationName(),
                    [this, index](){return valueParams(index);});
        }
    }

    Json::Value valueParams(int index)
    {
        SensorAndValue & sensor = _sensorsAndValues.at(index);
        Json::Value params;
        params["index"] = index;
        params["value"] = sensor._value;
        params["changedCount"] = sensor._valueChangedCount;
        return params;
    }

    virtual Value getValue(Sensor * sensor) const = 0;
    virtual std::string getNotificationName() const = 0;

private:
    struct SensorAndValue
    {
        Sensor * _sensor;
        Value _value;
        int _valueChangedCount;
    };

    JsonRpcTcpServer * _server;
    std::vector<SensorAndValue> _sensorsAndValues;
    //! @brief values can be used in simulator thread (main) with notifyChanged()
    //! and json RCP receive thread with bind() we need a mutex
    std::unique_ptr<std::mutex> _sensorsAndValuesMutex;
};

#endif
