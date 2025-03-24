//init
const int trigPin = D1; 
const int echoPin = D0; 
long duration;
int distance;

//debouncing
unsigned long loopTimer = millis();
unsigned long loopInterval = 1000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentTimer = millis();
  if(currentTimer-loopTimer >= loopInterval){
    loopTimer = currentTimer;
    Serial.println(sensor_ping());
  }
}

String sensor_ping(){
  String message = "";
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  //sets the trigpin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  //reads the echoPin, return the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  //calculate the distance
  distance = duration * 0.034 / 2; //s = v*t/2, where v = 340 m/s OR 0.034 cm/s 
  String distanceString = String(distance);
  message += "Distance: "+distanceString+" CM\n";
  if(distance<10){
    message+="Near...\0";
  }else if(distance>=10 && distance<20){
    message+="Far...\0";
  }else{
    message+="Very far...\0";
  }
  return message;
}