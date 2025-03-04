#pragma once
#include <Arduino.h>
#include "flprogUtilites.h"

struct FLProgAnalogSignalLevel
{
    int32_t value = 0;
    bool reached = false;
};

class FLProgAnalogSignalControl
{
public:
    FLProgAnalogSignalControl(uint8_t levelsCount);
    void inputValue(int32_t value);
    void inputSacaleUp(int32_t value);
    void inputSacaleDown(int32_t value);
    void outputSacaleUp(int32_t value);
    void outputSacaleDown(int32_t value);
    void upErrorLevel(int32_t value);
    void downErrorLevel(int32_t value);

    void setLevel(uint8_t index, int32_t value);

    int32_t outputValue() { return _outputValue; };
    int32_t runMotorsCount() { return _runMotorsCount; };

    bool hasUpError() { return _hasUpError; };
    bool hasDownError() { return _hasDownError; };

    bool isReachedLevel(uint8_t index);

protected:
    void calculate();
    int32_t _inputValue = 0;
    int32_t _outputValue = 0;
    int32_t _inputSacaleUp = 0;
    int32_t _inputSacaleDown = 0;
    int32_t _outputSacaleUp = 0;
    int32_t _outputSacaleDown = 0;
    FLProgAnalogSignalLevel *_levels = 0;
    uint8_t _levelsCount = 0;
    uint8_t _runMotorsCount = 0;
    int32_t _upErrorLevel = 0;
    int32_t _downErrorLevel = 0;
    bool _hasUpError = false;
    bool _hasDownError = false;
};
