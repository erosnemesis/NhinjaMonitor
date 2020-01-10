#pragma once

/* CurrentMonitor library by Bachta
 */

// This will load the definition for common Particle variable types
#include "Particle.h"
#include "math.h"

#define ADC_COUNTS (1 << 12)

// This is your main class that users will import into their application
class CurrentMonitor
{
public:
  /**
   * Constructor
   */
  CurrentMonitor(const double voltageRMS
                ,const double halfADCvoltage
                ,const int ctTurns
                ,const int burden
                ,const double boardVoltage
                ,const int samples);

  CurrentMonitor(const double boardVoltage
                ,const int samples);

  double getPower(const uint8_t channel);
  double getIrms(const uint8_t channel, const double calibration);
  

private:

  static const uint8_t maxClampCount = 8;
  const uint8_t hardwareLocation[maxClampCount] = {B2, B3, B4, B5, A0, A1, A6, A7};

  double vcc; // voltage from board
  double vRMS; // assumed voltage
  double offset; // half of ADC max voltage
  int numTurns; // 1:2000 CT turns
  int rBurden; // Burden resistor value
  unsigned int numSamples; // Number of samples before calcultating RMS

  // Sample reading storage location
  int sample;
  double iRMS[maxClampCount];
  double Ical;
  double filtered;
  
  void calculate(const uint8_t channel);
  void calculateIrms(const uint8_t channel);
};
