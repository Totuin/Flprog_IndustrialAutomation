#pragma once
#include <Arduino.h>
#include "flprogUtilites.h"

typedef void (*FLProgHourMeterCallback)(uint32_t counter);
class FLProgHourMeter
{
public:
    FLProgHourMeter() {};

    void equipmentStatus(bool status);
    void setWorkSeconds(uint32_t value);
    void resetCounter();
    void setCallBack(FLProgHourMeterCallback func) { _callBack = func; };
    uint32_t workTimeAsSeconds() { return _workSecondsCounter; };
    uint8_t seconds() { return _seconds; };
    uint8_t minutes() { return _minutes; };
    uint32_t hours() { return _hours; };

    void pool();

protected:
    void calculateSecondsMinutsHours();
    FLProgHourMeterCallback _callBack = 0;
    bool _equipmentStatus = false;
    uint32_t _workSecondsCounter = 0;
    uint32_t _startTime;
    uint8_t _seconds = 0;
    uint8_t _minutes = 0;
    uint32_t _hours = 0;
};