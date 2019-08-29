#include <Wire.h>
#include <Rtc_Pcf8563.h>

Rtc_Pcf8563 rtc;


void setup()
{ 
  Serial.begin(9600);
  rtc.initClock();
  //day, weekday, month, century(1=1900, 0=2000), year(0-99)
  rtc.setDate(29, 4, 3, 0, 17);
  
  rtc.setTime(21, 58, 00);
}

void loop()
{
 
 Serial.println(rtc.formatTime());
 //Serial.println(rtc.formatDate());

  delay(1000);
    
}
