#pragma once
#include <Arduino.h>
#include "flprogUtilites.h"

class FLProgMotorControl
{
public:
    FLProgMotorControl() {};
    void resetRelayProtection();
    void relayProtectionDelay(uint32_t value) { _relayProtectionDelay = value; };
    void useRelayProtection(bool value);
    void comand(bool value);
    void workStatus(bool value) { _workStatus = value; };
    void externalError(bool value);

    bool isRun() { return _isRun; };
    bool hasError();
    bool hasRelayError();

    void pool();

protected:
    void checkWorkStatus();
    bool canRun();
    uint32_t _relayProtectionDelay = 1000;
    bool _useRelayProtection = false;
    bool _hasRelayError = false;
    bool _hasExternalError = false;
    bool _isRun = false;
    bool _command = false;
    bool _workStatus = false;
    uint32_t _startTime;
    bool _whateRelayProtectionDelay = false;
};
