#include <Trackduino.h>
#include <Servo.h>

Servo servo;

int pingPin1 = IN1;
int pingPin2 = IN2;
int pingPin3 = IN3;
int IK = IN4;

//int MAS[9];
//int MASS[9];
int C1 = 0;
int C2 = 0;
int C3 = 0;
int kservo = 120;
const int motor_number1 = 1;
const int motor_number2 = 2;
//long duration1, inches1, cm1;
//long duration2, inches2, cm2;

float Kp = 0.27;
float Ki = 0.02;
float Kd = 0.58;

int E = 0;
int Eold = 0;

int PID = 0;
int P = 0;
long long I = 0;
int D = 0;

const int NUM_READ = 5;

unsigned long long pidt = 0;
int pidold = 0;

byte idx1 = 0;                // индекс
float valArray1[NUM_READ];    // массив

byte idx2 = 0;                // индекс
float valArray2[NUM_READ];    // массив

byte idx3 = 0;                // индекс
float valArray3[NUM_READ];    // массив

float EGOR = 0;

void setup() {
  Serial.begin(9600);
  Trackduino.setup();
  servo.attach(OUT1);
//  for (int i = 0; i < NUM_READ; ++i) {
//    C1 = constrain(FILT1(cm(pingPin1)), 10, 100);
//    C2 = constrain(FILT2(cm(pingPin2)), 10, 100);
//    C3 = FILT3(cm(pingPin3));
//  }
//  pinMode(IK, INPUT);
}
void loop() {
  C1 = constrain(cm(pingPin1), 6, 100);
  C2 = constrain(cm(pingPin2), 6, 100);
  C3 = cm(pingPin3);

//  C1 = constrain(FILT1(cm(pingPin1)), 6, 100);
//  C2 = constrain(FILT2(cm(pingPin2)), 6, 100);
//  C3 = FILT3(cm(pingPin3));
  //  forward(90);
  Serial.print(C1);
  Serial.print('\t');
  Serial.print(C2);
  Serial.print('\t');
  Serial.println(C3);
//  backward(70);
//  backward_err();
//  //  pid();
//  if (C1 > 35 and C2 > 35) {
//    servo.write(kservo);
//    forward(90);
//  }
//  else {
//    pid();
////    forward(90);
//  }
}
int cm(int a) {
  pinMode(a, OUTPUT);
  digitalWrite(a, LOW);
  delayMicroseconds(5);
  digitalWrite(a, HIGH);
  delayMicroseconds(10);
  digitalWrite(a, LOW);
  pinMode(a, INPUT);
  int duration = pulseIn(a, HIGH);
  int cmet = duration / 58;
  return cmet;
}

void pid() {
  //  if (C1 > 40 and C2 > 40) {
  E = C1 - C2;

  //  Serial.print(C2);
  //  Serial.print(" ");
  //  Serial.print(C1);
  //  Serial.print(" ");
  //  Serial.println(C3);

  P = E;
  I = (I * 0.95) + E;
  D = Eold - E;
  Eold = E;

  PID = P * Kp + I * Ki + D * Kd;
  int A = kservo + PID;
  A = constrain(A, 90, 150);
  servo.write(A);

  EGOR = PID * 0.5;

}

void backward_err() {
  if (abs(pidold - PID) < 10) {
    if (millis() - pidt > 5000) {
      C3 = FILT3(cm(pingPin3));
      servo.write(kservo);
      Trackduino.motor(motor_number1, -60);
      Trackduino.motor(motor_number2,  -60);
      delay(1000);
      if (I < 0) {
        servo.write(90);
        Trackduino.motor(motor_number1, 70);
        Trackduino.motor(motor_number2, 70);
        delay(500);
      }
      else {
        servo.write(150);
        Trackduino.motor(motor_number1, 70);
        Trackduino.motor(motor_number2, 70);
        delay(500);
      }
      pidt = millis();
    }
  }
  else {
    pidt = millis();
  }
}

void backward(int s) {
  //  Serial.println(C3);
  if (C3 < 30 ) {
    while ((C3 < 40 or C3 > 300) or ((C1 < 5 or C1 > 300)  and C3 < 20) or ((C2 < 5 or C2 > 300) and C3 < 20)) {
      C3 = FILT3(cm(pingPin3));
      servo.write(kservo);
      Trackduino.motor(motor_number1, -s + 30);
      Trackduino.motor(motor_number2, -s + 30);
    }
    if (I < 0) {
      servo.write(90);
      Trackduino.motor(motor_number1, s);
      Trackduino.motor(motor_number2, s);
      delay(500);
    }
    else {
      servo.write(150);
      Trackduino.motor(motor_number1, s);
      Trackduino.motor(motor_number2, s);
      delay(500);
    }
  }
}
void forward(int s) {
  int T = constrain(C3, 0, 80);
  T += (constrain(C1, 0, 80) + constrain(C2, 0, 80)) * 0.8;
  T /= 2;
  T = map(T, 20, 80, 0, s);
  Trackduino.motor(motor_number1, T);
  Trackduino.motor(motor_number2, T);
  //  Serial.print(T+EGOR);
  //  Serial.print('\t');
  //  Serial.println(T-EGOR);
}

float FILT1(float newVal) {
  valArray1[idx1] = newVal;
  if (++idx1 >= NUM_READ) idx1 = 0;
  float average = 0;
  for (int i = 0; i < NUM_READ; i++) {
    average += valArray1[i];
  }
  return (float)average / NUM_READ;
}

float FILT2(float newVal) {
  valArray2[idx2] = newVal;
  if (++idx2 >= NUM_READ) idx2 = 0;
  float average = 0;
  for (int i = 0; i < NUM_READ; i++) {
    average += valArray2[i];
  }
  return (float)average / NUM_READ;
}

float FILT3(float newVal) {
  valArray3[idx3] = newVal;
  if (++idx3 >= NUM_READ) idx3 = 0;
  float average = 0;
  for (int i = 0; i < NUM_READ; i++) {
    average += valArray3[i];
  }
  return (float)average / NUM_READ;
}
