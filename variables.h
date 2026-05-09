
#ifndef VARIABLES_H
#define VARIABLES_H

#include <Arduino.h>
#include "constants.h"
#include <SPI.h>
#include <SD.h>

struct FanControl{
  bool fanOn = 0;
  float VPD = 0;

};

struct PumpControl{
  bool pumpOn = 0;

};


extern FanControl fan;
extern PumpControl pump;






#endif 