#include "flprogMotorBackupControl.h"

FLProgMotorBackupControl::FLProgMotorBackupControl(uint8_t motorsCount)
{
    _motorsCount = motorsCount;
    if (_motorsCount == 0)
    {
        return;
    }
    _motors = new FLProgOneBacupMotor[_motorsCount];
    for (uint8_t i = 0; i < _motorsCount; i++)
    {
        _motors[i].index = i;
    }
}

FLProgOneBacupMotor *FLProgMotorBackupControl::motorAtIndex(uint8_t index)
{
    if (_motorsCount == 0)
    {
        return 0;
    }
    for (uint8_t i = 0; i < _motorsCount; i++)
    {
        if (_motors[i].index == index)
        {
            return &_motors[i];
        }
    }
    return 0;
}

void FLProgMotorBackupControl::readyMotor(uint8_t index, bool value)
{
    FLProgOneBacupMotor *motor = motorAtIndex(index);
    if (motor == 0)
    {
        return;
    }
    if (motor->isReady == value)
    {
        return;
    }
    motor->isReady = value;
    updateMotorNumbers();
}

void FLProgMotorBackupControl::hoursWorkedMotor(uint8_t index, uint32_t value)
{
    FLProgOneBacupMotor *motor = motorAtIndex(index);
    if (motor == 0)
    {
        return;
    }
    if (motor->hoursWorked == value)
    {
        return;
    }
    motor->hoursWorked = value;
}

void FLProgMotorBackupControl::numberMotor(uint8_t index, uint8_t value)
{
    FLProgOneBacupMotor *motor = motorAtIndex(index);
    if (motor == 0)
    {
        return;
    }
    if (motor->inputNumber == value)
    {
        return;
    }
    motor->inputNumber = value;
    if (_mode != FLPROG_MOTOR_BACKUP_CONTROL_HOURS_WORKED_MODE)
    {
        updateMotorNumbers();
    }
}

void FLProgMotorBackupControl::updateMotorNumbers()
{
    uint8_t count = _motorsCount;
    bool swapped = false;

    while (count--)
    {
        swapped = false;
        for (uint8_t i = 0; i < count; i++)
        {
            if (isNeedSwapMotor(i))
            {
                swapMotor(i);
                swapped = true;
            }
        }
        if (swapped == false)
        {
            break;
        }
    }
    if (_mode == FLPROG_MOTOR_BACKUP_CONTROL_CAROUSEL_MODE)
    {
        uint8_t readyCount = 0;
        for (uint8_t i = 0; i < _motorsCount; i++)
        {
            if (isReadyMotor(i))
            {
                readyCount++;
            }
        }
        if (_firstMotorIndex >= readyCount)
        {
            _firstMotorIndex = 0;
        }
        for (uint8_t i = 0; i < _firstMotorIndex; i++)
        {
            caruseleMotorsOneStep();
        }
    }
    for (uint8_t i = 0; i < _motorsCount; i++)
    {
        _motors[i].number = i;
    }
    checkIsRunMotors();
}

void FLProgMotorBackupControl::swapMotor(uint8_t index)
{
    FLProgOneBacupMotor temp = _motors[index];
    _motors[index] = _motors[index + 1];
    _motors[index + 1] = temp;
}

void FLProgMotorBackupControl::caruseleMotorsOneStep()
{
    if (!isReadyMotor(0))
    {
        return;
    }
    FLProgOneBacupMotor temp = _motors[0];
    for (uint8_t i = 0; i < _motorsCount; i++)
    {
        if (!isReadyMotor(i))
        {
            _motors[i - 1] = temp;
            return;
        }
        _motors[i] = _motors[i + 1];
    }
    _motors[_motorsCount - 1] = temp;
}

bool FLProgMotorBackupControl::isNeedSwapMotor(uint8_t index)
{
    if (isReadyMotor(index + 1) == isReadyMotor(index))
    {
        if (_mode == FLPROG_MOTOR_BACKUP_CONTROL_HOURS_WORKED_MODE)
        {
            return (_motors[index].hoursWorked > _motors[index + 1].hoursWorked);
        }
        else
        {
            return (_motors[index].inputNumber > _motors[index + 1].inputNumber);
        }
    }
    return isReadyMotor(index + 1);
}

void FLProgMotorBackupControl::runMotorsCount(uint8_t count)
{
    if (_initRunMotorsCount == count)
    {
        return;
    }
    if (count == 0)
    {
        if (_initRunMotorsCount > 0)
        {
            if (_mode == FLPROG_MOTOR_BACKUP_CONTROL_HOURS_WORKED_MODE)
            {
                updateMotorNumbers();
            }

            if (_mode == FLPROG_MOTOR_BACKUP_CONTROL_CAROUSEL_MODE)
            {
                _firstMotorIndex++;
                updateMotorNumbers();
            }
        }
    }
    _initRunMotorsCount = count;
    if ((_initRunMotorsCount == 0) || (_pauseTime == 0) || (_initRunMotorsCount < _runMotorsCount))
    {
        _runMotorsCount = _initRunMotorsCount;
        checkIsRunMotors();
        return;
    }
    if (_runMotorsCount == 0)
    {
        _runMotorsCount = 1;
        _startPauseTime = millis();
        checkIsRunMotors();
        return;
    }
    if (RT_HW_Base.getIsTimerMs(_startPauseTime, _pauseTime))
    {
        _runMotorsCount++;
        _startPauseTime = millis();
    }
    checkIsRunMotors();
}

void FLProgMotorBackupControl::mode(uint8_t value)
{
    if (value > FLPROG_MOTOR_BACKUP_CONTROL_HOURS_WORKED_MODE)
    {
        return;
    }
    if (_mode == value)
    {
        return;
    }
    _mode = value;
    updateMotorNumbers();
}

void FLProgMotorBackupControl::checkIsRunMotors()
{
    for (uint8_t i = 0; i < _motorsCount; i++)
    {
        _motors[i].isRun = ((i < _runMotorsCount) && (isReadyMotor(i)));
    }
}

uint8_t FLProgMotorBackupControl::motorNumber(uint8_t index)
{
    FLProgOneBacupMotor *motor = motorAtIndex(index);
    if (motor == 0)
    {
        return 255;
    }
    return ((motor->number) + 1);
}

bool FLProgMotorBackupControl::motorIsRun(uint8_t index)
{
    FLProgOneBacupMotor *motor = motorAtIndex(index);
    if (motor == 0)
    {
        return false;
    }
    return motor->isRun;
}

bool FLProgMotorBackupControl::isReadyMotor(uint8_t index)
{
    if (_motors[index].inputNumber == 0)
    {
        return false;
    }
    return _motors[index].isReady;
}

void FLProgMotorBackupControl::pool()
{
    if (_runMotorsCount >= _initRunMotorsCount)
    {
        return;
    }
    if (_pauseTime == 0)
    {
        _runMotorsCount = _initRunMotorsCount;
        return;
    }
    if (!RT_HW_Base.getIsTimerMs(_startPauseTime, _pauseTime))
    {
        return;
    }
    _startPauseTime = millis();
    _runMotorsCount++;
    checkIsRunMotors();
}

void FLProgMotorBackupControl::pauseTime(uint32_t value)
{
    _pauseTime = value;
}