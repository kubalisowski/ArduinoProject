#include <Rtc_Pcf8563.h>
#define LIGHT 6

Rtc_Pcf8563 RTC;

const byte numChars = 32;
char btData[numChars];

boolean newData = false;
float defTemp = 25.1;
//light[2] = {hour, minute}
char lightON[2] = {8, 0}; //08:00
char lightOFF[2] = {16, 0}; //16:00


void setup() {
    Serial.begin(9600);
}

void loop() {
    //recvWithEndMarker();
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
            btData[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }
    
    //nowe parametry do tablic
    if (newData == true) {
        //t 116, n 110, f 102
        if (btData[0] == 116){ //temperatura
          defTemp = btData[1]*1.0;
        }
        else if (btData[0] == 110){ //lightON
          //hour
          lightON[0] = btData[1];
          //minute
          lightON[1] = btData[3];
        }
        else if (btData[0] == 102){ //lightOFF
          lightOFF[0] = btData[1];
          lightOFF[1] = btData[3];
        }
        
        newData = false;
    }
    //Get time as String
    String realTime = RTC.formatTime();
    //Compare Time
    //If 1st char is 0
    if (realTime[0] == '0'){
      realTime.remove(0);
    }
    //IF ON
    if (int(realTime[0]) == int(lightON[0])  && int(realTime[1] == int(lightON[1])){
      if (digitalRead(LIGHT) == LOW){
        digitalWrite(LIGHT, HIGH);
      }
    }
    //IF OFF
    if (int(realTime[0]) == int(lightOFF[0])  && int(realTime[1] == int(lightOFF[1])){
      if (digitalRead(LIGHT) == HIGH){
        digitalWrite(LIGHT, LOW);
      }
    }
    
}
