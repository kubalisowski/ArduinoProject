#include <Wire.h>
#include <Rtc_Pcf8563.h>

int nHour, nMinute, fHour, fMinute;
byte btData;
char data2;


Rtc_Pcf8563 rtc;

void setup()
{ 
  Serial.begin(9600);
}

// n 110; f 102; t 116
void loop()
{
 if(Serial.available() > 0){
  btData = Serial.read();
  String data = String(btData);
  
  int len = data.length() + 1;
  char data2[len];
  data.toCharArray(data2, len);
  
  //Serial.print(data2);

  char tempChar[2];
  tempChar[0] = data2[3];
  tempChar[1] = data2[4];
  String temp = String(tempChar);
      
  /*if(cmd == '116'){
    String thetemp = String(tempChar);
    defTemp = thetemp.toInt();
    }*/
  Serial.print(temp);
  }
}

 
 //rtcHour = rtc.getHour();
 //delay(1000);
    
