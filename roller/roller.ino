const int sensorA = 14; // Connect Sensor A output to digital pin 2
const int sensorB = 16; // Connect Sensor B output to digital pin 3

int lastSensorAState = LOW;
int lastSensorBState = LOW;

int checkingClockwise = LOW;
int checkingAnticlockwise = LOW;

int rotationCount = 0;

void setup() {
  pinMode(sensorA, INPUT);
  pinMode(sensorB, INPUT);
  
  Serial.begin(9600); // Initialize serial communication for debugging
}

void loop() {
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
