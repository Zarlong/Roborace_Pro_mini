#include <Trackduino.h>
#include <Servo.h>

Servo servo;

int pingPin1 = IN1;
int pingPin2 = IN2;
int pingPin3 = IN3;

int MAS[9];
int MASS[9];
int C1 = 0;
int C2 = 0;
int C3 = 0;
int kservo = 120;
const int motor_number1 = 1;
const int motor_number2 = 2;
//long duration1, inches1, cm1;
//long duration2, inches2, cm2;

bool pov = false;

void setup() {
  Serial.begin(9600);
  Trackduino.setup();
  servo.attach(OUT1);
}
void loop() {
  C1 = cm(pingPin1);
  C2 = cm(pingPin2);
  C3 = cm(pingPin3);
  forward(90);
  backward(980);
  pid();
}

long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
int cm(int a) {
  int r = 0;

  for (int i = 0; i < 5; ++i) {
    pinMode(a, OUTPUT);
    digitalWrite(a, LOW);
    delayMicroseconds(2);
    digitalWrite(a, HIGH);
    delayMicroseconds(10);
    digitalWrite(a, LOW);
    pinMode(a, INPUT);
    int duration = pulseIn(a, HIGH);
    int cmet = microsecondsToInches(duration);
    r += cmet;
  }
  r = r / 5;
  return r; // возвращаю расстояние в сантиметрах
}

void pid() {
  int E = C1 - C2;//датчик и разность
  //  for (int i = 0; i < 8; i++) MAS[i] = MAS[i + 1];
  //  MAS[8] = E;
  //  for (int i = 0; i < 9; i++) MASS[i] = MAS[i];
  //  for (int j = 0; j < 9; j++) {
  //    for (int i = 0; i < 8; i++) {
  //      if (MASS[i] > MASS[i + 1]) {
  //        int temp = MASS[i];
  //        MASS[i] = MASS[i + 1];
  //        MASS[i + 1] = temp;
  //      }
  //    }
  //  }
  float K = 0.6;
  int A = kservo + (E * K);
  A = constrain(A, 80, 160);
  servo.write(A);
  if (A < kservo){
    pov = true;
  }
  else {
    pov = false;
  }
  //  Serial.println(A);
  //  Serial.print(E);
  //  Serial.print("\t");
  //  Serial.println(MASS[4]);
}

void backward(int s) {
  if (C3 < 10) {
    while (C3 < 20) {
      C3 = cm(pingPin3);
      servo.write(kservo);
      Trackduino.motor(motor_number1, -s + 10);
      Trackduino.motor(motor_number2, -s + 10);
    }
    if (pov == false){
      servo.write(85);
      Trackduino.motor(motor_number1, s);
      Trackduino.motor(motor_number2, s);
      delay(1500);
    }
    else{
      servo.write(155);
      Trackduino.motor(motor_number1, s);
      Trackduino.motor(motor_number2, s);
      delay(1500);
    }
  }
  //    if ((C1 - C2) > 0) {
  //      Trackduino.motor(motor_number1, s);
  //      Trackduino.motor(motor_number2, s-30);
  //      delay(1000);
  //      Trackduino.motor(motor_number1, s);
  //      Trackduino.motor(motor_number2, s);
  //      delay(2000);
  //    }
  //    else {
  //      Trackduino.motor(motor_number1, s-30);
  //      Trackduino.motor(motor_number2, s);
  //      delay(1000);
  //      Trackduino.motor(motor_number1, s);
  //      Trackduino.motor(motor_number2, s);
  //      delay(2000);
  //    }
  //  }
}
void forward(int s) {
  Trackduino.motor(motor_number1, s);
  Trackduino.motor(motor_number2, s);
}
