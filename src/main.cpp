/*
 * test-veml6075.ino
 *
 * Simpel sketch to test if a given board words.
 *
 * Author: Sean Caulfield <sean@yak.net>
 * License: GPLv2.0
 *
 */


#include <VEML6075.h>
#include <M5StickC.h>

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

VEML6075 veml6075 = VEML6075();
bool found = false;


void setup() {
  delay(1000);
  M5.begin();
  Wire.begin(0,26);
  Serial.begin(9600);

//OLED setup and startup display
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(10, 20);
  M5.Lcd.println("IR temp");
  M5.Lcd.setCursor(10, 30);
  M5.Lcd.setTextSize(1);
    if (!veml6075.begin()) {
    // Serial.println(F("VEML6075 not found!"));
    M5.Lcd.println("VEML6075 not found!");
  } else {
    found = true;
  }
  M5.Lcd.setCursor(70, 70);
  M5.Lcd.print("YR-Design ");
  M5.Lcd.setTextColor(ORANGE);
  M5.Lcd.print("2020");


  delay(2000);
  //clear the screen
    M5.Lcd.fillRect(0,0,160,80,BLACK);
    M5.Lcd.setTextColor(WHITE);
}

void loop() {

  if (found) {
    float value;
  M5.Lcd.setTextSize(2);
    // Poll sensor
    veml6075.poll();

    M5.Lcd.fillRect(0,0,160,80,BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextColor(WHITE);


// time print
    Serial.print(F("t = "));
    Serial.println(millis());

//UVA print
    value = veml6075.getUVA();
    Serial.print(F("UVA = "));
    Serial.println(value, 2);

    M5.Lcd.print(F("UVA ="));
    M5.Lcd.println(value);
    // M5.Lcd.print("C");

//UVB print
    value = veml6075.getUVB();
    Serial.print(F("UVB = "));
    Serial.println(value, 2);

    M5.Lcd.print(F("UVB ="));
    M5.Lcd.println(value);

//UV index print
    value = veml6075.getUVIndex();
    Serial.print(F("UV Index ="));
    Serial.println(value, 1);

    M5.Lcd.print(F("UVIND ="));
    M5.Lcd.println(value);

//Device ID print
    uint16_t devid = veml6075.getDevID();
    Serial.print(F("ID ="));
    Serial.println(devid, HEX);

    Serial.println(F("----------------"));

//copy right print
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(60, 70);
    M5.Lcd.print("YR-Design ");
    M5.Lcd.setTextColor(ORANGE);
    M5.Lcd.print("2020");
    M5.Lcd.setTextSize(2);
  }
    delay(1000);
}