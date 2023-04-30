#include <ArduinoBLE.h>

const char* serviceUuid = "2c3955ff-3427-4915-a5de-26ab681c45e4";
const char* rotationCountCharacteristicUuid = "00000001-0000-0000-0000-000000000000";

BLEService rotationService(serviceUuid);
BLEIntCharacteristic rotationCountCharacteristic(rotationCountCharacteristicUuid, BLERead | BLENotify);

int rotationCount = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) {}

  if (!BLE.begin()) {
    Serial.println("Failed to initialize BLE");
    while (1);
  }

  BLE.setLocalName("Arduino Roller");
  BLE.setAdvertisedService(rotationService);

  rotationService.addCharacteristic(rotationCountCharacteristic);
  BLE.addService(rotationService);

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
      // Update rotationCount value and notify p5.js
      rotationCount++; // Replace this with your actual rotation count calculation
      rotationCountCharacteristic.writeValue(rotationCount);
      delay(1000);
    }

    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}
