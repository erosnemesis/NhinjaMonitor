// Example usage for CurrentMonitor library by Bachta.

#include "CurrentMonitor.h"

// Initialize objects from the lib
CurrentMonitor currentMonitor;

void setup() {
    // Call functions on initialized library objects that require hardware
    currentMonitor.begin();
}

void loop() {
    // Use the library's initialized objects and functions
    currentMonitor.process();
}
