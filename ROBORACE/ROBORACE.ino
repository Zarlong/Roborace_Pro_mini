//#define L_PWM 3
//#define L_DIR 2
//#define R_PWM 5
//#define R_DIR 4
#include <Trackduino.h>

int pingPin1 = IN1;
int pingPin2 = IN2;
int MAS[9];
int MASS[9];

int C1 = 0, C2 = 0;

#include <Servo.h>
int kservo = 120;
Servo servo;
const int motor_number1 = 1;
const int motor_number2 = 2;

long duration1, inches1, cm1;
long duration2, inches2, cm2;

void setup() {
  //  pinMode(L_PWM, OUTPUT);
  //  pinMode(R_PWM, OUTPUT);
  //  pinMode(L_DIR, OUTPUT);
  //  pinMode(R_DIR, OUTPUT);
  Serial.begin(9600);
  Trackduino.setup();

  servo.attach(OUT1);
}

//void go(int left, int right) {
//  if (left >= 0) {
//    digitalWrite(L_DIR, HIGH);
//    analogWrite(L_PWM, left);
//  } else {
//    digitalWrite(L_DIR, LOW);
//    analogWrite(L_PWM, abs(left));
//  }
//  if (right >= 0) {
//    digitalWrite(R_DIR, HIGH);
//    analogWrite(R_PWM, right);
//  } else {
//    digitalWrite(R_DIR, LOW);
//    analogWrite(R_PWM, abs(right));
//  }
//}

void pid() {
  int E = C1 - C2;//датчик и разность
  for (int i = 0; i < 8; i++) MAS[i] = MAS[i + 1];
  MAS[8] = E;
  for (int i = 0; i < 9; i++) MASS[i] = MAS[i];
  for (int j = 0; j < 9; j++) {
    for (int i = 0; i < 8; i++) {
      if (MASS[i] > MASS[i + 1]) {
        int temp = MASS[i];
        MASS[i] = MASS[i + 1];
        MASS[i + 1] = temp;
      }
    }
  }
  float K = 0.8;
  //  int M1 = 80 + E*K;
  //  int M2 = 80 - E*K;
  int A = kservo + (MASS[4] * K);
  A = constrain(A, 80, 160);
  servo.write(A);
  //  Serial.println(A);
  Serial.print(E);
  Serial.print("\t");
  Serial.println(MASS[4]);
}

void loop() {
  pinMode(pingPin1, OUTPUT);
  digitalWrite(pingPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin1, LOW);
  pinMode(pingPin1, INPUT);
  duration1 = pulseIn(pingPin1, HIGH);
  cm1 = microsecondsToInches(duration1);
  C1 = cm1;
//  float sum = 0;                      // локальная переменная sum
//  for (int i = 0; i < 10; i++)  // согласно количеству усреднений
//    sum += cm1;                  // суммируем значения с любого датчика в переменную sum
//
//  C1 = sum / 10;



  pinMode(pingPin2, OUTPUT);
  digitalWrite(pingPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin2, LOW);
  pinMode(pingPin2, INPUT);
  duration2 = pulseIn(pingPin2, HIGH);
  cm2 = microsecondsToInches(duration2);
  C2 = cm2;
//  sum = 0;                      // локальная переменная sum
//  for (int i = 0; i < 10; i++)  // согласно количеству усреднений
//    sum += cm2;                  // суммируем значения с любого датчика в переменную sum
//
//  C2 = sum / 10;

    Trackduino.motor(motor_number1, 60);
    Trackduino.motor(motor_number2, 60);
  pid();
}

long microsecondsToInches(long microseconds)
{
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}

int median(int newVal) {
  static int buf[3];
  static byte count = 0;
  buf[count] = newVal;
  if (++count >= 3) count = 0;
  return (max(buf[0], buf[1]) == max(buf[1], buf[2])) ? max(buf[0], buf[2]) : max(buf[1], min(buf[0], buf[2]));
}
