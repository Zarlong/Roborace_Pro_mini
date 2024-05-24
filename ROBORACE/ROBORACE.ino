#include <Trackduino.h>
#include <Servo.h>

Servo servo;

int pingPin1 = IN2;
int pingPin2 = IN4;
int pingPin3 = IN3;
// int IK = IN4;

int C1 = 0;
int C2 = 0;
int C3 = 0;

int kservo = 120;
const int motor_number1 = 1;
const int motor_number2 = 3;

float Kp = 0.5;  // 3
float Ki = 0.0;   // 0.015
float Kd = 0.0;   // 0.2

int L = 60;

int F = 0;

int E = 0;
int Eold = 0;

int PID = 0;
int P = 0;
long long I = 0;
int D = 0;

float V = 0;
float distance = 0;
float filtered_val = 0;

const int NUM_READ = 9;

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
  // for (int i = 0; i < NUM_READ; ++i) {
  //   C1 = constrain(cm(pingPin1), 6, 250);
  //   C2 = constrain(cm(pingPin2), 6, 250);
  C3 = constrain(cm(), 6, 250);
  // }
  // servo.write(110);
  // delay(1000);
  // servo.write(145);
  // delay(1000);
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
  // if (C1 > L and C2 > L) {
  //   servo.write(kservo);
  //   forward(80);
  // } else {
  C3 = constrain(cm(), 6, 50);
  pid();
  forward(98);
  // }
  // forward(80);
  backward(60);
  // backward_err();

  // C1 = abs(constrain(cm(pingPin1), 6, 250));
  // C2 = abs(constrain(cm(pingPin2), 6, 250));
  // C3 = abs(constrain(cm(pingPin3), 6, 250));
  // Serial.print(C1);
  // Serial.print('\t');
  // Serial.print(C2);
  // Serial.print('\t');
  // Serial.print(A);
  // Serial.print('\t');
  // Serial.print(findMedianN1(sha1()));
  // Serial.print('\t');
  // Serial.println(findMedianN2(sha2()));
  //  }
}
int cm() {
  pinMode(IN3, OUTPUT);
  digitalWrite(IN3, LOW);
  delayMicroseconds(5);
  digitalWrite(IN3, HIGH);
  delayMicroseconds(10);
  digitalWrite(IN3, LOW);
  pinMode(IN3, INPUT);
  float duration = pulseIn(IN3, HIGH);
  float cmet = duration / 58.2;
  return cmet;
}


void pid() {
  // if (C1 > L and C2 < L) {
  //   F = 1;
  //   E = C2 - L;
  //   Trackduino.RGB_off();
  //   Trackduino.RGB_red();
  // } else if (C2 > L and C1 < L) {
  //   F = 2;
  //   E = L - C1;
  //   Trackduino.RGB_off();
  //   Trackduino.RGB_green();
  // } else if (C2 > L and C1 > L) {
  //   F = 3;
  //   E = 0;
  // } else if (C1 < L and C2 < L){
  //   F = 4;
  //   E = C1 - C2;
  //   Trackduino.RGB_off();
  //   Trackduino.RGB_blue();
  // }
  // C1 = abs(constrain(cm(pingPin1), 6, 250));
  // C2 = abs(constrain(cm(pingPin2), 6, 250));
  // C3 = abs(constrain(cm(pingPin3), 6, 250));

  E = sha1() - sha2() + 30;
  // if (C1 > L and C2 < L) {
  //   F = 1;
  //   E = -50; // СЕ -30 C2 -L
  //   Trackduino.RGB_off();
  //   Trackduino.RGB_red();
  // } else if (C2 > L and C1 < L) {
  //   F = 2;
  //   E = 50; //СЕ 30 L - C1
  //   Trackduino.RGB_off();
  //   Trackduino.RGB_green();
  // } else if (C2 > L and C1 > L) {
  //   F = 3;
  //   E = 0;
  // } else if (C1 < L and C2 < L){
  //   F = 4;
  //   E = C1 - C2;
  //   Trackduino.RGB_off();
  //   Trackduino.RGB_blue();
  // }

  // E = C2 - C1
  P = E;
  // I = (I * 0.93) + E;
  D = Eold - E;
  Eold = E;

  PID = P * Kp + I * Ki + D * Kd;
  int A = kservo - PID;
  A = constrain(A, (kservo - 25), (kservo + 25));
  servo.write(A);
  // EGOR = PID * 0.5;
  // pidold = PID;
}

void backward_err() {
  if (abs(pidold - PID) < 5) {
    if (millis() - pidt > 5000) {
      Trackduino.RGB_off();
      Trackduino.RGB_yellow();
      // C3 = abs(findMedianN3(cm(pingPin3)));  // ФИЛЬТР
      servo.write(kservo);
      Trackduino.motor(motor_number1, -60);
      Trackduino.motor(motor_number2, -60);
      delay(1000);
      if (Eold < 0) {
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
    while (C3 < 40) {  //) or ((C1 < 5 or C1 > 300) and C3 < 20) or ((C2 < 5 or C2 > 300) and C3 < 20)
      // C3 = abs(findMedianN3(cm(pingPin3)));  //ФИЛЬТР
      C3 = constrain(cm(), 6, 50);
      // servo.write(kservo);
      if (Eold > 0) {
        servo.write(kservo - 30);
      } else {
        servo.write(kservo + 30);
      }
      Trackduino.motor(motor_number1, -s);
      Trackduino.motor(motor_number2, -s);
    }
    //   servo.write(95);
    //   Trackduino.motor(motor_number1, s);
    //   Trackduino.motor(motor_number2, s);
    //   delay(400);
    // } else {
    //   servo.write(145);
    //   Trackduino.motor(motor_number1, s);
    //   Trackduino.motor(motor_number2, s);
    //   delay(400);
    // }
  }
}
void forward(int s) {
// <<<<<<< HEAD
//   // if (C1 > 100) {
// =======
//   // if (C1 > L) {
// >>>>>>> ec04900bb6c72cf4fd2c7304f47cdbcfcb2606b3
//   //   // if (Eold > 0)
//   //   // pid();
//   //   servo.write(95);
//   //   // }
// <<<<<<< HEAD
//   // }
//   // if (C2 > 100) {
// =======
//   // } 
//   // if (C2 > L) {
// >>>>>>> ec04900bb6c72cf4fd2c7304f47cdbcfcb2606b3
  //   // if (Eold > 0)
  //   // pid();
  //   servo.write(145);
  // }

  // int T = -constrain(C3, 0, 80) * 0.2;
  // T += (abs(constrain(findMedianN1(cm(pingPin1)), 4, 200)) + abs(constrain(findMedianN2(cm(pingPin2)), 4, 200))) * 0.5;
  // T /= 2;
  // T = map(T, 20, 80, -30, s);  // Поменял на -20 надо оттестить
  Trackduino.motor(motor_number1, s);
  Trackduino.motor(motor_number2, s);
  // Serial.println(s - T);
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
  return buf1[4];
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
  return buf2[4];
}