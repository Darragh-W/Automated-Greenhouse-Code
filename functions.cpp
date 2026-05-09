
#include "functions.h"





float soilMoistureCalibrationEquation(int soilMoistureValue_mV){

  float soilMoistureValue_percentage = sensorSEN.calibrate.slope * soilMoistureValue_mV + sensorSEN.calibrate.yAxis;

  return soilMoistureValue_percentage;
}

float temperatureCalibrationEquation(float measurement, float offset){
  return measurement + offset;
}

float humidityCalibrationEquation(float measurement, float offset){
  return measurement + offset;
}



float calcAverage(float measurement, float previousAverage, float alpha){
  float minusAlpha = 1 - alpha;
  float newAverage = (alpha * measurement) + (minusAlpha * previousAverage);
  return newAverage;
}

float calcVPD(float temperature, float humidity){
  float SVP = 0.6108 * exp((17.27 * temperature) / (temperature + 237.3));
  float AVP = SVP * (humidity / 100);

  float VPD = SVP - AVP;
  return VPD;
}

// Watchdog interrupt service routine
ISR(WDT_vect) {
  // Nothing needed here; the interrupt wakes the MCU
}


void sleepForTicks(uint8_t ticks){
  
  for (uint8_t i = 0; i < ticks; i++) {

    cli();
    wdt_reset();
    MCUSR = 0;
    WDTCSR |= (1<<WDCE) | (1<<WDE);
    WDTCSR = 0;
    sei();

    cli();
    wdt_reset();
    WDTCSR |= (1<<WDCE) | (1<<WDE);
    WDTCSR = (1<<WDIE) | (1<<WDP2) | (1<<WDP1);  // ~2s ticks
    sei();

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_mode();
    sleep_disable();
  }
  wdt_disable();
}


void serialPrint(float temperature, float humidity,float VPD, float soilMoisture_percentage, float fan, float pump){
  Serial.print(temperature);
  Serial.print(F(","));
  Serial.print(humidity);
  Serial.print(F(","));
  Serial.print(VPD);
  Serial.print(F(","));
  Serial.print(soilMoisture_percentage);

  Serial.print(F(","));
  Serial.print(fan ? F("ON") : F("OFF"));

  Serial.print(F(","));
  Serial.println(pump ? F("ON") : F("OFF"));

  Serial.flush();

  return;
}

void apcPrint(float temperature, float humidity,float VPD, float soilMoisture_percentage, float fan, float pump){
  digitalWrite(apcSleepPin, HIGH);
  delay(500);

  apcSerial.print(temperature);
  apcSerial.print(F(","));
  apcSerial.print(humidity);
  apcSerial.print(F(","));
  apcSerial.print(VPD);
  apcSerial.print(F(","));
  apcSerial.print(soilMoisture_percentage);

  apcSerial.print(F(","));
  apcSerial.print(fan ? F("ON") : F("OFF"));

  apcSerial.print(F(","));
  apcSerial.println(pump ? F("ON") : F("OFF"));

  apcSerial.flush();

  delay(100);

  digitalWrite(apcSleepPin, LOW);


  return;
}
