//include DHT sensor library by Adafruit
#include "DHT.h"

//initiating DHT
int DHTPIN = D4;
int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE);

//debouncing
unsigned long loopTimer = millis();
unsigned long loopInterval = 1000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentTimer = millis();
  if(currentTimer - loopTimer >= loopInterval){
    loopTimer = currentTimer;
    Serial.println(sinyal_DHT());
  }
}


String sinyal_DHT(){
  String message = "";
  float h = dht.readHumidity();        
  float t = dht.readTemperature();     //Celcius
  float f = dht.readTemperature(true); //Fahrenheit

  if (isnan(h) || isnan(t) || isnan(f)) {
    //Serial.println(F("Failed to read from DHT sensor!"));
    message = "Failed to read from DHT sensor!";
    return message;
  }

  // Serial.print(F("Humidity: "));
  // Serial.print(h);
  // Serial.print(F("%  Temperature: "));
  // Serial.print(t);
  // Serial.print(F("°C "));

  String tempString = String(t);
  String humidString = String(h);
  message += "Humidity: "+humidString+"% Temperature: "+tempString+"°C ";

  if(t<20){
    //Serial.println(", Panas");
    message+=", Panas";
  }else if(t>=20 && t<28){
    //Serial.println(", Sedang");
    message+=", Sedang";
  }else{
    //Serial.println(", Dingin");
    message+=", Dingin";
  }
  return message;
}