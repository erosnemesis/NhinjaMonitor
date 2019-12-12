// Example usage for RemoteAlarmReset library by Bachta.

#include "RemoteAlarmReset.h"

// Initialize objects from the lib
RemoteAlarmReset remoteAlarmReset;

void setup() {
    // Call functions on initialized library objects that require hardware
    remoteAlarmReset.begin();
}

void loop() {
    // Use the library's initialized objects and functions
    remoteAlarmReset.process();
}
