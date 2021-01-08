//**************************************************// 
//    Display for UV meter VEML6075  on M5StickC    //
//                    V1.0.1                        //
//   written by Rob Oudendijk                       //
//     Contact email: rob@yr-design.biz             //
//          Copyright (c) 2020, YR-Design           //        
//              All rights reserved.                //
//**************************************************//
// M5Stck info on https://github.com/m5stack/M5StickC/blob/master/README.md
// M5Stick-C SCL=G26 SDA=G0
// 2020-06-18 V1.0.1 Added code for BME280
// 2020-07-25 V1.0.2 Added Max values

#include <VEML6075.h>
#include <M5StickC.h>
#include <BME280I2C.h>

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

VEML6075 veml6075 = VEML6075();
bool found = false;
//Setup max values 
float UVA_Max=0;
float UVB_Max=0;
float UVIN_Max=0;
float TEMP_Max=0;
float HUM_Max=0;
float Volage=0;

String version="1.0.4";

//setup BME280
BME280I2C::Settings settings(
   BME280::OSR_X1,
   BME280::OSR_X1,
   BME280::OSR_X1,
   BME280::Mode_Forced,
   BME280::StandbyTime_1000ms,
   BME280::Filter_Off,
   BME280::SpiEnable_False,
   BME280I2C::I2CAddr_0x76 // I2C address. I2C specific.
);

BME280I2C bme(settings);


void printBME280Data
(
   Stream* client
)
{
   float temp(NAN), hum(NAN), pres(NAN);

   BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
   BME280::PresUnit presUnit(BME280::PresUnit_Pa);

   bme.read(pres, temp, hum, tempUnit, presUnit);

   client->print("Temp: ");
   client->print(temp);
       if (temp >TEMP_Max) {
      TEMP_Max = temp;
    }
      M5.Lcd.setTextSize(1);
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.setCursor(0, 50);
      M5.Lcd.print("Temp=");
      M5.Lcd.print(temp);
      M5.Lcd.setCursor(0, 60);
      M5.Lcd.print("max =");
      M5.Lcd.print(TEMP_Max);
   client->print("°"+ String(tempUnit == BME280::TempUnit_Celsius ? 'C' :'F'));
   client->print("\t\tHumidity: ");
   client->print(hum);
          if (hum >HUM_Max) {
      HUM_Max = hum;
    }
      M5.Lcd.setCursor(75, 50);
      M5.Lcd.print("Hum=");
      M5.Lcd.print(hum);
      M5.Lcd.setCursor(75, 60);
      M5.Lcd.print("max=");
      M5.Lcd.print(HUM_Max);
   client->print("% RH");
   client->print("\t\tPressure: ");
   client->print(pres);
   client->println("Pa");

   delay(1000);
}

void setup() 
{
  delay(1000);

//setup BME280
  Wire.begin(0,26);
  while(!bme.begin())
  {
    Serial.println("Could not find BME280I2C sensor!");
    delay(1000);
  }

  switch(bme.chipModel())
  {
     case BME280::ChipModel_BME280:
       Serial.println("Found BME280 sensor! Success.");
       break;
     case BME280::ChipModel_BMP280:
       Serial.println("Found BMP280 sensor! No Humidity available.");
       break;
     default:
       Serial.println("Found UNKNOWN sensor! Error!");
  }

// Change some settings before using BME280
settings.tempOSR = BME280::OSR_X4;

bme.setSettings(settings);

// start serials
  M5.begin();
  Wire.begin(0,26);
  Serial.begin(9600);
  
//setup pins
  // pinMode(10, OUTPUT);//led
  pinMode(37, INPUT);//button

//OLED setup and startup display
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(10, 20);
  M5.Lcd.print("UV/Temp/Hum meter");
  M5.Lcd.setCursor(10, 40);
  M5.Lcd.setTextSize(1);
  M5.Lcd.print("Version ");
  M5.Lcd.print(version);
  M5.Lcd.setCursor(10, 30);
  M5.Lcd.setTextSize(1);
    if (!veml6075.begin()) {
    M5.Lcd.println("VEML6075 not found!");
  } else {
    found = true;
  }

  M5.Lcd.setCursor(70, 70);
  M5.Lcd.print("YR-Design ");
  M5.Lcd.setTextColor(ORANGE);
  M5.Lcd.print("2020");
  //switch LED on 
  digitalWrite(10, LOW);

  delay(2000);
  //clear the screen
    M5.Lcd.fillRect(0,0,160,80,BLACK);
    M5.Lcd.setTextColor(WHITE);

  //switch LED off
    digitalWrite(10, LOW);
}

void loop() {
    if (M5.BtnA.wasReleasefor(2000)) { 
      
    }

  if (found) {
    float value;
  M5.Lcd.setTextSize(1);
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
    if (value >UVA_Max) {
      UVA_Max = value;
    }
    Serial.print(F("UVA = "));
    Serial.println(value, 2);
    Serial.print(F("UVAMax = "));
    Serial.println(UVA_Max, 2);

    M5.Lcd.print(F("UVA ="));
    M5.Lcd.println(value);
    M5.Lcd.print(F("max ="));
    M5.Lcd.println(UVB_Max);
    // M5.Lcd.print("C");

//UVB print
    value = veml6075.getUVB();
    if (value >UVB_Max) {
      UVB_Max = value;
    }
    Serial.print(F("UVB = "));
    Serial.println(value, 2);
    Serial.print(F("UVBMax = "));
    Serial.println(UVB_Max, 2);


    M5.Lcd.print(F("UVB ="));
    M5.Lcd.println(value);
    M5.Lcd.print(F("max ="));
    M5.Lcd.println(UVB_Max);

//UV index print
    value = veml6075.getUVIndex();
        if (value >UVIN_Max) {
      UVIN_Max = value;
    }
    Serial.print(F("UVIN="));
    Serial.println(value, 1);
    Serial.print(F("UVINMax = "));
    Serial.println(UVIN_Max, 2);

    M5.Lcd.print(F("UVIN="));
    M5.Lcd.println(value);
    M5.Lcd.print(F("max ="));
    M5.Lcd.println(UVIN_Max);

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

  // serial rpint BME data.
     printBME280Data(&Serial);

  //wait
    delay(1000);
}

