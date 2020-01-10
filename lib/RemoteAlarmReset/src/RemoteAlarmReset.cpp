/* RemoteAlarmReset library by Bachta
 */

#include "RemoteAlarmReset.h"

/**
 * Constructor.
 */
RemoteAlarmReset::RemoteAlarmReset()
{
    for(int i = 0; i < relayCount; i++){
        pinMode(hardwareRelayPin[i], OUTPUT);
        digitalWrite(hardwareRelayPin[i], LOW);
    }
}

void RemoteAlarmReset::process(const uint8_t &resetNum, const String &deviceName){

    Particle.publish(PUBLISH_NAME_ALARM, String("Remote " + deviceName + " Alarm " + String(resetNum) + " Reset Sent"), PRIVATE);

    digitalWrite(hardwareRelayPin[resetNum-1], HIGH);
    long resetDelay = millis();

    while(millis() - resetDelay < RELAY_DELAY); //wait (this is here instead to delay() so interrupts can occur)
  
    digitalWrite(hardwareRelayPin[resetNum-1], LOW);
}

const uint8_t RemoteAlarmReset::getDefaultRelayCount(){
    return relayCount;
}