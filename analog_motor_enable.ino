#include <Servo.h>

// Define IR Sensor Pins
#define IR_SENSOR_LEFT A5
#define IR_SENSOR_RIGHT A4
#define IR_SENSOR_SIDE_LEFT A3
#define IR_SENSOR_SIDE_RIGHT A2

// Define Color Sensor Pins
#define S0 5
#define S1 4
#define S2 3
#define S3 2
#define OUT 12

// Define Motor Pins
#define ENA A7
#define IN1 6
#define IN2 7
#define ENB A6
#define IN3 9
#define IN4 10

// Define Servo Pin
#define SERVO_PIN A2

// Global Variables
Servo myServo;
bool running = true;
bool pathEnded = false;
int motorASpeed = 150;
int motorBSpeed = 150;
int turnSpeed = 180;
int threshold = 500;

void setup() {
    Serial.begin(9600);
    
    pinMode(IR_SENSOR_LEFT, INPUT);
    pinMode(IR_SENSOR_RIGHT, INPUT);
    pinMode(IR_SENSOR_SIDE_LEFT, INPUT);
    pinMode(IR_SENSOR_SIDE_RIGHT, INPUT);
    
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(OUT, INPUT);
    
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    
    myServo.attach(SERVO_PIN);
    
    digitalWrite(S0, HIGH);
    digitalWrite(S1, HIGH);

    calibrateIRSensors();  // 🔧 Auto-calibrate sensors at startup
    
    Serial.println("🚀 Robot Ready to Follow Edge!");
}

void loop() {
  if (Serial.available() > 0) {
        String command = Serial.readString();
        command.trim();
        if (command.equalsIgnoreCase("STOP")) {
            Serial.println("Stopping all functions...");
            stopMotors();
            running = false;
        }
  }
  if (!running) {
      stopMotors();
      return;
  }

  float leftIR = analogRead(IR_SENSOR_LEFT);
  float rightIR = analogRead(IR_SENSOR_RIGHT);
  
  if (leftIR > threshold && rightIR > threshold) {
      Serial.println("⬆️ Moving Forward!");
      moveForward(motorASpeed, motorBSpeed);
  } else if (leftIR < threshold) {
      Serial.println("⬅️ Adjusting Left!");
      turnLeft(turnSpeed);
  } else if (rightIR < threshold) {
      Serial.println("➡️ Adjusting Right!");
      turnRight(turnSpeed);
  } else {
      Serial.println("🛑 Edge Lost - Stopping!");
      stopMotors();
  }
  
  delay(100);
}

void stop() {
    Serial.println("🛑 Emergency Stop Activated!");
    running = false;
    stopMotors();
}

void calibrateIRSensors() {
    Serial.println("🔧 Calibrating IR Sensors...");
    int blackLeft = 1023, blackRight = 1023;
    int whiteLeft = 0, whiteRight = 0;
    for (int i = 0; i < 500; i++) {
        int leftIR = analogRead(IR_SENSOR_LEFT);
        int rightIR = analogRead(IR_SENSOR_RIGHT);
        if (leftIR < blackLeft) blackLeft = leftIR;
        if (leftIR > whiteLeft) whiteLeft = leftIR;
        if (rightIR < blackRight) blackRight = rightIR;
        if (rightIR > whiteRight) whiteRight = rightIR;
        delay(10);
    }
    threshold = (blackLeft + whiteLeft + blackRight + whiteRight) / 4;
    Serial.print("✅ Calibration Complete! New Threshold: ");
    Serial.println(threshold);
}

void moveForward(int speedA, int speedB) {
    analogWrite(ENA, speedA);
    analogWrite(ENB, speedB);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void turnLeft(int speed) {
    analogWrite(ENA, speed * 0.7);
    analogWrite(ENB, speed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void turnRight(int speed) {
    analogWrite(ENA, speed);
    analogWrite(ENB, speed * 0.7);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void stopMotors() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
}
