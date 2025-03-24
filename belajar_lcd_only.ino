// this code is written using LiquidCrystal I2C by Frank de Branbander
// SDA: always connect this pin to GPIO 4 (D2 on many ESP8266 boards).
// SCL: always connect this pin connect to GPIO 5 (D1 on many ESP8266 boards)
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//lcd initilization
LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display
                                  // 0x27 adalah alamat yg didapat dari scan_i2c.ino
void setup(){
  lcd.init();      // initiating the lcd 
  lcd.backlight(); // turning on the lcd light
}

void loop(){
  //cursor(kolom / char, baris) -> index start at 0
  lcd.setCursor(0,0);
  lcd.print("Hello, world!");
  lcd.setCursor(0,1);
  lcd.print("belajar LCD!");

  delay(2000); //supaya bisa dibaca manusia
  lcd.clear(); //menghilangkan string mula-mula

  lcd.setCursor(0,0);
  lcd.print("Goodbye, world!");
  lcd.setCursor(0,1);            //read from column to row first
  lcd.print("selesai :)");
  delay(2000); 
  lcd.clear(); 
}