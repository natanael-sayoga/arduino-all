// analogRead()  -> can READ value from 0 -> 1023 (10 Bit Integer)
// analogWrite() -> can WRITE value from 0 -> 255 (8 Bit Integer)

unsigned long loopTimer = millis();
unsigned long loopInterval = 100;

int potentiometer = A0;
int led = D1;
void setup() {
  Serial.begin(9600);
  pinMode(potentiometer, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentTimer = millis();
  if(currentTimer - loopTimer >= loopInterval){
    loopTimer = currentTimer;
    readAnalogValue();
  }
}

void readAnalogValue(){
  //analogReadResolution(10);
  
  Serial.print("Analog value -> ");
  Serial.print(analogRead(potentiometer));
  Serial.print(", Voltage: ");
  Serial.println(analogConverter());
}

float analogConverter(){
  float voltage = analogRead(potentiometer) * 3.3 / 1023.0;
  
  //coonvert 1024 based bit to 256 based bit
  int brightness = analogRead(potentiometer) / 4; //karena int, maka value dibelakang koma akan di truncate
  analogWrite(led, brightness);                   //brighness will be an INTEGER from 0 - 255!
  //alternatively:
  //brightness = map(analogRead(potentiometer), 0, 1023, 0, 255); //might vary, some sensor read 1023 and 255 as max value, but mine will read 1024 and 256

  return voltage;
}