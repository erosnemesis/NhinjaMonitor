/* AlarmDetector library by Bachta
 */

#include "AlarmDetector.h"

/**
 * Constructor.
 */
AlarmDetector::AlarmDetector()
{
  for(uint8_t i = 0; i < alarmCount; i++){
      pinMode(hardwareAlarmPin[i], INPUT);
  }

  attachInterrupt(digitalPinToInterrupt(hardwareAlarmPin[0]), &AlarmDetector::alarm0, this, CHANGE);
  attachInterrupt(digitalPinToInterrupt(hardwareAlarmPin[1]), &AlarmDetector::alarm1, this, CHANGE);
  attachInterrupt(digitalPinToInterrupt(hardwareAlarmPin[2]), &AlarmDetector::alarm2, this, CHANGE);
  attachInterrupt(digitalPinToInterrupt(hardwareAlarmPin[3]), &AlarmDetector::alarm3, this, CHANGE);
}

const uint8_t AlarmDetector::getAlarmState(uint8_t alarm){
    return alarmState[alarm];
}

void AlarmDetector::alarm0()
{
    alarmState[0] = !digitalRead(hardwareAlarmPin[0]);
}
void AlarmDetector::alarm1()
{
    alarmState[1] = !digitalRead(hardwareAlarmPin[1]);
}
void AlarmDetector::alarm2()
{
    alarmState[2] = !digitalRead(hardwareAlarmPin[2]);
}
void AlarmDetector::alarm3()
{
    alarmState[3] = !digitalRead(hardwareAlarmPin[3]);
}

const uint8_t AlarmDetector::getDefaultAlarmCount()
{
    return alarmCount;
}

const uint8_t AlarmDetector::getPreviousAlarmState(uint8_t alarm){
    return previousAlarmState[alarm];
}

void AlarmDetector::setPreviousAlarmState(uint8_t alarm, bool state){
    previousAlarmState[alarm] = state;
}