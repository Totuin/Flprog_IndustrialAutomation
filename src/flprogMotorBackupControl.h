#pragma once
#include <Arduino.h>
#include "flprogUtilites.h"

#define FLPROG_MOTOR_BACKUP_CONTROL_MANUAL_MODE 0
#define FLPROG_MOTOR_BACKUP_CONTROL_CAROUSEL_MODE 1
#define FLPROG_MOTOR_BACKUP_CONTROL_HOURS_WORKED_MODE 2

struct FLProgOneBacupMotor
{
    bool isReady = true;
    uint8_t inputNumber = 0;
    uint8_t number = 0;
    bool isRun = false;
    uint32_t hoursWorked = 0;
    uint8_t index = 0;
};

class FLProgMotorBackupControl
{
public:
    FLProgMotorBackupControl(uint8_t motorsCount);

    void readyMotor(uint8_t index, bool value);
    void numberMotor(uint8_t index, uint8_t value);
    void hoursWorkedMotor(uint8_t index, uint32_t value);

    void runMotorsCount(uint8_t count);
    void mode(uint8_t value);
    uint8_t motorNumber(uint8_t index);
    bool motorIsRun(uint8_t index);


protected:
    void updateMotorNumbers();
    FLProgOneBacupMotor *motorAtIndex(uint8_t index);
    bool isNeedSwapMotor(uint8_t index);
    bool isReadyMotor(uint8_t index);
    void checkIsRunMotors();
    void caruseleMotorsOneStep();
    void swapMotor(uint8_t index);

    uint8_t _motorsCount = 0;
    uint8_t _mode = FLPROG_MOTOR_BACKUP_CONTROL_CAROUSEL_MODE;
    FLProgOneBacupMotor *_motors = 0;
    uint8_t _runMotorsCount = 0;
    uint8_t _firstMotorIndex = 0;
};
