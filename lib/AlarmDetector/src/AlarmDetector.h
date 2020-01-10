#pragma once

/* AlarmDetector library by Bachta
 */

// This will load the definition for common Particle variable types
#include "Particle.h"
#include "Arduino.h"

#define PUBLISH_NAME_ALARM "Dryer_Alarms" // DO NOT MODIFY THIS. GOOGLE CLOUD PUBSUB DEPENDANCY

// This is your main class that users will import into their application
class AlarmDetector
{
public:
  /**
   * Constructor
   */
  AlarmDetector();

  void setParticleVariables(const uint8_t &alarmCount);
  void alarm0();
  void alarm1();
  void alarm2();
  void alarm3(); 
  const uint8_t getDefaultAlarmCount();
  const byte getAlarmState(const uint8_t &alarm);
  void processAlarm(const uint8_t &alarmNum, const bool &state, const String &deviceName);
  void checkStateOnBoot();

private:

  const byte getPreviousAlarmState(const uint8_t &alarm);
  void setPreviousAlarmState(const uint8_t &alarm, const bool &state);

  static const uint8_t alarmCount = 4;
  int previousAlarmState[alarmCount] = {0, 0, 0, 0};
  const uint8_t hardwareAlarmPin[alarmCount] = {C0, C3, C4, C5};
  const uint8_t hardwareLEDPin[alarmCount] = {D4, D5, D6, D7};
  volatile int alarmState[alarmCount] = {0, 0, 0, 0};
};
