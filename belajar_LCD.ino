#include "DHT.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//init lcd
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//init for sinyal suhu
int DHTPIN = D6;
int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE);

unsigned long timerSuhu = millis();
int led1 = D8;
int led2 = D4;
int led3 = D3;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  
  //starting dht and lcd
  dht.begin();
  lcd.init();
  lcd.backlight();
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorSuhu();
}

void sensorSuhu(){
  // Wait a few seconds between measurements.
  unsigned long currentTimerSuhu = millis();
  if(currentTimerSuhu - timerSuhu >= 1000){
    timerSuhu = currentTimerSuhu;
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    lcd.clear(); //menghilangkan string mula-mula

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      lcd.setCursor(0,0);
      lcd.print("Failed to read");
      lcd.setCursor(0,1);
      lcd.print("from DHT sensor!");

      matikanLampu(led1);
      matikanLampu(led2);
      matikanLampu(led3);
      return;
    }

    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));

    String tempString = String(t);
    String humidString = String(h);

    lcd.setCursor(0,0);
    lcd.print("Humidity: ");
    lcd.setCursor(10,0);
    lcd.print(humidString);

    lcd.setCursor(0,1);
    lcd.print("Temp: ");
    lcd.setCursor(7,1);
    lcd.print(tempString);

    if(t<20){
      nyalakanLampu(led1);
      matikanLampu(led2);
      matikanLampu(led3);
      Serial.println(", LED 1 ON!");
    }else if(t>=20 && t<28){
      matikanLampu(led1);
      nyalakanLampu(led2);
      matikanLampu(led3);
      Serial.println(", LED 2 ON!");
    }else{
      matikanLampu(led1);
      matikanLampu(led2);
      nyalakanLampu(led3);
      Serial.println(", LED 3 ON!");
    }
  }
}

void nyalakanLampu(int led){
  digitalWrite(led, HIGH);
}

void matikanLampu(int led){
  digitalWrite(led, LOW);
}
