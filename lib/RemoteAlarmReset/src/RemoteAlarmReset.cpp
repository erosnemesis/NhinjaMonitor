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
        digitalWrite(hardwareRelayPin[i], HIGH);
    }
}

void RemoteAlarmReset::process(const uint8_t resetNum){
    digitalWrite(hardwareRelayPin[resetNum-1], LOW);
    long resetDelay = millis();

    while(millis() - resetDelay < RELAY_DELAY); //wait (this is here instead to delay() so interrupts can occur)
  
    digitalWrite(hardwareRelayPin[resetNum-1], HIGH);
}

const uint8_t RemoteAlarmReset::getDefaultRelayCount(){
    return relayCount;
}