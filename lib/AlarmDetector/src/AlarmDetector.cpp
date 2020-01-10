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
      pinMode(hardwareLEDPin[i], OUTPUT);
      digitalWrite(hardwareLEDPin[i], LOW);
  }

  attachInterrupt(digitalPinToInterrupt(hardwareAlarmPin[0]), &AlarmDetector::alarm0, this, CHANGE);
  attachInterrupt(digitalPinToInterrupt(hardwareAlarmPin[1]), &AlarmDetector::alarm1, this, CHANGE);
  attachInterrupt(digitalPinToInterrupt(hardwareAlarmPin[2]), &AlarmDetector::alarm2, this, CHANGE);
  attachInterrupt(digitalPinToInterrupt(hardwareAlarmPin[3]), &AlarmDetector::alarm3, this, CHANGE);
}

void AlarmDetector::setParticleVariables(const uint8_t alarmCount){
    for(int i = 0; i < alarmCount; i++){
        Particle.variable(String("Alarm_" + String(i+1)), &previousAlarmState[i], INT);
    }
}

const uint8_t AlarmDetector::getAlarmState(uint8_t alarm){
    return alarmState[alarm];
}

/**
 * ONLY CALL THIS METHOD IN SETUP! This checks the current state of all the alarms
 * on startup to check if an alarm is already set.
 */
void AlarmDetector::checkStateOnBoot(){
    alarm0();
    alarm1();
    alarm2();
    alarm3();
}

void AlarmDetector::alarm0()
{
    alarmState[0] = digitalRead(hardwareAlarmPin[0]);
}
void AlarmDetector::alarm1()
{
    alarmState[1] = digitalRead(hardwareAlarmPin[1]);
}
void AlarmDetector::alarm2()
{
    alarmState[2] = digitalRead(hardwareAlarmPin[2]);
}
void AlarmDetector::alarm3()
{
    alarmState[3] = digitalRead(hardwareAlarmPin[3]);
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

/**
 * This is executed each loop() to continually check the state
 * of air dryer alarms. Any changes in alarm state get published
 * as an event to Particle.io.
 */
void AlarmDetector::processAlarm(int alarmNum, bool state){
  
  if(state){
    // Only publish the in alarm state if the previous state was not in alarm
    if(!getPreviousAlarmState(alarmNum)){
      String alarmStr = String("Dryer ") + String(alarmNum+1) + String(" in Alarm");
      digitalWrite(hardwareLEDPin[alarmNum], HIGH);
      Particle.publish(PUBLISH_NAME_ALARM, alarmStr, PRIVATE);
    }
    setPreviousAlarmState(alarmNum, true);
    return;
  }

  // Only publish the alarm reset state if the preivous state was in alarm
  if(getPreviousAlarmState(alarmNum)){
    String resetStr = String("Dryer Alarm " + String(alarmNum+1) + " Reset");
    digitalWrite(hardwareLEDPin[alarmNum], LOW);
    Particle.publish(PUBLISH_NAME_ALARM, resetStr, PRIVATE);
  }
  setPreviousAlarmState(alarmNum, false);
}