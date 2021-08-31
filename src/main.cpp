// WireCommMaster between two ESP8266 ##################################
// Masterthesis V 0.1.0 ################################################
// by Joel Lehmann #####################################################
// 30.08.2021 ##########################################################

#include <Arduino.h>
#include <Wire.h>
#include <ArduinoJson.h>

#define SDA_PIN 4
#define SCL_PIN 5
const int16_t I2C_MASTER = 0x42;
const int16_t I2C_SLAVE = 0x08;
int runNumber = 1;
int count = 0;
int i = 0;
char c;
char metaCount;
char mesCount;
char buffer[200];
char buffertmp[200];
bool bEOS;
String sMeta;
StaticJsonDocument<200> doc;

typedef struct
  {
    String key;
    String value;
  }  kvp;

kvp META[23];
kvp MES[23];

void setup() {
  Serial.begin(9600);
  Wire.begin(SDA_PIN, SCL_PIN, I2C_MASTER);
  Wire.setClock(30000);
  Serial.println("Setup accomplished!");
}

void loop() {
  
//################################################################################
//Request Number of Metadata #####################################################
//################################################################################
  
  if (runNumber == 1)
  {
    Wire.beginTransmission(I2C_SLAVE);
    Wire.write("reqMetaCount\n");
    Wire.endTransmission();
    delay(100);
    Wire.requestFrom(I2C_SLAVE, 1);
    while (Wire.available()) { 
      c = Wire.read();
      metaCount = c;      
    }
    Serial.println("");
    runNumber++;
  } 
  delay(100);

//################################################################################
//Request Metadata ###############################################################
//################################################################################

  if (runNumber == 2)
  {
    while (i < (int)metaCount-48)
    {
      Wire.beginTransmission(I2C_SLAVE);
      Wire.write((String(i)+"\n").c_str());
      Wire.endTransmission();  
      delay(100);
      Wire.requestFrom(I2C_SLAVE, 23);
      while (Wire.available()) { 
      c = Wire.read();
      if (c != 255 && !bEOS) {   
        sMeta += c;     
      } else {
        bEOS = true;
      }
    }
    Serial.print("KVP1 ");
    Serial.print(i);
    Serial.print(" -> ");
    META[i].key = sMeta.substring(0,sMeta.indexOf(":"));
    META[i].value = sMeta.substring(sMeta.indexOf(":")+1,sMeta.length());
    sMeta = "";
    bEOS = false;

    Serial.print(META[i].key);
    Serial.print(":");
    Serial.print(META[i].value);
    Serial.println("");
    i++;
    }
    runNumber++;
    i = 0;
  }
    delay(100);
    
//################################################################################
//Request Number of Measurements #################################################
//################################################################################

    if (runNumber == 3)
  {
    Wire.beginTransmission(I2C_SLAVE);  
    Wire.write("reqMesCount\n");
    Wire.endTransmission();
    delay(100);
    Wire.requestFrom(I2C_SLAVE, 1);
    while (Wire.available()) { 
      c = Wire.read();
      mesCount = c;      
    }
    Serial.println("");
    runNumber++;
  } 
  delay(100);

//################################################################################
//Request Measurements ###########################################################
//################################################################################

if (runNumber == 4)
  {
    while (i < (int)mesCount-48)
    {
      Wire.beginTransmission(I2C_SLAVE);
      Wire.write((String(i)+"\n").c_str());
      Wire.endTransmission();  
      delay(100);
      Wire.requestFrom(I2C_SLAVE, 23);
      while (Wire.available()) { 
      c = Wire.read();
      if (c != 255 && !bEOS) {   
        sMeta += c;     
      } else {
        bEOS = true;
      }
    }
    Serial.print("KVP2 ");
    Serial.print(i);
    Serial.print(" -> ");
    MES[i].key = sMeta.substring(0,sMeta.indexOf(":"));
    MES[i].value = sMeta.substring(sMeta.indexOf(":")+1,sMeta.length());
    sMeta = "";
    bEOS = false;

    Serial.print(MES[i].key);
    Serial.print(":");
    Serial.print(MES[i].value);
    Serial.println("");
    i++;
    }
    Serial.println("");
    runNumber++;
    i = 0;
  }
    delay(100);

//################################################################################
//Request Telemetry ##############################################################
//################################################################################

if (runNumber == 5)
  {
    while (i < (int)mesCount-48)
    {
      Wire.beginTransmission(I2C_SLAVE);
      Wire.write(("req"+String(MES[i].key)+"\n").c_str());
      Wire.endTransmission();  
      delay(100);
      Wire.requestFrom(I2C_SLAVE, 23);
      while (Wire.available()) { 
      c = Wire.read();
      if (c != 255 && !bEOS) {   
        sMeta += c;     
      } else {
        bEOS = true;
      }
    }
    Serial.print(MES[i].key);
    Serial.print(" -> ");
    Serial.println(sMeta);
    sMeta = "";
    bEOS = false;
    i++;
    }
    Serial.println("");
    runNumber++;
    i = 0;
  }
    delay(3000);

//################################################################################
//Building up ATTRIBUTES JSON ####################################################
//################################################################################

if (runNumber == 6)
  {
    JsonObject devid = doc.createNestedObject(META[0].value);
    i = 1;

    while (i < (int)metaCount-48) 
    {
      devid[META[i].key] = META[i].value;
      i++;
    }

    serializeJson(doc, buffertmp);
    strncat(buffer, buffertmp+1,strlen(buffertmp)-2);
    Serial.println(buffer);

    runNumber++;
    i = 0;
    memset(buffer, 0, sizeof buffer);
    memset(buffertmp, 0, sizeof buffer);
    doc.clear();
  }
    delay(3000);

//################################################################################
//Building up FEATURES JSON ######################################################
//################################################################################

if (runNumber == 7)
  {
    while (i < (int)mesCount-48) 
    {
      JsonObject mesid = doc.createNestedObject(MES[i].key);
      mesid["value"] = "init";
      mesid["unit"] = MES[i].value;
      i++;
    }

    serializeJson(doc, buffertmp);
    strncat(buffer, buffertmp+1,strlen(buffertmp)-2);
    Serial.println(buffer);

    runNumber++;
    i = 0;
    memset(buffer, 0, sizeof buffer);
    memset(buffertmp, 0, sizeof buffer);
    doc.clear();
  }
    delay(99999999);

}
