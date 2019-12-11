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

PRODUCT_VERSION(12);

/*
 * Project Nhinja Monitoring
 * Description: Dryer Monitoring IOT project for telecommunication air dryers
 * Author: Robert Bachta @ PacTel Solutions
 * Date: 2019
 */

#include "CurrentMonitor.h"

#define MONITOR_DEBUG false
#define PUBLISH_NAME "Dryer_Alarms" // DO NOT MODIFY THIS. GOOGLE CLOUD PUBSUB DEPENDS ON THIS

struct MonitorSettings{
  uint8_t version;
  uint8_t alarms, relays, clamps;
  double calibration;
} settings;

const uint8_t relayCount = 4;
const uint16_t RELAY_DELAY = 500;
const uint8_t alarmCount = 4;
const uint8_t ampCount = 8;
unsigned long debounceTime = millis();
const uint8_t DEBOUNCE_DELAY = 200;
const uint16_t rPins[relayCount] = {D0, D1, D2, D3};
const uint8_t ALARM[alarmCount] = {C0, C1, C2, C3};

double AMP_READING[ampCount];

byte alarmState[alarmCount] = {0, 0, 0, 0};
double signalStrength;

//function declarations
void setAlarm(bool inAlarm, int alarmNum);
int alarmReset(String alarmNum);
int setAlarmCount(String alarmCount);
int setClampCount(String clampCount);
int setCalibration(String num);
void updateAmpVariable();

Timer timer(2000, updateAmpVariables);

CurrentMonitor monitor(3300, 1480);
double cMonCalibration = 111.1;

void setup() {

  #if MONITOR_DEBUG
    Serial.begin();
  #endif

  // Get saved settings from EEPROM
  EEPROM.get(0, settings);

  if(settings.version != 0){
    settings.version = 0;
    settings.alarms = alarmCount;
    settings.relays = relayCount;
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

  // Timer for Amp Clamp ADC updates
  timer.start();

  for(int i = 0; i < alarmCount; i++){
    pinMode(ALARM[i], INPUT);
  }

  for(int i = 0; i < relayCount; i++){
    pinMode(rPins[i], OUTPUT);
    digitalWrite(rPins[i], HIGH);
  }

  Particle.variable("Signal_Strength", &signalStrength, DOUBLE);

  for(int i = 0; i < settings.clamps; i++){
    Particle.variable(String("Amp_" + String(i)), &AMP_READING[i], DOUBLE);
  }

}

void loop() {

  for(int i = 0; i < settings.alarms; i++){
    byte pinStatus = digitalRead(ALARM[i]);
    pinStatus == HIGH ? setAlarm(true, i) : setAlarm(false, i);
    
    #if MONITOR_DEBUG
    Serial.print(String(i) + " Value: " + String(pinStatus));
    Serial.print(" ");
    #endif
  }
  #if MONITOR_DEBUG
  Serial.println();
  #endif

  CellularSignal sig = Cellular.RSSI();
  signalStrength = sig.getStrength();

}

int alarmReset(String alarmNum){
  int alarm = alarmNum.toInt();
  if(alarm < 1 || alarm > settings.relays){
    return 0;
  }

  digitalWrite(rPins[alarm-1], LOW);
  long resetDelay = millis();
  while(millis() - resetDelay < RELAY_DELAY){
    //wait (this is here instead to delay() so interrupts can occur)
  }
  digitalWrite(rPins[alarm-1], HIGH);

  Particle.publish(PUBLISH_NAME, String("Remote Dryer Alarm " + alarmNum + " Reset Sent"), PRIVATE);

  #if MONITOR_DEBUG
  Serial.println("Remote Dryer Alarm " + alarmNum + " Reset Sent");
  #endif
  return 1;
}

void setAlarm(bool inAlarm, int alarmNum){
  
  if(inAlarm){
    String alarmStr = String("Dryer ") + String(alarmNum+1) + String(" in Alarm");

    if(((millis() - debounceTime) > DEBOUNCE_DELAY) && alarmState[alarmNum] == 0){
      Particle.publish(PUBLISH_NAME, alarmStr, PRIVATE);
      debounceTime = millis();
      #if MONITOR_DEBUG
      Serial.println(alarmStr);
      #endif
    }
    alarmState[alarmNum] = 1;
    return;
  }

  if(((millis() - debounceTime) > DEBOUNCE_DELAY) && alarmState[alarmNum] == 1){
    String resetStr = String("Dryer Alarm " + String(alarmNum+1) + " Reset");
    Particle.publish(PUBLISH_NAME, resetStr, PRIVATE);
    debounceTime = millis();
    #if MONITOR_DEBUG
    Serial.println(resetStr);
    #endif
  }
  alarmState[alarmNum] = 0;
}

int setAlarmCount(String count){
  int aCount = count.toInt();
  if(aCount < 1 || aCount > alarmCount){
    return 0;
  }

  settings.alarms = aCount;

  EEPROM.put(0, settings);

  Particle.publish(PUBLISH_NAME, String("Alarm Count has been updated to " + String(aCount)), PRIVATE);

  return 1;
}

int setRelayCount(String count){
  int rCount = count.toInt();
  if(rCount < 0 || rCount > relayCount){
    return 0;
  }

  settings.relays = rCount;

  EEPROM.put(0, settings);

  Particle.publish(PUBLISH_NAME, String("Relay Count has been updated to " + String(rCount)), PRIVATE);

  return 1;
}

int setClampCount(String count){
  int cCount = count.toInt();
  if(cCount < 1 || cCount > ampCount){
    return 0;
  }

  settings.clamps = cCount;

  EEPROM.put(0, settings);

  Particle.publish(PUBLISH_NAME, String("Amp Clamp Count has been updated to " + String(cCount)), PRIVATE);

  return 1;
}

int setCalibration(String num)
{
  double calibration = num.toFloat();
  if(calibration < 1){
    return 0;
  }
  
  settings.calibration = calibration;

  EEPROM.put(0, settings);

  Particle.publish(PUBLISH_NAME, String("Calibration has been updated to " + String(calibration)), PRIVATE);

  return 1;
}

void updateAmpVariables(){
  for(int i = 0; i < settings.clamps; i++){
    double iRMS = monitor.getIrms(i, settings.calibration);
    AMP_READING[i] = iRMS;
  }

  #if MONITOR_DEBUG
  for(int i = 0; i < settings.clamps; i++){
    Serial.print(String("Amp_") + String(i) + String(" "));
    Serial.print(AMP_READING[i]);
    Serial.print(" ");
  }
  Serial.println();
  #endif
}