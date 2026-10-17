#include <Servo.h>

// -------- ULTRASONIC --------
#define trigPin 3
#define echoPin 2

// -------- MOTOR DRIVER --------
#define FLIP_A 1
#define FLIP_B 1

#if FLIP_A
  #define IN1 9
  #define IN2 10
#else
  #define IN1 10
  #define IN2 9
#endif

#if FLIP_B
  #define IN3 7
  #define IN4 8
#else
  #define IN3 8
  #define IN4 7
#endif

#define ENA 11
#define ENB 6

// -------- SERVO --------
#define servoPin 5
Servo myServo;

// -------- SPEED --------
int speedLeft = 140;
int speedRight = 140;

// -------- DETECTION --------
int detectDist  = 40;        
int sideBlocked = 30;        

// -------- TIMINGS (ms) - tune these --------
int backTimeNormal  = 300;   
int backTimeTrapped = 500;   
int turnTimeNormal  = 650;   
int turnTimeTrapped = 1200;  

// -------- SETUP --------
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  myServo.attach(servoPin);
  myServo.write(90);

  stopCar();
  delay(1000);
}

// -------- LOOP --------
void loop() {

  int frontDist = getDistance();

  if (frontDist > detectDist) {
    forward();
  }
  else {

    // STOP
    stopCar();
    delay(100);

    // SMALL BACKUP (create turning space)
    backward();
    delay(backTimeNormal);
    stopCar();
    delay(100);

    // SCAN RIGHT & LEFT
    int rightDist = lookRight();
    int leftDist  = lookLeft();

    delay(100);

    // DECIDE DIRECTION
    if (rightDist < sideBlocked && leftDist < sideBlocked) {
      // TRAPPED (3 walls): back up more, then U-turn
      backward();
      delay(backTimeTrapped);
      stopCar();
      delay(100);
      turnRight(turnTimeTrapped);
    }
    else if (rightDist > leftDist) {
      turnRight(turnTimeNormal);
    }
    else {
      turnLeft(turnTimeNormal);
    }

    stopCar();
    delay(100);
  }
}

// -------- ULTRASONIC --------
// One raw reading in cm (200 = no echo)
int readOnce() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) return 200;

  return duration * 0.034 / 2;
}

int getDistance() {
  int d1 = readOnce();
  delay(10);
  int d2 = readOnce();
  delay(10);
  int d3 = readOnce();

  int m = d1;
  if (d2 < m) m = d2;
  if (d3 < m) m = d3;
  return m;
}

// -------- SCAN SERVO --------
int lookRight() {
  myServo.write(10);
  delay(300);
  int d = getDistance();
  myServo.write(90);
  delay(200);
  return d;
}

int lookLeft() {
  myServo.write(170);
  delay(300);
  int d = getDistance();
  myServo.write(90);
  delay(200);
  return d;
}

void forward() {
  analogWrite(ENA, speedRight);
  analogWrite(ENB, speedLeft);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void backward() {
  analogWrite(ENA, speedRight);
  analogWrite(ENB, speedLeft);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnRight(int ms) {
  analogWrite(ENA, speedRight);
  analogWrite(ENB, speedLeft);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  delay(ms);
}

void turnLeft(int ms) {
  analogWrite(ENA, speedRight);
  analogWrite(ENB, speedLeft);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  delay(ms);
}

void stopCar() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
