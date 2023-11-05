#ifndef LINE_TRACK_SENSORS_NOTIFIER_HPP
#define LINE_TRACK_SENSORS_NOTIFIER_HPP

#include <cstdint>
#include <vector>
#include <memory>

class LineTrackSensor;
class JsonRpcTcpServer;


class LineTrackSensorsNotifier
{
public:
    LineTrackSensorsNotifier(JsonRpcTcpServer * server);

    void add(LineTrackSensor * sensor);

    inline LineTrackSensor * getSensor(int index) {return _sensors.at(index)._sensor;}

    void notifyChanged();
    void bind();

private:
    struct Sensor
    {
        LineTrackSensor * _sensor;
        bool _isDetected;
        int _isDetectedChangedCount;
        std::uint8_t _value;
        int _valueChangedCount;
    };

    JsonRpcTcpServer * _server;
    std::vector<Sensor> _sensors;
    //! @brief values (_isDetected and _value) can be used in simulator thread (main) with notify()
    //! and json RCP receive thread with bind()
    std::unique_ptr<std::mutex> _sensorsMutex;
};

#endif
