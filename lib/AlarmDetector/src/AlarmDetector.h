#pragma once

/* AlarmDetector library by Bachta
 */

// This will load the definition for common Particle variable types
#include "Particle.h"
#include "Arduino.h"

// This is your main class that users will import into their application
class AlarmDetector
{
public:
  /**
   * Constructor
   */
  AlarmDetector();

  void alarm0();
  void alarm1();
  void alarm2();
  void alarm3();
  const uint8_t getDefaultAlarmCount();
  const byte getAlarmState(uint8_t alarm);
  const byte getPreviousAlarmState(uint8_t alarm);
  void setPreviousAlarmState(uint8_t alarm, bool state);

private:

  static const uint8_t alarmCount = 4;
  volatile byte alarmState[alarmCount] = {0, 0, 0, 0};
  byte previousAlarmState[alarmCount] = {0, 0, 0, 0};
  const uint8_t hardwareAlarmPin[alarmCount] = {C0, C3, C4, C5};
};
