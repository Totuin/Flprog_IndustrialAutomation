#include "flprogAnalogSignalControl.h"

FLProgAnalogSignalControl::FLProgAnalogSignalControl(uint8_t levelsCount)
{
    _levelsCount = levelsCount;
    if (_levelsCount == 0)
    {
        return;
    }
    _levels = new FLProgAnalogSignalLevel[_levelsCount];
}

void FLProgAnalogSignalControl::calculate()
{
    _outputValue = map(_inputValue, _inputSacaleDown, _inputSacaleUp, _outputSacaleDown, _outputSacaleUp);
    uint8_t reachedLevels = 0;
    for (uint8_t i = 0; i < _levelsCount; i++)
    {
        _levels[i].reached = _levels[i].value <= _outputValue;
        if (_levels[i].reached)
        {
            reachedLevels++;
        }
    }
    if (reachedLevels < 2)
    {
        if (reachedLevels == 0)
        {
            _runMotorsCount = 0;
        }
    }
    else
    {
        if ((reachedLevels - 1) > _runMotorsCount)
        {
            _runMotorsCount = reachedLevels - 1;
        }
    }
    _hasUpError = _inputValue > _upErrorLevel;
    _hasDownError = _inputValue < _downErrorLevel;
}

void FLProgAnalogSignalControl::setLevel(uint8_t index, int32_t value)
{
    if (index >= _levelsCount)
    {
        return;
    }
    if (_levels[index].value == value)
    {
        return;
    }
    _levels[index].value = value;
    calculate();
}

bool FLProgAnalogSignalControl::isReachedLevel(uint8_t index)
{
    if (index >= _levelsCount)
    {
        return false;
    }
    return _levels[index].reached;
}

void FLProgAnalogSignalControl::inputValue(int32_t value)
{
    if (value == _inputValue)
    {
        return;
    }
    _inputValue = value;
    calculate();
}

void FLProgAnalogSignalControl::inputSacaleUp(int32_t value)
{
    if (value == _inputSacaleUp)
    {
        return;
    }
    _inputSacaleUp = value;
    calculate();
}

void FLProgAnalogSignalControl::inputSacaleDown(int32_t value)
{
    if (value == _inputSacaleDown)
    {
        return;
    }
    _inputSacaleDown = value;
    calculate();
}

void FLProgAnalogSignalControl::outputSacaleUp(int32_t value)
{
    if (value == _outputSacaleUp)
    {
        return;
    }
    _outputSacaleUp = value;
    calculate();
}

void FLProgAnalogSignalControl::outputSacaleDown(int32_t value)
{
    if (value == _outputSacaleDown)
    {
        return;
    }
    _outputSacaleDown = value;
    calculate();
}

void FLProgAnalogSignalControl::upErrorLevel(int32_t value)
{
    if (value == _upErrorLevel)
    {
        return;
    }
    _upErrorLevel = value;
    calculate();
}

void FLProgAnalogSignalControl::downErrorLevel(int32_t value)
{
    if (value == _downErrorLevel)
    {
        return;
    }
    _downErrorLevel = value;
    calculate();
}