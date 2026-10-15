// -------- ULTRASONIC --------
#define trigPin 3
#define echoPin 2

// -------- MOTOR DRIVER --------
#define IN1 10
#define IN2 9
#define IN3 8
#define IN4 7

#define ENA 11e
#define ENB 6

// -------- SPEED --------
int speedLeft = 140;
int speedRight = 140;

// -------- TURN ALTERNATION --------
bool turnRightNext = true;

// -------- SETUP --------
void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  stopCar();
  delay(1000);
}

// -------- LOOP --------
void loop() {

  int frontDist = getDistance();
  Serial.println(frontDist);

  if (frontDist > 25) {
    forward();
  }
  else {
    stopCar();
    delay(100);

    backward();
    delay(350);

    stopCar();
    delay(100);

    if (turnRightNext) {
      turnRight();
    } else {
      turnLeft();
    }
    turnRightNext = !turnRightNext;

    stopCar();
    delay(100);
  }
}

// -------- ULTRASONIC --------
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 25000);

  if (duration == 0) return 200;

  return duration * 0.034 / 2;
}

// -------- MOVEMENTS (direction swapped to match your wiring) --------
void forward() {
  analogWrite(ENA, speedRight);
  analogWrite(ENB, speedLeft);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void backward() {
  analogWrite(ENA, speedRight);
  analogWrite(ENB, speedLeft);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  analogWrite(ENA, speedRight);
  analogWrite(ENB, speedLeft);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  delay(400);
}

void turnLeft() {
  analogWrite(ENA, speedRight);
  analogWrite(ENB, speedLeft);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  delay(400);
}

void stopCar() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}