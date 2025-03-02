#include "flprogHourMeter.h"

void FLProgHourMeter::equipmentStatus(bool status)
{
    if ((status) && (!_equipmentStatus))
    {
        _startTime = millis();
    }
    if ((!status) && (_equipmentStatus) && (_callBack != 0))
    {
        _callBack(_workSecondsCounter);
    }
    _equipmentStatus = status;
}

void FLProgHourMeter::pool()
{
    if (!_equipmentStatus)
    {
        return;
    }
    if (!RT_HW_Base.getIsTimerUs(_startTime, 1000))
    {
        return;
    }
    _startTime = millis();
    _workSecondsCounter++;
    calculateSecondsMinutsHours();
}

void FLProgHourMeter::calculateSecondsMinutsHours()
{
    uint32_t oldHours = _hours;
    uint32_t temp;
    _hours = (uint32_t)(_workSecondsCounter / 3600);
    temp = _workSecondsCounter - (_hours * 3600);
    _minutes = (uint8_t)(temp / 60);
    _seconds = temp - (_minutes * 60);
    if (_callBack == 0)
    {
        return;
    }
    if (oldHours == _hours)
    {
        return;
    }
    _callBack(_workSecondsCounter);
}

void FLProgHourMeter::setWrkSeconds(uint32_t value)
{
    _workSecondsCounter = value;
    calculateSecondsMinutsHours();
}

void FLProgHourMeter::resetCounter()
{
    _workSecondsCounter = 0;
    calculateSecondsMinutsHours();
}