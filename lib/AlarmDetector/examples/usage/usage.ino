// Example usage for AlarmDetector library by Bachta.

#include "AlarmDetector.h"

// Initialize objects from the lib
AlarmDetector alarmDetector;

void setup() {
    // Call functions on initialized library objects that require hardware
    alarmDetector.begin();
}

void loop() {
    // Use the library's initialized objects and functions
    alarmDetector.process();
}
