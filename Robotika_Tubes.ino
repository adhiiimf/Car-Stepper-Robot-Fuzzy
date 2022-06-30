/*
Title: Car Prototype with Fuzzy
Develop by: - Adhi as Software and Daffa as Hardware
*/

//PIN for Stepper 1
#define stepperPIN_1 2
#define stepperPIN_2 3
#define stepperPIN_3 4
#define stepperPIN_4 5

//PIN for Stepper 2
#define stepperPIN_5 6
#define stepperPIN_6 7
#define stepperPIN_7 8
#define stepperPIN_8 9

#include <Servo.h>

Servo myServo;

float stepperDelay=0;
int angle = 90;

#define trig 11
#define echo 12

float jarak,stepper_delay,out;

struct fuzzy{
  float dekat,sedang,jauh;
}stepper,jaraks;

float rule_1, rule_2A, rule_2B, rule_3;
float cm;

unsigned char jarakDekat(){
  if (jarak <= 15){
    jaraks.dekat =1;
  }
  else if (jarak >=15 && jarak <=35){
    jaraks.dekat=(35-jarak)/20;
  }
  else if (jarak >= 35){
    jaraks.dekat =0;
  }
  return jaraks.dekat;
}

unsigned char jarakSedang(){
  if (jarak <= 15){
    jaraks.sedang =0;
  }
  else if (jarak >=15 && jarak <=35){
    jaraks.sedang=(jarak-15)/20;
  }
  else if (jarak >=35 && jarak <=45){
    jaraks.sedang=(45-jarak)/10;
  }
  else if (jarak >= 45){
    jaraks.sedang =0;
  }
  return jaraks.sedang;
}

unsigned char jarakJauh(){
  if (jarak <= 35){
    jaraks.jauh =0;
  }
  else if (jarak >=35 && jarak <=45){
    jaraks.jauh=(jarak-35)/10;
  }
  else if (jarak >= 45){
    jaraks.jauh =1;
  }
  return jaraks.jauh;
}

unsigned char stepperDekat(){
  if (stepper_delay <= 5){
    stepper.dekat =1;
  }
  else if (stepper_delay >=5 && stepper_delay <=7){
    stepper.dekat=(7-stepper_delay)/2;
  }
  else if (stepper_delay >= 7){
    stepper.dekat =0;
  }
  return stepper.dekat;
}

unsigned char stepperSedang(){
  if (stepper_delay <= 2){
    stepper.sedang =0;
  }
  else if (stepper_delay >=2 && stepper_delay <=4){
    stepper.sedang=(stepper_delay-2)/2;
  }
  else if (stepper_delay >=4 && stepper_delay <=6){
    stepper.sedang=(6-stepper_delay)/2;
  }
  else if (stepper_delay >= 6){
    stepper.sedang =0;
  }
  return stepper.dekat;
}

unsigned char stepperJauh(){
  if (stepper_delay <= 4){
    stepper.jauh =0;
  }
  else if (stepper_delay >=4 && stepper_delay <=6){
    stepper.jauh =(stepper_delay-4)/2;
  }
  else if (stepper_delay >= 6){
   stepper.jauh =1;
  }
  return stepper.jauh;
}

void fuzzyList(){
  jarakDekat();
  jarakSedang();
  jarakJauh();
  stepperDekat();
  stepperSedang();
  stepperJauh();
}

long frontDistance(){
  digitalWrite(trig, LOW);
//  delayMicroseconds(4);
  digitalWrite(trig, HIGH);
//  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long t = pulseIn(echo, HIGH);
  jarak = t/29/2;
  return jarak;
}

void fuzzyRule(){
  forwardStepper();
  frontDistance();
  fuzzyList();
  //IF Jarak dekat, then Stepper Lambat dan degree 140
  rule_1 = 4+(jaraks.dekat*2);
  //IF Jarak sedang, then Stepper Sedang dan degree 120
  rule_2A = 2+(jaraks.sedang*2);
  rule_2B = 6-(jaraks.sedang*2);
  //IF Jarak jauh, then Stepper Cepat dan degree 90
  rule_3 = 4-(jaraks.jauh*2);
  out = ((rule_1*jaraks.dekat) + (rule_2A*jaraks.sedang) + (rule_2B*jaraks.sedang) + (rule_3*jaraks.jauh)) / (jaraks.jauh+jaraks.sedang+jaraks.sedang+jaraks.dekat);
  float temp=rule_3;
  rule_3=rule_1;
  rule_1=temp;
  if(out<=rule_1){
     myServo.write(90);
     delay(10);
     forwardStepper();
  }else if(out>rule_2A && out<rule_2B){
     myServo.write(105);
     delay(10);
     forwardStepper();
  }else if(out>=rule_3){
     myServo.write(135);
     delay(10);
     forwardStepper();
  }
  stepperDelay = out;
}

void setup() {
  //Init PIN for Stepper
  pinMode(stepperPIN_1, OUTPUT);
  pinMode(stepperPIN_2, OUTPUT);
  pinMode(stepperPIN_3, OUTPUT);
  pinMode(stepperPIN_4, OUTPUT);
  pinMode(stepperPIN_5, OUTPUT);
  pinMode(stepperPIN_6, OUTPUT);
  pinMode(stepperPIN_7, OUTPUT);
  pinMode(stepperPIN_8, OUTPUT);
  myServo.attach(10);
  myServo.write(90);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  Serial.begin(9600);
}

void loop() {
  fuzzyRule();
  Serial.println(jarak);
  Serial.println(out);
  Serial.print("dekat : ");
  Serial.print(jaraks.dekat);
  Serial.print("t");
  Serial.print("sedang : ");
  Serial.print(jaraks.sedang);
  Serial.print("t");
  Serial.print("jauh : ");
  Serial.println(jaraks.jauh);
  Serial.print("Rule1 : ");
  Serial.println(rule_1);
  Serial.print("Rule2a : ");
  Serial.println(rule_2A);
  Serial.print("Rule2b : ");
  Serial.println(rule_2B);
  Serial.print("Rule3 : ");
  Serial.println(rule_3);
  Serial.print("Hasil Fuzzy: ");
  Serial.println(out);
  Serial.println("END");
  delay(1000);
}

void forwardStepper(){
  if(out<=rule_1){
    digitalWrite(stepperPIN_1,HIGH);
    digitalWrite(stepperPIN_2,LOW);
    digitalWrite(stepperPIN_3,LOW);
    digitalWrite(stepperPIN_4,LOW);
    digitalWrite(stepperPIN_5,HIGH);
    digitalWrite(stepperPIN_6,LOW);
    digitalWrite(stepperPIN_7,LOW);
    digitalWrite(stepperPIN_8,LOW);
    delay(stepperDelay);
    digitalWrite(stepperPIN_1,LOW);
    digitalWrite(stepperPIN_2,HIGH);
    digitalWrite(stepperPIN_3,LOW);
    digitalWrite(stepperPIN_4,LOW);
    digitalWrite(stepperPIN_5,LOW);
    digitalWrite(stepperPIN_6,HIGH);
    digitalWrite(stepperPIN_7,LOW);
    digitalWrite(stepperPIN_8,LOW);
    delay(stepperDelay);
    digitalWrite(stepperPIN_1,LOW);
    digitalWrite(stepperPIN_2,LOW);
    digitalWrite(stepperPIN_3,HIGH);
    digitalWrite(stepperPIN_4,LOW);
    digitalWrite(stepperPIN_5,LOW);
    digitalWrite(stepperPIN_6,LOW);
    digitalWrite(stepperPIN_7,HIGH);
    digitalWrite(stepperPIN_8,LOW);
    delay(stepperDelay);
    digitalWrite(stepperPIN_1,LOW);
    digitalWrite(stepperPIN_2,LOW);
    digitalWrite(stepperPIN_3,LOW);
    digitalWrite(stepperPIN_4,HIGH);
    digitalWrite(stepperPIN_5,LOW);
    digitalWrite(stepperPIN_6,LOW);
    digitalWrite(stepperPIN_7,LOW);
    digitalWrite(stepperPIN_8,HIGH);
    delay(stepperDelay);
  }else if(out>=rule_2A && out<rule_2B){
    digitalWrite(stepperPIN_1,HIGH);
    digitalWrite(stepperPIN_2,LOW);
    digitalWrite(stepperPIN_3,LOW);
    digitalWrite(stepperPIN_4,LOW);
    digitalWrite(stepperPIN_5,HIGH);
    digitalWrite(stepperPIN_6,LOW);
    digitalWrite(stepperPIN_7,LOW);
    digitalWrite(stepperPIN_8,LOW);
    delay(stepperDelay);
    digitalWrite(stepperPIN_1,LOW);
    digitalWrite(stepperPIN_2,HIGH);
    digitalWrite(stepperPIN_3,LOW);
    digitalWrite(stepperPIN_4,LOW);
    digitalWrite(stepperPIN_5,LOW);
    digitalWrite(stepperPIN_6,HIGH);
    digitalWrite(stepperPIN_7,LOW);
    digitalWrite(stepperPIN_8,LOW);
    delay(stepperDelay);
    digitalWrite(stepperPIN_1,LOW);
    digitalWrite(stepperPIN_2,LOW);
    digitalWrite(stepperPIN_3,HIGH);
    digitalWrite(stepperPIN_4,LOW);
    digitalWrite(stepperPIN_5,LOW);
    digitalWrite(stepperPIN_6,LOW);
    digitalWrite(stepperPIN_7,HIGH);
    digitalWrite(stepperPIN_8,LOW);
    delay(stepperDelay);
    digitalWrite(stepperPIN_1,LOW);
    digitalWrite(stepperPIN_2,LOW);
    digitalWrite(stepperPIN_3,LOW);
    digitalWrite(stepperPIN_4,HIGH);
    digitalWrite(stepperPIN_5,LOW);
    digitalWrite(stepperPIN_6,LOW);
    digitalWrite(stepperPIN_7,LOW);
    digitalWrite(stepperPIN_8,HIGH);
    delay(stepperDelay);
  }else if(out>=rule_3){
    digitalWrite(stepperPIN_1,HIGH);
    digitalWrite(stepperPIN_2,LOW);
    digitalWrite(stepperPIN_3,LOW);
    digitalWrite(stepperPIN_4,LOW);
    digitalWrite(stepperPIN_5,LOW);
    digitalWrite(stepperPIN_6,LOW);
    digitalWrite(stepperPIN_7,LOW);
    digitalWrite(stepperPIN_8,HIGH);
    delay(stepperDelay);
    digitalWrite(stepperPIN_1,LOW);
    digitalWrite(stepperPIN_2,HIGH);
    digitalWrite(stepperPIN_3,LOW);
    digitalWrite(stepperPIN_4,LOW);
    digitalWrite(stepperPIN_5,LOW);
    digitalWrite(stepperPIN_6,LOW);
    digitalWrite(stepperPIN_7,HIGH);
    digitalWrite(stepperPIN_8,LOW);
    delay(stepperDelay);
    digitalWrite(stepperPIN_1,LOW);
    digitalWrite(stepperPIN_2,LOW);
    digitalWrite(stepperPIN_3,HIGH);
    digitalWrite(stepperPIN_4,LOW);
    digitalWrite(stepperPIN_5,LOW);
    digitalWrite(stepperPIN_6,HIGH);
    digitalWrite(stepperPIN_7,LOW);
    digitalWrite(stepperPIN_8,LOW);
    delay(stepperDelay);
    digitalWrite(stepperPIN_1,LOW);
    digitalWrite(stepperPIN_2,LOW);
    digitalWrite(stepperPIN_3,LOW);
    digitalWrite(stepperPIN_4,HIGH);
    digitalWrite(stepperPIN_5,HIGH);
    digitalWrite(stepperPIN_6,LOW);
    digitalWrite(stepperPIN_7,LOW);
    digitalWrite(stepperPIN_8,LOW);
    delay(stepperDelay);
  }
}
