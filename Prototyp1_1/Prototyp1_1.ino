#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

//Define Relais
int RH = 4;
int RR = 7;
int LH = 8;
int LR = 11;

String command;
float measuredresult;

Adafruit_MPU6050 mpu;

void pullup() {
  digitalWrite(RR,HIGH);
  digitalWrite(RH,HIGH);
  digitalWrite(LH,HIGH);
  digitalWrite(LR,HIGH);
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

void balancevehicle(){   
stopall();
sensors_event_t a, g, temp;
mpu.getEvent(&a, &g, &temp);
measuredresult = a.acceleration.x;
int counter = 0;
while(measuredresult < 0.82 or measuredresult > 0.80){

  
  if(Serial.available()){
  command = Serial.readStringUntil('\n');
  Serial.println(command);
    if(command == "stop"){
      break;
    } 
    }
  if(counter == 1){
    command = "stop";
    break;
    }

  
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  measuredresult = a.acceleration.x;
  
  while(measuredresult < 0.82){
    digitalWrite(RH,LOW);
    delay(50);
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    measuredresult = a.acceleration.x;
  }
  digitalWrite(RH,HIGH);
  delay(1000);
  counter = counter + 1;
  Serial.println(counter);
  
  while(measuredresult > 0.80){
    digitalWrite(LH,LOW);
    delay(50);
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    measuredresult = a.acceleration.x;
  }
  digitalWrite(LH,HIGH);
}
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
  
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  Serial.println("Accelerometer range set to: +-16G ");

  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  Serial.println("Gyro range set to: +- 250 deg/s");

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.println("Filter bandwidth set to: 5 Hz");
  digitalWrite(LED_BUILTIN,HIGH);
  //delay(15000);  
  digitalWrite(LED_BUILTIN,LOW);
  
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

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

}
