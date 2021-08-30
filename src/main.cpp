#include <Arduino.h>
#include <Wire.h>
#include <PolledTimeout.h>

#define SDA_PIN 4
#define SCL_PIN 5
const int16_t I2C_MASTER = 0x42;
const int16_t I2C_SLAVE = 0x08;
int runNumber = 1;
int count = 0;
int i = 0;
char c;
char metaCount;
bool bEOS;
String sKey;
String sValue;
String sMeta;

typedef struct
  {
      String key;
      String value;
  }  metadata;

metadata META[23];

void setup() {
  Serial.begin(9600);  // start serial for output
  Wire.begin(SDA_PIN, SCL_PIN, I2C_MASTER);        // join i2c bus (address optional for master)
  Wire.setClock(30000);
  Serial.println("Setup fertig.");
}

void loop() {
  delay(3000);
  
  if (runNumber == 1)
  {
    Serial.println("Transmission");
    Wire.beginTransmission(I2C_SLAVE);  // transmit to device #8
    Wire.write("reqMetaCount\n");       // sends five bytes
    Wire.endTransmission();             // stop transmitting
    delay(100);
    Wire.requestFrom(I2C_SLAVE, 1);     // Counter of Measurements
    while (Wire.available()) { 
      c = Wire.read();
      metaCount = c;      
    }
    Serial.print((int)metaCount-48);
    Serial.println("");
    runNumber++;
  } 
  delay(3000);
  //memset(c, \n, sizeof c);

  if (runNumber == 2)
  {
    while (i < (int)metaCount-48)
    {
      Serial.print("Transmission ");
      Serial.println(i);
      Wire.beginTransmission(I2C_SLAVE);
      //Serial.println(String(i).c_str());
      Wire.write((String(i)+"\n").c_str());
      Wire.endTransmission();  
      delay(100);
      Wire.requestFrom(I2C_SLAVE, 23);
      while (Wire.available()) { 
      c = Wire.read();
      //Serial.println(c);
      if (c != 255 && !bEOS) {   
        sMeta += c;     
      } else {
        bEOS = true;
        //Serial.println("EOS");
      }
    }
    //Serial.println(sMeta);
    sKey = sMeta.substring(0,5);
    //Serial.println(sMeta.length());
    sValue = sMeta.substring(6,sMeta.length());
    Serial.print("KVP ");
    Serial.print(i);
    Serial.print(" -> ");
    META[i].key = sKey;
    META[i].value = sValue;
    sMeta = "";
    sKey = "";
    sValue = "";
    bEOS = false;

    Serial.print(META[i].key);
    Serial.print(":");
    Serial.print(META[i].value);
    Serial.println("");
    i++;
    }
    delay(9999999);
    


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
