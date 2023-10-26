#include <Trackduino.h>
#include <Servo.h>

Servo servo;

int pingPin1 = IN1;
int pingPin2 = IN2;
int pingPin3 = IN3;

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
float Ki = 0.015;
float Kd = 0.58;

int E = 0;
int Eold = 0;

int PID = 0;
int P = 0;
long long I = 0;
int D = 0;

const int NUM_READ = 3;

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
}
void loop() {
  C1 = constrain(FILT1(cm(pingPin1)), 10, 100);
  C2 = constrain(FILT2(cm(pingPin2)), 10, 100);
  C3 = FILT3(cm(pingPin3));
//  forward(90);
  backward(70);
//  pid();
  if(C1 > 30 and C2 > 30){
    servo.write(kservo);
    forward(90);
  }
  else{
    pid();
  }
}
int cm(int a) {
  pinMode(a, OUTPUT);
  digitalWrite(a, LOW);
  delayMicroseconds(2);
  digitalWrite(a, HIGH);
  delayMicroseconds(5);
  digitalWrite(a, LOW);
  pinMode(a, INPUT);
  int duration = pulseIn(a, HIGH);
  int cmet = duration / 58;
  return cmet;
}

void pid() {
//  if (C1 > 40 and C2 > 40) {
    E = C1 - C2;

    Serial.print(C2);
    Serial.print(" ");
    Serial.print(C1);
    Serial.print(" ");
    Serial.println(C3);

    P = E;
    I = (I * 0.95) + E;
    D = Eold - E;
    Eold = E;

    PID = P * Kp + I * Ki + D * Kd;
    int A = kservo + PID;
    A = constrain(A, 90, 150);
    servo.write(A);

    EGOR = PID * 1;
}

void backward(int s) {
  if (C3 < 10 or C3 > 400) {
    while (C3 < 25 or C3 > 400) {
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
  int T = constrain(C3, 0, 80);
  T = map(T, 8, 80, 50, s);
  Trackduino.motor(motor_number1, T + EGOR);
  Trackduino.motor(motor_number2, T - EGOR);
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
