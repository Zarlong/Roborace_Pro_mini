#include <Trackduino.h>
#include <Servo.h>

Servo servo;

int pingPin1 = IN2;
int pingPin2 = IN7;
int pingPin3 = IN3;
int IK = IN4;

int C1 = 0;
int C2 = 0;
int C3 = 0;
int kservo = 120;
const int motor_number1 = 1;
const int motor_number2 = 2;

float Kp = 0.6;
float Ki = 0.015;
float Kd = 0;

int L = 30;

int F = 0;

int E = 0;
int Eold = 0;

int PID = 0;
int P = 0;
long long I = 0;
int D = 0;

const int NUM_READ = 4;

unsigned long long pidt = 0;
int pidold = 0;

byte idx1 = 0;              // индекс
float valArray1[NUM_READ];  // массив
byte idx2 = 0;              // индекс
float valArray2[NUM_READ];  // массив
byte idx3 = 0;              // индекс
float valArray3[NUM_READ];  // массив

// float EGOR = 0;

void setup() {
  Serial.begin(9600);
  Trackduino.setup();
  servo.attach(OUT1);
  for (int i = 0; i < NUM_READ; ++i) {
    C1 = constrain(findMedianN1(cm(pingPin1)), 4, 200);
    C2 = constrain(findMedianN2(cm(pingPin2)), 4, 200);
    C3 = constrain(findMedianN3(cm(pingPin3)), 4, 200);
  }
  //  pinMode(IK, INPUT);
}
void loop() {
  // delay(1000)
  // C1 = FILT1(constrain(cm(pingPin1), 2, 200));
  // C2 = FILT2(constrain(cm(pingPin2), 2, 200));
  // // C1 = cm(pingPin1);
  // // C2 = cm(pingPin2);
  // C3 = FILT3(constrain(cm(pingPin3), 2, 100));

  //  forward(90);
  // Serial.print(C1);
  // Serial.print('\t');
  // Serial.print(C3);
  // Serial.print('\t');
  // Serial.println(C2);
  //  backward(70);
  //  backward_err();
  //  //  pid();
  //  if (C1 > 30 and C2 > 30) {
  //    servo.write(kservo);
  //    forward(90);
  //  }
  //  else {
  //    pid();
  if (C1 > L and C2 > L) {
    servo.write(kservo);
    forward(95);
  } else {
    pid();
    forward(80);
  }
  // forward(80);
  backward(60);
  backward_err();

  C1 = abs(constrain(findMedianN1(cm(pingPin1)), 4, 200));
  C2 = abs(constrain(findMedianN2(cm(pingPin2)), 4, 200));
  C3 = abs(constrain(findMedianN3(cm(pingPin3)), 4, 200));
  //  }
}
int cm(int a) {
  pinMode(a, OUTPUT);
  digitalWrite(a, LOW);
  delayMicroseconds(2);
  digitalWrite(a, HIGH);
  delayMicroseconds(10);
  digitalWrite(a, LOW);
  pinMode(a, INPUT);
  int duration = pulseIn(a, HIGH);
  int cmet = duration / 58;
  return cmet;
}

void pid() {
  // if (C1 > L) {
  //   F = 1;
  //   E = C2 - L;
  //   Trackduino.RGB_off();
  //   Trackduino.RGB_red();
  // } else if (C2 > L) {
  //   F = 2;
  //   E = C1 - L;
  //   Trackduino.RGB_off();
  //   Trackduino.RGB_green();
  // } else if (C2 > L and C1 > L) {
  //   F = 3;
  //   servo.write(kservo);
  //   forward(70);
  // } else if (C1 < L and C2 < L){
  //   F = 4;
  //   E = C1 - C2;
  //   Trackduino.RGB_off();
  //   Trackduino.RGB_blue();
  // }

  E = C2 - C1;
  P = E;
  I = (I * 0.90) + E;
  D = Eold - E;
  Eold = E;

  PID = P * Kp + I * Ki + D * Kd;
  int A = kservo + PID;
  A = constrain(A, 95, 145);
  servo.write(A);
  // EGOR = PID * 0.5;
  // Serial.print(C1);
  // Serial.print('\t');
  // Serial.print(C2);
  // Serial.print('\t');
  // Serial.print(F);
  // Serial.print('\t');
  // Serial.println(PID);
  // pidold = PID;
}

void backward_err() {
  if (abs(pidold - PID) < 5) {
    if (millis() - pidt > 5000) {
      Trackduino.RGB_off();
      Trackduino.RGB_yellow();
      C3 = abs(findMedianN3(cm(pingPin3)));  // ФИЛЬТР
      servo.write(kservo);
      Trackduino.motor(motor_number1, -60);
      Trackduino.motor(motor_number2, -60);
      delay(1000);
      if (I < 0) {
        servo.write(90);
        Trackduino.motor(motor_number1, 70);
        Trackduino.motor(motor_number2, 70);
        delay(500);
      } else {
        servo.write(150);
        Trackduino.motor(motor_number1, 70);
        Trackduino.motor(motor_number2, 70);
        delay(500);
      }
      pidt = millis();
    }
  } else {
    pidt = millis();
  }
}

void backward(int s) {
  //  Serial.println(C3);
  if (C3 < 30) {
    while (C3 < 40 or C3 > 300) {            //) or ((C1 < 5 or C1 > 300) and C3 < 20) or ((C2 < 5 or C2 > 300) and C3 < 20)
      C3 = abs(findMedianN3(cm(pingPin3)));  //ФИЛЬТР
      servo.write(kservo);
      Trackduino.motor(motor_number1, -s + 30);
      Trackduino.motor(motor_number2, -s + 30);
    }
    if (I < 0) {
      servo.write(95);
      Trackduino.motor(motor_number1, s);
      Trackduino.motor(motor_number2, s);
      delay(400);
    } else {
      servo.write(145);
      Trackduino.motor(motor_number1, s);
      Trackduino.motor(motor_number2, s);
      delay(400);
    }
  }
}
void forward(int s) {
  int T = -constrain(C3, 0, 80) * 0.5;
  T += (abs(constrain(findMedianN1(cm(pingPin1)), 4, 200)) + abs(constrain(findMedianN2(cm(pingPin2)), 4, 200))) * 0.5;
  T /= 2;
  T = map(T, 20, 80, -30, s);  // Поменял на -20 надо оттестить
  Trackduino.motor(motor_number1, s + T);
  Trackduino.motor(motor_number2, s + T);
  Serial.println(s - T);
  //  Serial.print(T+EGOR);
  //  Serial.print('\t');
  //  Serial.println(T-EGOR);
}

// float FILT1(float newVal) {
//   valArray1[idx1] = newVal;
//   if (++idx1 >= NUM_READ) idx1 = 0;
//   float average = 0;
//   for (int i = 0; i < NUM_READ; i++) {
//     average += valArray1[i];
//   }
//   return (float)average / NUM_READ;
// }
int findMedianN1(int newVal1) {
  static int buffer1[NUM_READ];  // статический буфер
  static byte count1 = 0;        // счётчик
  buffer1[count1] = newVal1;
  if (++count1 >= NUM_READ) count1 = 0;  // перемотка буфера

  int buf1[NUM_READ];  // локальный буфер для медианы
  for (byte i = 0; i < NUM_READ; i++) buf1[i] = buffer1[i];
  for (int i = 0; i <= (int)((NUM_READ / 2) + 1); i++) {  // пузырьковая сортировка массива (можно использовать любую)
    for (int m = 0; m < NUM_READ - i - 1; m++) {
      if (buf1[m] > buf1[m + 1]) {
        int buff1 = buf1[m];
        buf1[m] = buf1[m + 1];
        buf1[m + 1] = buff1;
      }
    }
  }
  return buf1[2];
}
int findMedianN2(int newVal2) {
  static int buffer2[NUM_READ];  // статический буфер
  static byte count2 = 0;        // счётчик
  buffer2[count2] = newVal2;
  if (++count2 >= NUM_READ) count2 = 0;  // перемотка буфера

  int buf2[NUM_READ];  // локальный буфер для медианы
  for (byte i = 0; i < NUM_READ; i++) buf2[i] = buffer2[i];
  for (int i = 0; i <= (int)((NUM_READ / 2) + 1); i++) {  // пузырьковая сортировка массива (можно использовать любую)
    for (int m = 0; m < NUM_READ - i - 1; m++) {
      if (buf2[m] > buf2[m + 1]) {
        int buff2 = buf2[m];
        buf2[m] = buf2[m + 1];
        buf2[m + 1] = buff2;
      }
    }
  }
  return buf2[2];
}
int findMedianN3(int newVal3) {
  static int buffer3[NUM_READ];  // статический буфер
  static byte count3 = 0;        // счётчик
  buffer3[count3] = newVal3;
  if (++count3 >= NUM_READ) count3 = 0;  // перемотка буфера

  int buf3[NUM_READ];  // локальный буфер для медианы
  for (byte i = 0; i < NUM_READ; i++) buf3[i] = buffer3[i];
  for (int i = 0; i <= (int)((NUM_READ / 2) + 1); i++) {  // пузырьковая сортировка массива (можно использовать любую)
    for (int m = 0; m < NUM_READ - i - 1; m++) {
      if (buf3[m] > buf3[m + 1]) {
        int buff3 = buf3[m];
        buf3[m] = buf3[m + 1];
        buf3[m + 1] = buff3;
      }
    }
  }
  return buf3[2];
}