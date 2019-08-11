#include <DS18B20.h>
#include <OneWire.h>

#define HIG A0 //pin analog higr
#define DHIGred 2 //higr dioda czerwona
#define DHIGyell 3 //higr dioda zolta
#define TEMP 4
#define WENT 5

//OBIEKTY klas
OneWire onewire(TEMP);
DS18B20 tempSensor(&onewire); //w nawiasie referencja do obiektu onewire klasy OneWire

//ZMIENNE przypisane na sztywno
//8 elementowa tablica typu byte z adresem czujnika
byte tempAddress[8] = {0x28, 0xFF, 0x98, 0x3E, 0x2, 0x17, 0x4, 0x30};
//temp
float temperature;
//wentylacja
float defTemp = 25.1;
float refDegree = 0.5;
int wypelnienie;

void setup() {
  //TEMPERATURA
  //start tempSensor
  tempSensor.begin();
  //zadanie pomiaru temp
  tempSensor.request(tempAddress);
  //Serial monitor
  Serial.begin(9600);
  
  //HIGROMETR
  //diody deklaracja
  pinMode(DHIGred, OUTPUT);
  pinMode(DHIGyell, OUTPUT);
  //diody stan poczatkowy
  digitalWrite(DHIGred, LOW);
  digitalWrite(DHIGyell, LOW);
  
  //WENTYLACJA
  pinMode(WENT, OUTPUT);
}

void loop() {
  //HIGROMETR
  int higr = analogRead(HIG);
  //test//
  Serial.print("HIGR: ");
  Serial.println(higr);
  delay(1000);
  //diody stanu nawodnienia
  if (higr < 400){ //czerwona dioda
    digitalWrite(DHIGred, HIGH);
    digitalWrite(DHIGyell, LOW);
  }
  else if ((higr > 400) && (higr < 600)){ //zolta dioda
    digitalWrite(DHIGred, LOW);
    digitalWrite(DHIGyell, HIGH);
  }
  else{
    digitalWrite(DHIGred, LOW);
    digitalWrite(DHIGyell, LOW);
  }

  //TEMPERATURA
  //sprawdzenie czy czujnik skonczyl pomiar
  if (tempSensor.available()){
    //pobieranie temperatury
    temperature = tempSensor.readTemperature(tempAddress);
    //test//
    Serial.print("TEMP: ");
    Serial.println(temperature);
    delay(2000);
    //zadanie pomiaru temperatury
    tempSensor.request(tempAddress);
  }

  //WENTYLACJA
  if (temperature <= defTemp){
  wypelnienie = 128;
  analogWrite(WENT, wypelnienie);
  }
  else{
    float tempCompare = temperature - defTemp;
    int wentIncrease = round(tempCompare/refDegree);
    wypelnienie = wentIncrease + 128;
    if (wypelnienie > 255){
      wypelnienie = 255;    
      analogWrite(WENT, wypelnienie);
      //test
      Serial.print("MOC: ");
      Serial.println(wypelnienie);
      delay(3000);
    }
    else{
      analogWrite(WENT, wypelnienie);
      //test
      Serial.print("MOC: ");
      Serial.println(wypelnienie);
      delay(3000);
    }
  }
 }
