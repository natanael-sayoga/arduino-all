//https://cedalo.com/blog/how-to-install-mosquitto-mqtt-broker-on-windows/ -> adding mosquitto to path variable and make sure the service is running
//https://www.emqx.com/en/blog/esp8266-connects-to-the-public-mqtt-broker  -> how to connect to broker
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

//init for sinyal suhu
int DHTPIN = D4;
int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE);

// WiFi
const char *ssid = "Redmi Note 10S"; // Enter your WiFi name
const char *password = "bukandiUKDW2020";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic1 = "natanael sayoga/DHT";
const char *topic2 = "natanael sayoga/PING";
const char *mqtt_username = "";
const char *mqtt_password = "";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

int button = D1;
int led1 = D8;
int led2 = D7;
int led3 = D6;

//DEBOUNCING
bool buttonState;
bool modeSuhu = true;
unsigned long loopTimer = millis();
unsigned long loopInterval = 1000;
unsigned long buttonTimer = millis();
unsigned long buttonInterval = 100;
unsigned long timerJarak = millis();
unsigned long timerSuhu = millis();

//init for sinyal jarak
const int trigPin = D3; 
const int echoPin = D2; 
long duration;
int distance;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(button, INPUT_PULLUP);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  //untuk sinyal jarak
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //turn on the built in led for the rest of the program
  digitalWrite(LED_BUILTIN, LOW);
  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
    String client_id = "esp8266-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
        Serial.println("Public emqx mqtt broker connected");
    } else {
        Serial.print("failed with state ");
        Serial.print(client.state());
        delay(2000);
    }
  }
  // // publish and subscribe

  client.subscribe(topic1);
  client.subscribe(topic2);
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}

void loop() {
  client.loop();
  // put your main code here, to run repeatedly:
  unsigned long currentTime = millis();
  if(currentTime - buttonTimer >= buttonInterval){
    boolean currentButtonState = digitalRead(button);
    buttonTimer = currentTime;
    if(buttonState != currentButtonState){
      buttonState = currentButtonState;
      if(!buttonState){
        //karena input pullup, kita cek keadaan ketika buttonState == false / grounded
        modeSuhu = !modeSuhu;
      }
    }
    // Serial.print("current mode: ");
    // String mode = modeSuhu?"suhu":"ping";
    // Serial.println(mode);
  }
  if(modeSuhu){
    sensorSuhu();
  }else{
    sensorJarak();
  }
}

void sensorJarak(){
  //clear the trig pin:
  unsigned long currentTimerJarak = millis();
  const char *currentTopic = "natanael sayoga/PING";
  String message = "";
  if(currentTimerJarak - timerJarak >= 1000){
    timerJarak = currentTimerJarak;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    //sets the trigpin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    //reads the echoPin, return the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);

    //calculate the distance
    distance = duration * 0.034 / 2; //340 M/s 
    String distanceString = String(distance);
    message += distanceString;
    Serial.print("Distance: ");
    Serial.print(distance);
    if(distance<10){
      Serial.println(" CM, LED 1 ON!");
      message+=" CM, LED 1 ON!";
      nyalakanLampu(led1);
      matikanLampu(led2);
      matikanLampu(led3);
    }else if(distance>=10 && distance<20){
      Serial.println(" CM, LED 2 ON!");
      message+=" CM, LED 2 ON!";
      nyalakanLampu(led2);
      matikanLampu(led1);
      matikanLampu(led3);
    }else{
      Serial.println(" CM, LED 3 ON!");
      message+=" CM, LED 3 ON!";
      nyalakanLampu(led3);
      matikanLampu(led1);
      matikanLampu(led2);
    }
    client.publish(currentTopic, message.c_str());
  }
  //delay(1000);
}

void sensorSuhu(){
  // Wait a few seconds between measurements.
  const char *currentTopic = "natanael sayoga/DHT";
  unsigned long currentTimerSuhu = millis();
  if(currentTimerSuhu - timerSuhu >= 1000){
    timerSuhu = currentTimerSuhu;
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    //float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    String message = "";
    if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      client.publish(currentTopic, "Failed to read from DHT sensor!");
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
    message += "Humidity: "+humidString+"% Temperature: "+tempString+"°C ";

    if(t<20){
      nyalakanLampu(led1);
      matikanLampu(led2);
      matikanLampu(led3);
      Serial.println(", LED 1 ON!");
      message+=", LED 1 ON!";
    }else if(t>=20 && t<28){
      matikanLampu(led1);
      nyalakanLampu(led2);
      matikanLampu(led3);
      Serial.println(", LED 2 ON!");
      message+=", LED 1 ON!";
    }else{
      matikanLampu(led1);
      matikanLampu(led2);
      nyalakanLampu(led3);
      Serial.println(", LED 3 ON!");
      message+=", LED 1 ON!";
    }
    client.publish(currentTopic, message.c_str());
  }
}

void nyalakanLampu(int led){
  digitalWrite(led, HIGH);
}

void matikanLampu(int led){
  digitalWrite(led, LOW);
}
