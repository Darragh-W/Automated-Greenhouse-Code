


#include <DHT.h>
#include <DHT_U.h>

#include <Wire.h>
#include "RTClib.h"


#include <string.h>

#include "constants.h"
#include "pins.h"
#include "variables.h"
#include "sensor.h"
#include "functions.h"


#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include <SPI.h>
#include <SD.h>

#include <SoftwareSerial.h>

#define DHTPIN 7  // DHT connects to a digital pin
#define DHTTYPE DHT22

DHT dht(DHTPIN,DHTTYPE);  // create object dht of class DHT
SoftwareSerial apcSerial(10, 11); // RX=10, TX=11

RTC_DS3231 rtc;



void setup() {

  MCUSR = 0;  
  wdt_disable();

  Serial.begin(BAUD); // begins serial communication at 9600 bits per second
  apcSerial.begin(BAUD);

  analogReference(INTERNAL);
  // discard first few readings due to spike
  analogRead(A0);
  delay(10);


  dht.begin();  // initialise sensor

  Wire.begin();
  rtc.begin();

  pinMode(fanRelayPin, OUTPUT);
  pinMode(pumpRelayPin, OUTPUT);
  pinMode(apcSleepPin, OUTPUT);


  digitalWrite(fanRelayPin, LOW);
  digitalWrite(pumpRelayPin, LOW);
  digitalWrite(apcSleepPin, LOW);

  Serial.println(F("Temperature (C),Humidity (%RH),VPD (kPa),Soil Moisture (%),Fan Status,Pump Status"));
  apcSerial.println(F("Temperature (C),Humidity (%RH),VPD (kPa),Soil Moisture (%),Fan Status,Pump Status"));
}





void loop() {

  sensorDHT.measure.temperature = dht.readTemperature();  // reads temperature value from sensor in C
  sensorDHT.measure.humidity = dht.readHumidity(); // reads humidity value from sensor in %RH

  sensorDHT.measure.humidity = humidityCalibrationEquation(sensorDHT.measure.humidity, humidityOffset);
  sensorDHT.measure.temperature = temperatureCalibrationEquation(sensorDHT.measure.temperature, temperatureOffset);

  sensorSEN.measure.soilMoisture_mV = analogRead(A0); // sets value for soil moisture sensor
  

  //Converts soil moisture (mV) into soil moisture (%)
  sensorSEN.measure.soilMoisture_percentage = soilMoistureCalibrationEquation(sensorSEN.measure.soilMoisture_mV);

  if (sensorSEN.measure.soilMoisture_percentage > 100){
    sensorSEN.measure.soilMoisture_percentage = 100;
  }


  // for first step so averages dont equal zero
  if (!sensorDHT.measure.initialised){
    sensorDHT.measure.averageTemperature = sensorDHT.measure.temperature;
    sensorDHT.measure.averageHumidity = sensorDHT.measure.humidity;
    sensorDHT.measure.initialised = true;
  }

  if (!sensorSEN.measure.initialised){
    sensorSEN.measure.averageSoilMoisture_percentage = sensorSEN.measure.soilMoisture_percentage;
    sensorSEN.measure.initialised = true;
  }



  sensorDHT.measure.averageTemperature = calcAverage(
    sensorDHT.measure.temperature,
    sensorDHT.measure.averageTemperature,
    temperatureAlpha
  );

  sensorDHT.measure.averageHumidity = calcAverage(
    sensorDHT.measure.humidity,
    sensorDHT.measure.averageHumidity,
    humidityAlpha
  );

  sensorSEN.measure.averageSoilMoisture_percentage = calcAverage(
    sensorSEN.measure.soilMoisture_percentage,
    sensorSEN.measure.averageSoilMoisture_percentage,
    soilMoistureAlpha
  );

  fan.VPD = calcVPD(sensorDHT.measure.averageTemperature, sensorDHT.measure.averageHumidity);


  if (fan.VPD < .4 || fan.VPD > 1.3){

    fan.fanOn = 1;

  }

  else if (fan.VPD > .65 && fan.VPD < 1.05){

    fan.fanOn = 0;

  }




  if (sensorSEN.measure.averageSoilMoisture_percentage < 70){

    digitalWrite(pumpRelayPin, 1);
    delay(500);
    digitalWrite(pumpRelayPin, 0);
    
    pump.pumpOn = 1;

  }

  else{
    pump.pumpOn = 0;
  }



  digitalWrite(fanRelayPin, fan.fanOn);
  

  serialPrint(
    sensorDHT.measure.averageTemperature,
    sensorDHT.measure.averageHumidity,
    fan.VPD,
    sensorSEN.measure.averageSoilMoisture_percentage,
    fan.fanOn,
    pump.pumpOn
  );


  apcPrint(
    sensorDHT.measure.averageTemperature,
    sensorDHT.measure.averageHumidity,
    fan.VPD,
    sensorSEN.measure.averageSoilMoisture_percentage,
    fan.fanOn,
    pump.pumpOn    
  );
  
  //delay(30000);
  sleepForTicks(sleepTicks_30); 
  
}
