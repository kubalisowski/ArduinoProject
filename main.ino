void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly#include <DS18B20.h>
#include <OneWire.h>

#define HIG A0 //pin analog higr
#define DHIGred 8 //higr dioda czerwona
#define DHIGyell 9 //higr dioda zolta
#define TEMP 10
#define WENT 3

//OBIEKTY klas
OneWire onewire(TEMP);
DS18B20 tempSensor(&onewire); //w nawiasie referencja do obiektu onewire klasy OneWire

//ZMIENNE przypisane na sztywno
//8 elementowa tablica typu byte z adresem czujnika
byte tempAddress[8] = {0x28, 0xFF, 0x98, 0x3E, 0x2, 0x17, 0x4, 0x30};
//temp
float temperature;
//stałe zmienne wentylacji
float defTemp = 25.1;
float refDegree = 0.5;


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
  //serial monitor
  Serial.print("HIGR: ");
  Serial.println(higr);
  delay(1000);
  //odczyt wilg
  if (higr < 400){ //czerwona dioda
    digitalWrite(DHIGred, HIGH);
    digitalWrite(DHIGyell, LOW);
  }
  else if ((higr > 400) && (higr < 1000)){ //zolta dioda ---> !!!!! zmienic na 600 !!!!!
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
    //wyswietlanie temperatury
    Serial.print("TEMP: ");
    Serial.println(temperature);
    delay(2000);

    tempSensor.request(tempAddress);
  }

  //WENTYLACJA
  if (temperature <= defTemp){
  int wypelnienie = 128;
  analogWrite(WENT, wypelnienie);
  }
  else{
  float tempCompare = temperature - defTemp;
  int wentIncrease = round(tempCompare/refDegree);
  int wypelnienie = wentIncrease + 128;
  if (wypelnienie > 255){
    wypelnienie = 255;    
  }
  analogWrite(WENT, wypelnienie);
  }
}

}
