/* CurrentMonitor library by Bachta
 */

#include "CurrentMonitor.h"

/**
 * Constructor.
 */
CurrentMonitor::CurrentMonitor(const double voltageRMS
                ,const double halfADCvoltage
                ,const int ctTurns
                ,const int burden
                ,const double boardVoltage
                ,const int samples)
{
  vRMS = voltageRMS;
  offset = halfADCvoltage;
  numTurns = ctTurns;
  rBurden = burden;
  vcc = boardVoltage;
  numSamples = samples;
}

CurrentMonitor::CurrentMonitor(const double boardVoltage
                ,const int samples)
{
  vcc = boardVoltage;
  numSamples = samples;
  
  offset = ADC_COUNTS>>1;
}

/**
 * Processes the given channel and returns the calculated Irms value.
 */
double CurrentMonitor::getPower(const uint8_t channel)
{

  if(channel < 0 || channel > maxClampCount - 1){
    return -1;
  }
  calculate(channel);
  return iRMS[channel] * vRMS;
}

double CurrentMonitor::getIrms(const uint8_t channel, const double calibration)
{
  if(channel < 0 || channel > maxClampCount - 1){
    return -1;
  }
  Ical = calibration;
  calculateIrms(channel);
  return iRMS[channel];
}

/**
 * Private: Receives analog reading from given channel and calculates Irms.
 * The Irms is calculated from a numberOfSamples to receive a more accurate number
 */
void CurrentMonitor::calculate(const uint8_t channel)
{
  double accumulator = 0.0;
  double voltage, iPrimary;

    // Take a number of samples and calculate RMS current
  for (unsigned int i = 0; i < numSamples; i++){

    // Read ADC, convert to voltage, remove offset
    sample = analogRead(hardwareLocation[channel]);
    voltage = (sample * 3.3) / 4096; // 3.3 voltage from board. 4096 is 2^12 resolution
    voltage = voltage - offset;

    // calculate the sensed current
    iPrimary = (voltage / rBurden) * numTurns;

    // Square current and add to accumulator
    accumulator += pow(iPrimary, 2);
  }

  // Calculate RMS from accumulated values
  iRMS[channel] = sqrt(accumulator / numSamples);

  return;
}

void CurrentMonitor::calculateIrms(const uint8_t channel)
{
  double sum = 0; 
  double filtered;
  unsigned int sample;

  for (unsigned int n = 0; n < numSamples; n++)
  {
    sample = analogRead(hardwareLocation[channel]);

    // Digital low pass filter extracts the 2.5 V or 1.65 V dc offset,
    //  then subtract this - signal is now centered on 0 counts.
    offset = (offset + (sample-offset)/1024);
    filtered = sample - offset;

    // Root-mean-square method current
    // 1) square current values
    double squared = filtered * filtered;
    // 2) sum
    sum += squared;
  }

  double I_RATIO = Ical *((vcc/1000.0) / (ADC_COUNTS));
  iRMS[channel] = I_RATIO * sqrt(sum / numSamples);

  return;
}