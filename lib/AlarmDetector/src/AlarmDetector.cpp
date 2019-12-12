/* AlarmDetector library by Bachta
 */

#include "AlarmDetector.h"

/**
 * Constructor.
 */
AlarmDetector::AlarmDetector()
{
  for(uint8_t i = 0; i < alarmCount; i++){
      pinMode(ALARM_PIN[i], INPUT);
  }

  attachInterrupt(digitalPinToInterrupt(ALARM_PIN[0]), &AlarmDetector::alarm0, this, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ALARM_PIN[1]), &AlarmDetector::alarm1, this, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ALARM_PIN[2]), &AlarmDetector::alarm2, this, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ALARM_PIN[3]), &AlarmDetector::alarm3, this, CHANGE);
}

void AlarmDetector::alarm0()
{
    alarmState[0] = digitalRead(ALARM_PIN[0]);
}
void AlarmDetector::alarm1()
{
    alarmState[1] = digitalRead(ALARM_PIN[1]);
}
void AlarmDetector::alarm2()
{
    alarmState[2] = digitalRead(ALARM_PIN[2]);
}
void AlarmDetector::alarm3()
{
    alarmState[3] = digitalRead(ALARM_PIN[3]);
}


uint8_t AlarmDetector::getDefaultAlarmCount()
{
    return alarmCount;
}
