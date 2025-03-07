#include "flprogDiscreteSignalsControl.h"

FLProgDiscreteSignalsControl::FLProgDiscreteSignalsControl(uint8_t levelsCount)
{
    _levelsCount = levelsCount;
    _controlLevelsCout = levelsCount;
    if (_levelsCount == 0)
    {
        return;
    }
    _levels = new bool[_levelsCount];
    for (uint8_t i = 0; i < _levelsCount; i++)
    {
        _levels[i] = false;
    }
}

void FLProgDiscreteSignalsControl::controlLevelsCout(uint8_t value)
{
    if (value == _controlLevelsCout)
    {
        return;
    }
    if (value > _levelsCount)
    {
        if (_controlLevelsCout != _levelsCount)

        {
            _controlLevelsCout = _levelsCount;
            calculate();
        }
        return;
    }
    _controlLevelsCout = value;
    calculate();
}

void FLProgDiscreteSignalsControl::setLevel(uint8_t index, bool value)
{
    if (index >= _levelsCount)
    {
        return;
    }
    if (_levels[index] == value)
    {
        return;
    }
    _levels[index] = value;
    calculate();
}

bool FLProgDiscreteSignalsControl::getLevel(uint8_t index)
{
    if (index >= _levelsCount)
    {
        return false;
    }
    return _levels[index];
}

void FLProgDiscreteSignalsControl::calculate()
{
    checkErrorLevels();
    calculateNeedMotorsCount();
}

void FLProgDiscreteSignalsControl::calculateNeedMotorsCount()
{
    if (_controlLevelsCout == 0)
    {
        _needMotorsCount = 0;
        return;
    }
    if (_controlLevelsCout == 1)
    {
        if (_levels[_levelsCount - 1])
        {
            _needMotorsCount = 1;
        }
        else
        {
            _needMotorsCount = 0;
        }
        return;
    }
    if (_controlLevelsCout == 2)
    {
        if (_levels[_levelsCount - 1])
        {
            _needMotorsCount = 1;
        }
        else
        {
            if (!_levels[0])
            {
                _needMotorsCount = 0;
            }
        }
        return;
    }
    if (!_levels[0])
    {
        _needMotorsCount = 0;
        return;
    }
    uint8_t count = 0;
    if (_levels[_levelsCount - 1])
    {
        count = _controlLevelsCout - 1;
    }
    else
    {
        for (uint8_t i = 1; i < (_controlLevelsCout - 1); i++)
        {
            if (_levels[i])
            {
                count = i;
            }
        }
    }
    if (_needMotorsCount < count)
    {
        _needMotorsCount = count;
    }
}

void FLProgDiscreteSignalsControl::checkErrorLevels()
{
    if (_controlLevelsCout < 2)
    {
        _hasErrorLevels = false;
        return;
    }
    if (_levelsCount < 2)
    {
        _hasErrorLevels = false;
        return;
    }
    bool currentStatus = true;
    for (uint8_t i = 0; i < (_controlLevelsCout - 1); i++)
    {
        if (_levels[i])
        {
            if (!currentStatus)
            {
                _hasErrorLevels = true;
                return;
            }
        }
        else
        {
            currentStatus = false;
        }
    }
    if (!currentStatus)
    {
        if (_levels[_levelsCount - 1])
        {
            _hasErrorLevels = true;
            return;
        }
    }
    _hasErrorLevels = false;
}
