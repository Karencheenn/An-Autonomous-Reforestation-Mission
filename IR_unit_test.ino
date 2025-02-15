#define IR_SENSOR A4  // IR Sensor connected to Analog Pin A0

void setup() {
  Serial.begin(9600);  // Start serial communication
}

void loop() {
  int sensorValue = analogRead(IR_SENSOR);  // Read analog value from sensor
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);  // Print value to Serial Monitor

  // Threshold to differentiate Black & White
  if (sensorValue < 500) {  // Adjust this value based on your sensor
    Serial.println("⬜ White Surface Detected!");
  } else {
    Serial.println("⬛ Black Surface Detected!");
  }

  delay(500);  // Wait for stability
}
