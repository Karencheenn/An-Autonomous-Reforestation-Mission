#include <Servo.h>

// Define IR Sensor Pins
#define IR_SENSOR_LEFT A5
#define IR_SENSOR_RIGHT A4

// Define Color Sensor Pins
#define S0 5
#define S1 4
#define S2 3
#define S3 2
#define OUT 12

// Define Motor Pins
#define ENA 8
#define IN1 6
#define IN2 7
#define ENB 11
#define IN3 9
#define IN4 10

// Define Servo Pin
#define SERVO_PIN A2

// Global Variables
Servo myServo;
int red, green, blue;
bool running = true;
bool pathEnded = false;
int forwardSpeed = 100;
int turnSpeed = 95;
int threshold = 500;

void setup() {
    Serial.begin(9600);
    pinMode(IR_SENSOR_LEFT, INPUT);
    pinMode(IR_SENSOR_RIGHT, INPUT);
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
    
    if (!running) return;
    
    int leftIR = analogRead(IR_SENSOR_LEFT);
    int rightIR = analogRead(IR_SENSOR_RIGHT);
    GetColors();

    String detectedColor = detectColor();
    Serial.println("Detected Color: " + detectedColor);
    
    if (detectedColor == "Red") {
        Serial.println("Detected: Red - Continue Following Black Line");
        followLine(leftIR, rightIR);
    } else if (detectedColor == "Blue" || detectedColor == "Green") {
        Serial.println("Detected: " + detectedColor + " - Stopping and Rotating Servo");
        stopMotors();
        rotateServo();
        moveForward(forwardSpeed);
    } else {
        followLine(leftIR, rightIR);
    }
    
    checkPathEnd(leftIR, rightIR);
}

String detectColor() {
    if (red < blue && red < green) return "Red";
    if (blue < green && blue < red) return "Blue";
    if (green < red && green < blue) return "Green";
    if (red <= 15 && green <= 15 && blue <= 15) return "White";
    if (red > 50 && green > 50 && blue > 50) return "Black";
    return "Unknown";
}

void followLine(int leftIR, int rightIR) {
    if (leftIR > threshold && rightIR > threshold) {
        Serial.print("rightIR:");
        Serial.println(rightIR);
        Serial.print("leftIR:");
        Serial.println(leftIR);
        Serial.println("👆 Forward!");
        moveForward(forwardSpeed);
    } else if (leftIR < threshold && rightIR > threshold) {
        Serial.print("rightIR:");
        Serial.println(rightIR);
        Serial.print("leftIR:");
        Serial.println(leftIR);
        Serial.println("👉 Turn right!");
        turnRight(turnSpeed);
    } else if (rightIR < threshold && leftIR > threshold) {
        Serial.print("rightIR:");
        Serial.println(rightIR);
        Serial.print("leftIR:");
        Serial.println(leftIR);
        Serial.println("👈 Turn left!");
        turnLeft(turnSpeed);
    } else {
        Serial.print("rightIR:");
        Serial.println(rightIR);
        Serial.print("leftIR:");
        Serial.println(leftIR);
        Serial.println("🛑 Stop motor!");
        stopMotors();
    }
}

void checkPathEnd(int leftIR, int rightIR) {
    if (leftIR < threshold && rightIR < threshold) { // Assume no black path detected
        delay(500); // Ensure the path is truly missing
        leftIR = analogRead(IR_SENSOR_LEFT);
        rightIR = analogRead(IR_SENSOR_RIGHT);
        if (leftIR < threshold && rightIR < threshold) {
            Serial.println("Path Ended - Stopping the robot.");
            stopMotors();
            running = false;
            pathEnded = true;
        }
    }
}

void moveForward(int speed) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, speed);
    analogWrite(ENB, speed);
}

void turnLeft(int speed) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, speed);
    analogWrite(ENB, speed);
}

void turnRight(int speed) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENA, speed);
    analogWrite(ENB, speed);
}

void stopMotors() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
}

void rotateServo() {
    myServo.write(0);
    delay(1000);
    myServo.write(180);
    delay(1000);
}

void GetColors() {
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    red = pulseIn(OUT, LOW, 50000);
    delay(20);
    digitalWrite(S3, HIGH);
    blue = pulseIn(OUT, LOW, 50000);
    delay(20);
    digitalWrite(S2, HIGH);
    green = pulseIn(OUT, LOW, 50000);
    delay(20);
}
