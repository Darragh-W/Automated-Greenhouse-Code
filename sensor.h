
#ifndef SENSOR_H
#define SENSOR_H


#include <Arduino.h>
#include "constants.h"
#include "variables.h"



struct DHT22Measurements{
  float temperature = 0.0;
  float humidity = 0.0;
  float averageTemperature = 0.0;
  float averageHumidity = 0.0;
  bool initialised = false;
};

struct SEN0193Measurements{
  int soilMoisture_mV = 0;
  float soilMoisture_percentage = 0.0;
  float averageSoilMoisture_percentage = 0.0;
  bool initialised = false;
};

struct SensorSEN{
  SEN0193Measurements measure;
  CalibrationSEN calibrate;
};

struct SensorDHT{
  DHT22Measurements measure;
  uint16_t temperatureNanCounter = 0;
  uint16_t humidityNanCounter = 0;
};

extern SensorSEN sensorSEN;
extern SensorDHT sensorDHT;



#endif