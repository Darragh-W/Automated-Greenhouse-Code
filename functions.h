
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <Arduino.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include "constants.h"
#include "variables.h"
#include "sensor.h"
#include "pins.h"
#include <SoftwareSerial.h>

extern SoftwareSerial apcSerial;
float soilMoistureCalibrationEquation(int soilMoistureValue_mV);
float humidityCalibrationEquation(float measurement, float offset);
float temperatureCalibrationEquation(float measurement, float offset);
float calcAverage(float measurement, float previousAverage, float alpha); //EMA
float calcVPD(float temperature, float humidity);
void sleepForTicks(uint8_t ticks);
void serialPrint(float temperature, float humidity, float VPD, float soilMoisture_percentage, float fan, float pump);
void apcPrint(float temperature, float humidity, float VPD, float soilMoisture_percentage, float fan, float pump);


#endif
