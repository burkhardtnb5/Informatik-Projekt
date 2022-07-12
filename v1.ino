#include <HX711_ADC.h>
#include <ArduinoBLE.h>

//Initializie Bluetooth Sevice
BLEService gaslevelService("1813");

//Initialize Bluetooth Char
BLEUnsignedCharCharacteristic gaslevelChar("2A98",BLERead | BLENotify);
BLEUnsignedCharCharacteristic percentChar("2A01",BLERead | BLENotify);//2A01

//pins:
const int HX711_dout = 3; //mcu > HX711 dout pin
const int HX711_sck = 4; //mcu > HX711 sck pin

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);
float calibrationValue = 696.0;

void setup(){
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  LoadCell.begin();
  LoadCell.start(2000, false);
  LoadCell.setCalFactor(calibrationValue);

  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  BLE.setLocalName("Gasflasche");
  BLE.setAdvertisedService(gaslevelService); // add the service UUID
  gaslevelService.addCharacteristic(gaslevelChar); // add the battery level characteristic
  gaslevelService.addCharacteristic(percentChar);
  BLE.addService(gaslevelService);
  
  BLE.advertise();
  Serial.println("Bluetooth® device active, waiting for connections...");
}

void loop(){
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to central: ");
    // print the central's BT address:
    Serial.println(central.address());
    // turn on the LED to indicate the connection:
    digitalWrite(LED_BUILTIN, HIGH);

    while (central.connected()){
      LoadCell.update();
      gaslevelChar.writeValue(LoadCell.getData());
      Serial.println(LoadCell.getData());
      
      delay(200);
    }

    // when the central disconnects, turn off the LED:
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
}
}
