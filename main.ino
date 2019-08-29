#include <DS18B20.h>
#include <OneWire.h>
#include <Rtc_Pcf8563.h>

#define HIG A0
#define TEMP 4
#define WENT 5
#define LIGHT 6
#define WATER 7

//OBIEKTY KLAS
OneWire onewire(TEMP);
DS18B20 tempSensor(&onewire);
Rtc_Pcf8563 RTC;

//ZMIENNE
byte tempAddress[8] = {0x28, 0xFF, 0x98, 0x3E, 0x2, 0x17, 0x4, 0x30};
float defTemp = 25.1;
const byte numChars = 32;
char btData[numChars];
boolean newData = false;
//lightON/OFF[2] = {hour, minute}
char lightON[2] = {8, 0}; //08:00
char lightOFF[2] = {16, 0}; //16:00

void setup() {
  Serial.begin(9600);
  
  //TEMPERATURA
  tempSensor.begin();
  tempSensor.request(tempAddress);
  //WENTYLACJA
  pinMode(WENT, OUTPUT);
  //NAWODNIENIE
  pinMode(WATER, OUTPUT);
}

void loop() {
  //BLUETOOTH
    static byte ndx = 0;
    int endMarker = 111;
    int rc;
    
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (rc != endMarker) {
            btData[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            btData[ndx] = '\0'; //koniec instrukcji
            ndx = 0;
            newData = true;
        }
    }
    
    //ROZPOZNAWANIE INSTRUKCJI
    if (newData == true) {
        //t 116, n 110, f 102
        if (btData[0] == 116){
          defTemp = btData[1]*1.0;
        }
        else if (btData[0] == 110){
          //hour
          lightON[0] = btData[1];
          //minute
          lightON[1] = btData[3];
        }
        else if (btData[0] == 102){
          lightOFF[0] = btData[1];
          lightOFF[1] = btData[3];
        }
        
        newData = false;
    }
    
    //CZAS - RTC
    String realTime = RTC.formatTime();
    //PORÓWNANIE CZASU
    if (realTime[0] == '0'){
      realTime.remove(0);
    }
    
    //ŚWIATŁO
    //ON
    if (int(realTime[0]) == int(lightON[0])  && int(realTime[1]) == int(lightON[1])){
      if (digitalRead(LIGHT) == LOW){
        digitalWrite(LIGHT, HIGH);
      }
    }
    //OFF
    if (int(realTime[0]) == int(lightOFF[0])  && int(realTime[1]) == int(lightOFF[1])){
      if (digitalRead(LIGHT) == HIGH){
        digitalWrite(LIGHT, LOW);
      }
    }
    
  //TEMPERATURA POMIAR
  if (tempSensor.available()){
    float temperature;

    temperature = tempSensor.readTemperature(tempAddress);
    tempSensor.request(tempAddress);
  }

  //WENTYLACJA
  if (temperature <= defTemp){
  int fill;
  fill = 128;
  analogWrite(WENT, fill);
  }
  else{
    int fill;
    float tempCompare = temperature - defTemp;
    int wentIncrease = round(tempCompare*12,75);
    
    fill = wentIncrease + 128;
    if (fill > 255){
      fill = 255;    
      analogWrite(WENT, fill);
    else {
      analogWrite(WENT, fill);
    }
    }
  }
  
  //HIGROMETR+POMPA(PRZEKAZNIK)
  int higr = analogRead(HIG);
  if (higr < 900){
    digitalWrite(WATER, HIGH);
    if (higr < 400){
      delay(1000);
      digitalWrite(WATER, LOW);
    }
    else if (higr > 400 && higr < 850){
      delay(640);
      digitalWrite(WATER, LOW);
    }
    else if (higr > 850 && higr < 900){
      delay(200);
      digitalWrite(WATER, LOW);
    }
}
 }
