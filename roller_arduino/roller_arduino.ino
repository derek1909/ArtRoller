
#include <ArduinoBLE.h>

const char* serviceUuid = "2c3955ff-3427-4915-a5de-26ab681c45e4";
const char* rotationCountCharacteristicUuid = "00000001-0000-0000-0000-000000000000";
const char* joystickXCharacteristicUuid = "00000002-0000-0000-0000-000000000000";
const char* joystickYCharacteristicUuid = "00000003-0000-0000-0000-000000000000";

BLEService rollerService(serviceUuid);
BLEIntCharacteristic joystickXCharacteristic(joystickXCharacteristicUuid, BLERead | BLENotify);
BLEIntCharacteristic joystickYCharacteristic(joystickYCharacteristicUuid, BLERead | BLENotify);
BLEIntCharacteristic rotationCountCharacteristic(rotationCountCharacteristicUuid, BLERead | BLENotify);

int rotationCount = 0;

// Replace with your sensor pins
const int sensorA = 14;
const int sensorB = 16;

const int joystickXPin = A6;
const int joystickYPin = A5;

int lastSensorAState;
int lastSensorBState;

void setup() {
  pinMode(sensorA, INPUT);
  pinMode(sensorB, INPUT);
  pinMode(joystickXPin, INPUT);
  pinMode(joystickYPin, INPUT);

  lastSensorAState = digitalRead(sensorA);
  lastSensorBState = digitalRead(sensorB);

  Serial.begin(9600);
  while (!Serial) {}

  if (!BLE.begin()) {
    Serial.println("Failed to initialize BLE");
    while (1);
  }

  BLE.setLocalName("Arduino Roller");
  BLE.setAdvertisedService(rollerService);

  rollerService.addCharacteristic(rotationCountCharacteristic);
  rollerService.addCharacteristic(joystickXCharacteristic);
  rollerService.addCharacteristic(joystickYCharacteristic);
  
  BLE.addService(rollerService);

  rotationCountCharacteristic.writeValue(rotationCount);


  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      updateRotationCount();
      rotationCountCharacteristic.writeValue(rotationCount);
      
      int joystickX = analogRead(joystickXPin);
      int joystickY = analogRead(joystickYPin);
      joystickXCharacteristic.writeValue(joystickX);
      joystickYCharacteristic.writeValue(joystickY);
      
      delay(1);
    }

    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}

void updateRotationCount() {
  int sensorAState = digitalRead(sensorA);
  int sensorBState = digitalRead(sensorB);
   
  // Check if both sensors are HIGH
  if (sensorAState == HIGH && sensorBState == HIGH) {
    // Determine the direction of rotation
    if (lastSensorAState == LOW && lastSensorBState == HIGH) {
      rotationCount++; // Clockwise rotation
      Serial.print("Clockwise Rotation, Count: ");
      Serial.println(rotationCount);
    } else if (lastSensorAState == HIGH && lastSensorBState == LOW) {
      rotationCount--; // Counterclockwise rotation
      Serial.print("Counterclockwise Rotation, Count: ");
      Serial.println(rotationCount);
    }
  }
  
  //Serial.print("A:");
  //Serial.print(sensorAState);
  //Serial.print(",");
  //Serial.print("B:");
  //Serial.println(sensorBState);

  lastSensorAState = sensorAState;
  lastSensorBState = sensorBState;

  delay(1); // Add a small delay to avoid false readings due to sensor noise or bouncing
}
