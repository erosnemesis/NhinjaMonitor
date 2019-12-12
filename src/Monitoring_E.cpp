/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "application.h"
#line 1 "c:/Users/erosn/ownCloud/ParticleProjects/Monitoring_E/src/Monitoring_E.ino"
// Product: E_Series
// Product Name: Nhinja 2020 (Proto)
void setup();
void loop();
int setAlarmCount(String count);
int setRelayCount(String count);
int setClampCount(String count);
void updateAmpVariables();
#line 3 "c:/Users/erosn/ownCloud/ParticleProjects/Monitoring_E/src/Monitoring_E.ino"
PRODUCT_ID(10352);

PRODUCT_VERSION(16);

/*
 * Project Nhinja Monitoring
 * Description: Dryer Monitoring IOT project for telecommunication air dryers
 * Author: Robert Bachta @ PacTel Solutions
 * Date: 2019
 */

#include "CurrentMonitor.h"
#include "AlarmDetector.h"
#include "RemoteAlarmReset.h"

#define PUBLISH_NAME_ALARM "Dryer_Alarms" // DO NOT MODIFY THIS. GOOGLE CLOUD PUBSUB DEPENDANCY
#define PUBLISH_NAME_MSG "Messages" // DO NOT MODIFY THIS. GOOGLE CLOUD PUBSUB DEPENDANCY

struct MonitorSettings{
  uint8_t version;
  uint8_t alarms, relays, clamps;
  double calibration;
} settings;

const uint8_t ampCount = 8;
double AMP_READING[ampCount];
double signalStrength;

Timer timer(2000, updateAmpVariables);
AlarmDetector alarmDetector;
RemoteAlarmReset remoteAlarmReset;

CurrentMonitor monitor(3300, 1480);
double cMonCalibration = 111.1;

//function declarations
void processAlarm(int alarmNum, bool alarmState);
int alarmReset(String alarmNum);
int setAlarmCount(String alarmCount);
int setClampCount(String clampCount);
int setCalibration(String num);
void updateAmpVariable();

void setup() {

  // Get saved settings from EEPROM
  EEPROM.get(0, settings);

  // Check if EEPROM data exists
  if(settings.version != 0){
    settings.version = 0;
    settings.alarms = alarmDetector.getDefaultAlarmCount();
    settings.relays = remoteAlarmReset.getDefaultRelayCount();
    settings.clamps = ampCount;
    settings.calibration = cMonCalibration;

    EEPROM.put(0, settings);
  }

  // Setup Particle Functions
  Particle.function("Reset_Dryer", alarmReset);
  Particle.function("Alarm_Count", setAlarmCount);
  Particle.function("Clamp_Count", setClampCount);
  Particle.function("Relay_Count", setRelayCount);
  Particle.function("Change_Calibration", setCalibration);

  // Setup Particle Variables
  Particle.variable("Signal_Strength", &signalStrength, DOUBLE);
  for(int i = 0; i < settings.clamps; i++){
    Particle.variable(String("Amp_" + String(i)), &AMP_READING[i], DOUBLE);
  }
  Particle.variable("Amp_Calibration", &settings.calibration, DOUBLE);

  // Timer for Amp Clamp ADC updates
  timer.start();

}

void loop() {

  for(int i = 0; i < settings.alarms; i++){
    // critical
    noInterrupts();
    bool pinState = alarmDetector.getAlarmState(i);
    interrupts();
    // critical
    processAlarm(i, pinState);
  }

  CellularSignal sig = Cellular.RSSI();
  signalStrength = sig.getStrength();

}

/**
 * This is executed each loop() to continually check the state
 * of air dryer alarms. Any changes in alarm state get published
 * as an event to Particle.io.
 */
void processAlarm(int alarmNum, bool alarmState){
  
  if(alarmState){
    // Only publish the in alarm state if the previous state was not in alarm
    if(!alarmDetector.getPreviousAlarmState(alarmNum)){
      String alarmStr = String("Dryer ") + String(alarmNum+1) + String(" in Alarm");
      Particle.publish(PUBLISH_NAME_ALARM, alarmStr, PRIVATE);
    }
    alarmDetector.setPreviousAlarmState(alarmNum, true);
    return;
  }

  // Only publish the alarm reset state if the preivous state was in alarm
  if(alarmDetector.getPreviousAlarmState(alarmNum)){
    String resetStr = String("Dryer Alarm " + String(alarmNum+1) + " Reset");
    Particle.publish(PUBLISH_NAME_ALARM, resetStr, PRIVATE);
  }
  alarmDetector.setPreviousAlarmState(alarmNum, false);
}

/**
 * This is an exposed method to Particle.io to trigger the
 * alarm Reset relay. Any time the remote reset is triggered
 * the setAlarm 
 */
int alarmReset(String alarmNum){
  int resetNum = alarmNum.toInt();
  if(resetNum < 1 || resetNum > settings.relays){
    return 0;
  }

  remoteAlarmReset.process(resetNum);

  Particle.publish(PUBLISH_NAME_ALARM, String("Remote Dryer Alarm " + alarmNum + " Reset Sent"), PRIVATE);

  return 1;
}

/**
 * This is an exposed method to Particle.io to set the amount
 * of installed Air Dryer Alarms. The settings is saved to 
 * EEPROM.
 * param count: Number of installed Alarms 1 - 4
 */
int setAlarmCount(String count){
  int aCount = count.toInt();
  if(aCount < 1 || aCount > alarmDetector.getDefaultAlarmCount()){
    return 0;
  }

  settings.alarms = aCount;
  EEPROM.put(0, settings);

  Particle.publish(PUBLISH_NAME_MSG, String("Alarm Count has been updated to " + String(aCount)), PRIVATE);

  return 1;
}

/**
 * This is an exposed method to Particle.io to set the amount
 * of installed reset Relays. The setting is saved to EEPROM.
 * param count: Number of installed Relays 0 - 4
 */
int setRelayCount(String count){
  int rCount = count.toInt();
  if(rCount < 0 || rCount > remoteAlarmReset.getDefaultRelayCount()){
    return 0;
  }

  settings.relays = rCount;
  EEPROM.put(0, settings);

  Particle.publish(PUBLISH_NAME_MSG, String("Relay Count has been updated to " + String(rCount)), PRIVATE);

  return 1;
}

/**
 * This is an exposed method to Particle.io to set the amount
 * of AMP clamps that are installed on the monitor. The setting
 * is saved to EEPROM.
 * param count: Number of AMP clamps 1 - 8
 */
int setClampCount(String count){
  int cCount = count.toInt();
  if(cCount < 1 || cCount > ampCount){
    return 0;
  }

  settings.clamps = cCount;
  EEPROM.put(0, settings);

  Particle.publish(PUBLISH_NAME_MSG, String("Amp Clamp Count has been updated to " + String(cCount)), PRIVATE);

  return 1;
}

/**
 * This is an exposed method to Particle.io to set the calibration
 * for the AMP clamps. The setting is saved to EEPROM.
 * param num: Calibration value
 */
int setCalibration(String num)
{
  double calibration = num.toFloat();
  if(calibration < 1){
    return 0;
  }
  
  settings.calibration = calibration;
  EEPROM.put(0, settings);

  Particle.publish(PUBLISH_NAME_MSG, String("Calibration has been updated to " + String(calibration)), PRIVATE);

  return 1;
}

/**
 * This method retrieves the Amperage reading on each enabled
 * AMP terminal.
 * The globally declared timer is set to trigger this function
 * at a set interval. Depending on the settings, the AMP clamp
 * readings get updated.
 */
void updateAmpVariables(){
  for(int i = 0; i < settings.clamps; i++){
    double iRMS = monitor.getIrms(i, settings.calibration);
    AMP_READING[i] = iRMS;
  }
}