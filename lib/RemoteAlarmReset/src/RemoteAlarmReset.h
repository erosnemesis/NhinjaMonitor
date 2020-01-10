#pragma once

/* RemoteAlarmReset library by Bachta
 */

// This will load the definition for common Particle variable types
#include "Particle.h"
#include "AlarmDetector.h"

// This is your main class that users will import into their application
class RemoteAlarmReset
{
public:
  /**
   * Constructor
   */
  RemoteAlarmReset();

  void process(const uint8_t resetNum);
  const uint8_t getDefaultRelayCount();

private:

  static const uint8_t relayCount = 4;
  const uint16_t RELAY_DELAY = 200;
  const uint16_t hardwareRelayPin[relayCount] = {D0, D1, D2, D3};
};
