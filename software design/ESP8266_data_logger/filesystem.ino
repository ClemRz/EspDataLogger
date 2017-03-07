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

void saveData(void) {
  File file = SPIFFS.open(FILE_PATH, "a");
  if (!file) {
#if DEBUG
    Serial.println(F("file open failed"));
#endif  //DEBUG
    return;
  }
  file.println(millis()); file.print(F(",")); file.print(_celsiusHundredths); file.print(F(",")); file.println(_humidityPercent);
  file.close();
}

void saveHeaders(void) {
  File file = SPIFFS.open(FILE_PATH, "a");
  if (!file) {
#if DEBUG
    Serial.println(F("file open failed"));
#endif  //DEBUG
    return;
  }
  file.println(compile_date); file.print(F(", rate: ")); file.print(WAKEUP_RATE); file.print(F(", version: ")); file.println(VERSION);
  file.close();
}

void retrieveData(void) {
  File file = SPIFFS.open(FILE_PATH, "r");
  if (!file) {
    Serial.println(F("file open failed"));
    return;
  }
  Serial.println(F("-- bof --"));
  while(file.available()) {
    Serial.println(file.readStringUntil('\n'));
  }
  Serial.println(F("-- eof --"));
}

void deleteData(void) {
  bool ok = SPIFFS.remove(FILE_PATH);
  if (ok) {
    Serial.print(FILE_PATH); Serial.println(F(" deleted"));
    saveHeaders();
  } else {
    Serial.println(F("file delete failed"));
  }
}

