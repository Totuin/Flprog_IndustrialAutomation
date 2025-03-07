#pragma once
#include <Arduino.h>
#include "flprogUtilites.h"

class FLProgDiscreteSignalsControl
{
public:
    FLProgDiscreteSignalsControl(uint8_t levelsCount);
    void controlLevelsCout(uint8_t value);
    void setLevel(uint8_t index, bool value);

    bool getLevel(uint8_t index);
    bool getLevelError() { return _hasErrorLevels; };
    uint8_t getNeedMotorsCount() { return _needMotorsCount; };

protected:
    void calculate();
    void checkErrorLevels();
    void calculateNeedMotorsCount();
    bool *_levels = 0;
    uint8_t _controlLevelsCout = 0;
    uint8_t _levelsCount = 0;
    bool _hasErrorLevels = false;
    uint8_t _needMotorsCount = 0;
};
