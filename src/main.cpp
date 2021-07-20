#include <Arduino.h>
#include <Wire.h>
#include <PolledTimeout.h>

#define SDA_PIN 4
#define SCL_PIN 5
const int16_t I2C_MASTER = 0x42;
const int16_t I2C_SLAVE = 0x08;
int runNumber = 1;

void setup() {
  Serial.begin(9600);  // start serial for output
  Wire.begin(SDA_PIN, SCL_PIN, I2C_MASTER);        // join i2c bus (address optional for master)
  Wire.setClock(20000);
}

void loop() {
  delay(3000);
  
  if (runNumber == 1)
  {
    Wire.beginTransmission(I2C_SLAVE);  // transmit to device #8
    Wire.write("reqCNT\n");             // sends five bytes
    Wire.endTransmission();             // stop transmitting
    delay(100);
    Wire.requestFrom(I2C_SLAVE, 2);     // Counter of Measurements
    while (Wire.available()) { 
      char c = Wire.read(); 
      Serial.print(c);         
    }
    Serial.println("");
    runNumber++;
  } 
  delay(3000);
  if (runNumber == 2)
  {
    Wire.beginTransmission(I2C_SLAVE);  // transmit to device #8
    Wire.write("reqFEA\n");             // sends five bytes
    Wire.endTransmission();             // stop transmitting
    delay(100);
    Wire.requestFrom(I2C_SLAVE, 4);     // Counter of Measurements
    while (Wire.available()) { 
      char c = Wire.read(); 
      Serial.print(c);         
    }
    Serial.println("");
    runNumber++;
  } 
  
  //Wire.requestFrom(I2C_SLAVE, 4);    // request 6 bytes from slave device #8
  //Identifier immer 3 stellig
  //MOT
  //TMP
  //HUM
  //PRS

  while (Wire.available()) { 
    char c = Wire.read(); 
    Serial.print(c);         
  }
}
