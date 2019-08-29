#include <DS18B20.h>
#include <OneWire.h>

#define HIG A0 //pin analog higr
#define TEMP 4
#define WENT 5
#define WATER 7

OneWire onewire(TEMP);
DS18B20 tempSensor(&onewire);

byte tempAddress[8] = {0x28, 0xFF, 0x98, 0x3E, 0x2, 0x17, 0x4, 0x30};

float defTemp = 25.1;

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
  //TEMPERATURA
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
  //HIGROMETR+POMPA+PRZEKAZNIK
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
