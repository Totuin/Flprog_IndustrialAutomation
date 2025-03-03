#include "flprogMotorControl.h"

void FLProgMotorControl::comand(bool value)
{
    if (_command == value)
    {
        return;
    }
    _command = value;
    checkWorkStatus();
}

void FLProgMotorControl::externalError(bool value)
{
    if (_hasExternalError == value)
    {
        return;
    }
    _hasExternalError = value;
    checkWorkStatus();
}

void FLProgMotorControl::useRelayProtection(bool value)
{
    if (_useRelayProtection == value)
    {
        return;
    }
    _useRelayProtection = value;
    checkWorkStatus();
}

void FLProgMotorControl::pool()
{
    if (!_useRelayProtection)
    {
        return;
    }
    if (!_isRun)
    {
        return;
    }
    if (_relayProtectionDelay == 0)
    {
        return;
    }
    if (_whateRelayProtectionDelay)
    {
        if (RT_HW_Base.getIsTimerMs(_startTime, _relayProtectionDelay))
        {
            _whateRelayProtectionDelay = false;
        }
        return;
    }
    if (!_workStatus)
    {
        _hasRelayError = true;
        checkWorkStatus();
    }
}

void FLProgMotorControl::resetRelayProtection()
{
    _hasRelayError = false;
    checkWorkStatus();
}

void FLProgMotorControl::checkWorkStatus()
{
    if (_isRun)
    {
        if (!canRun())
        {
            _isRun = false;
        }
        return;
    }
    if (!canRun())
    {
        return;
    }
    _isRun = true;
    _startTime = millis();
    _whateRelayProtectionDelay = true;
}

bool FLProgMotorControl::canRun()
{
    if (!_command)
    {
        return false;
    }
    return !hasError();
}

bool FLProgMotorControl::hasError()
{
    if (_hasExternalError)
    {
        return true;
    }
    return hasRelayError();
}

bool FLProgMotorControl::hasRelayError()
{
    if (!_useRelayProtection)
    {
        return false;
    }
    if (_relayProtectionDelay == 0)
    {
        return false;
    }
    return _hasRelayError;
}