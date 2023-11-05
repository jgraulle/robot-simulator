#include "lineTrackSensorsNotifier.hpp"

#include "../lineTrackSensor.hpp"
#include "../jsonrpctcpserver.hpp"
#include <json/value.h>
#include <iostream>


LineTrackSensorsNotifier::LineTrackSensorsNotifier(JsonRpcTcpServer * server)
    : _server(server)
    , _sensors()
    , _sensorsMutex(std::make_unique<std::mutex>())
{}

void LineTrackSensorsNotifier::add(LineTrackSensor * sensor)
{
    const std::lock_guard<std::mutex> lock(*_sensorsMutex);
    _sensors.emplace_back(sensor, sensor->isDetected(), 0, sensor->getValue(), 0);
}

void LineTrackSensorsNotifier::notifyChanged()
{
    const std::lock_guard<std::mutex> lock(*_sensorsMutex);
    for (std::size_t index = 0; index<_sensors.size(); index++)
    {
        Sensor & sensor = _sensors.at(index);
        if (sensor._isDetected != sensor._sensor->isDetected())
        {
            sensor._isDetected = sensor._sensor->isDetected();
            sensor._isDetectedChangedCount++;
            Json::Value params;
            params["index"] = index;
            params["value"] = sensor._isDetected;
            params["changedCount"] = sensor._isDetectedChangedCount;
            _server->sendNotification("isLineTrackDetected", params);
        }
        if (sensor._value != sensor._sensor->getValue())
        {
            sensor._value = sensor._sensor->getValue();
            sensor._valueChangedCount++;
            Json::Value params;
            params["index"] = index;
            params["value"] = sensor._value;
            params["changedCount"] = sensor._valueChangedCount;
            _server->sendNotification("lineTrackValue", params);
        }
    }
}

void LineTrackSensorsNotifier::bind()
{
    _server->bindMethod("isLineTrackDetected", [this](const Json::Value & params){
        const std::lock_guard<std::mutex> lock(*_sensorsMutex);
        Sensor & sensor = _sensors.at(params["index"].asInt());
        return Json::Value(sensor._isDetected);
    });
    _server->bindMethod("lineTrackValue", [this](const Json::Value & params){
        const std::lock_guard<std::mutex> lock(*_sensorsMutex);
        Sensor & sensor = _sensors.at(params["index"].asInt());
        return Json::Value(sensor._value);
    });
}
