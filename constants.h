
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Arduino.h>

struct CalibrationSEN{
  static constexpr float slope = -0.649f;
  static constexpr float yAxis = 330.98f;
};

//single point humidity calibration
constexpr float humidityOffset = 17.2;

//single point temperature calibration
constexpr float temperatureOffset = -0.3;

//Alphas for EMA
constexpr float temperatureAlpha = 0.33;

constexpr float humidityAlpha = 0.26;

constexpr float soilMoistureAlpha = 0.18;


//Set the Baud rate
constexpr int BAUD = 9600;


constexpr int sleepTicks_30 = 27;




#endif