/*
    Copyright (C) 2016 Clément Ronzon

    This file is part of EspDataLogger.

    SpotPipe is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with EspDataLogger.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <ESP8266WiFi.h>
#include <Wire.h>
#include <SI7021.h>
#include "FS.h"
#include "structures.h"

#define MICROSEC              1000000L
#define MILLISEC              1000L
#define SEC                   1L
#define MINUTE                (unsigned int) 60L*SEC
#define HOUR                  (unsigned int) 60L*MINUTE
#define DAY                   (unsigned long) 24L*HOUR

#define SDA                   13
#define SCL                   12

#define WAKEUP_RATE           10*MINUTE
#define FILE_PATH             "/data.csv"

#define DEBUG                 0        
#define RETRIEVE              0
#define DELETE                1

#define VERSION               "1.0"

const char compile_date[] = __DATE__ " " __TIME__;

SI7021 _sensor;

bool _sensorExists = false;
     
int _celsiusHundredths,
    _humidityPercent;


void setup() {
  WiFi.mode(WIFI_OFF);
#if DEBUG || RETRIEVE || DELETE
  initSerial();
#endif  // DEBUG || RETRIEVE || DELETE
  initSI7021();
  initFS();
#if RETRIEVE
  delay(5 * SEC);
  retrieveData();
#endif //RETRIEVE
#if DELETE
  delay(5 * SEC);
  deleteData();
#else
  readSensor();
  saveData();
  sleep();
#endif  // DELETE
#if RETRIEVE || DELETE
  quickSleep();
#endif  // RETRIEVE || DELETE
}

void loop() {
}

void sleep(void) {
#if DEBUG
  Serial.print(F("Go to sleep for "));
  Serial.print(WAKEUP_RATE * MICROSEC);
  Serial.println(F("us."));
#endif  //DEBUG
  ESP.deepSleep(WAKEUP_RATE * MICROSEC);
}

void quickSleep(void) {
  Serial.print(F("Go to sleep for "));
  Serial.print(1 * MICROSEC);
  Serial.println(F("us."));
  ESP.deepSleep(1 * MICROSEC);
}

