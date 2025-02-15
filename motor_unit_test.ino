// Define Left Motor Pins
#define ENA 8
#define IN1 6
#define IN2 7
// #define ENB 11
// #define IN3 9
// #define IN4 10

int speed = 150;

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  // pinMode(ENB, OUTPUT);
  // pinMode(IN3, OUTPUT);
  // pinMode(IN4, OUTPUT);
}

void loop() {
  delay(2000);
  analogWrite(ENA, speed); // Set Speed (0-255)
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);  // Forward
  // analogWrite(ENB, speed); // Set Speed (0-255)
  // digitalWrite(IN3, LOW);
  // digitalWrite(IN4, HIGH);  // Forward
  delay(2000);

  // digitalWrite(IN1, LOW);
  // digitalWrite(IN2, HIGH); // Reverse
  // digitalWrite(IN3, LOW);
  // digitalWrite(IN4, HIGH);  // Forward
  delay(2000);
}
