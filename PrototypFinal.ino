#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

//Define Relais
int RH = 4;
int RR = 7;
int LH = 8;
int LR = 11;

int counter = 0;
String command;
float measuredresult;

Adafruit_MPU6050 mpu;

void pullup() {
  stopall();
  delay(500);
  digitalWrite(LR,LOW);
  digitalWrite(RR,LOW);
  while (command == "end"){
    command = Serial.readString();
  }
}

void stopall() {
  digitalWrite(RR,HIGH);
  digitalWrite(RH,HIGH);
  digitalWrite(LH,HIGH);
  digitalWrite(LR,HIGH);
}

void expandall() {
  //Benötigt 115000ms zum vollen Ausfahren
  stopall();
  delay(500);
  digitalWrite(LH,LOW);
  digitalWrite(RH,LOW);
  while (command == "expand"){
    command = Serial.readString();
  }
}

void measuresensor() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  measuredresult = a.acceleration.x;
}

void checkserial(){
  if(Serial.available()){
  command = Serial.readStringUntil('\n');
  Serial.println(command);
}}

void balancevehicle() {
  stopall();
  delay(500);
  checkserial();
  measuresensor();

  while (measuredresult >= -0.06){
    digitalWrite(RH,LOW);
    delay(100);
    measuresensor();
    checkserial();
    if (command != "balance"){
      break;
    }}
    stopall();

  while (measuredresult <= -0.14){
    digitalWrite(LH,LOW);
    delay(100);
    measuresensor();
    checkserial();
    if (command != "balance"){
      break;
    }}
    stopall();
    command = "stop";
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RH,OUTPUT);
  pinMode(RR,OUTPUT);
  pinMode(LH,OUTPUT);
  pinMode(LR,OUTPUT);
  digitalWrite(RR,HIGH);
  digitalWrite(RH,HIGH);
  digitalWrite(LH,HIGH);
  digitalWrite(LR,HIGH);
  
  Serial.begin(9600);
  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
  }

  //Initialisierungsshit
  Serial.println("MPU6050 Found!");
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  Serial.println("Accelerometer range set to: +-2G ");
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  Serial.println("Gyro range set to: +- 250 deg/s");
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.println("Filter bandwidth set to: 5 Hz");

  //Sensorabfrage
  measuresensor();
}

void loop() {
  
  delay(500);
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");


 if(Serial.available()){
  command = Serial.readStringUntil('\n');
  Serial.println(command);
 }
  if(command.equals("end")){
      pullup();
  }
  else if(command.equals("stop")){
      stopall();
  }
  else if(command.equals("balance")){
      balancevehicle();
  }
  else if(command.equals("expand")){
      expandall();
  }

}
