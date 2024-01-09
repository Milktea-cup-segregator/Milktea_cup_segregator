//判断是否工作
bool flag = false;


// 压电传感器相关
int sensorPin = A0;   // select the input pin for the potentiometer
// int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
int value = 0;


// 丝杆电机驱动定义
#define IN1 3  //定义IN1为3口
#define IN2 4  //定义IN2为4口
#define  ENA  9 //定义ENA为9口
// 旋转速度
const int cupClampSpeed = 50;


// 滑台设定
#include <Stepper.h>
const int stepsPerRevolution = 600;  // change this to fit the number of steps per revolution
// for your motor
// initialize the stepper library on pins 10 through 13:
Stepper myStepper(stepsPerRevolution, 10, 11, 12, 13);
// 第一次向下移动距离
const int down1=2000;
// 第二次向下移动距离
const int down2=50;
// 复位
const int up=down1+down2;

// 杯身旋转设定
#include <Servo.h>
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
int inipos =8;

// 吸管舵机设定
Servo myservo2;
int inipos2 = 5;

// 吸管旋转舵机设定
Servo myservo3;
int inipos3 = 0;

// 滑台旋转舵机设定
Servo myservo4;
int inipos4 = 87;

// 
void setup() {
  Serial.begin(9600);
  // 传感器和丝杆电机设定
    pinMode(IN1,OUTPUT);
    pinMode(IN2,OUTPUT);
    pinMode(ENA,OUTPUT);

  // 滑台设定
  // set the speed at 60 rpm:
  // max 3000
    myStepper.setSpeed(51200);
  // initialize the serial port:

  // 杯身旋转设定
    myservo.attach(7);  // attaches the servo on pin 9 to the servo object
    myservo.write(inipos); //设定初始角度
    delay(3000); 
  // 吸管夹爪设定
    myservo2.attach(9);  // attaches the servo on pin 9 to the servo object
    myservo2.write(inipos2); // 设定初始角度
    delay(3000);
  //吸管夹爪旋转设定
    myservo3.attach(6);  // attaches the servo on pin 9 to the servo object
    myservo3.write(inipos3); // 设定初始角度
    delay(3000);
  // 滑台旋转舵机设定
    myservo4.attach(8);  // attaches the servo on pin 9 to the servo object
    myservo4.write(inipos4); // 设定初始角度
    delay(3000);
}


// 杯身夹紧
void cupClamp(){
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  delay(100);
  value = (1023-sensorValue)*510/sensorValue;

  // 杯身夹紧
  while(value>=1&&value<10){
    digitalWrite(IN1,LOW);  //控制电机正转
    digitalWrite(IN2,HIGH);
    analogWrite(ENA,cupClampSpeed);   //控制电机转速
  }
  // 停下电机
  digitalWrite(IN1,LOW); //控制电机停下
  digitalWrite(IN2,LOW); 
  analogWrite(ENA,cupClampSpeed); 
}

void cupLoose(){
    // 松开夹爪
    // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  delay(100);
  value = (1023-sensorValue)*510/sensorValue;

  while(value>0){
    digitalWrite(IN1,HIGH);  //控制电机正转
    digitalWrite(IN2,LOW);
    analogWrite(ENA,cupClampSpeed);   //控制电机转速
  }
  // 停下电机
  digitalWrite(IN1,LOW); //控制电机停下
  digitalWrite(IN2,LOW); 
  analogWrite(ENA,cupClampSpeed); 
}


void straw_whole(){
  myservo3.write(130);
  myservo2.write(5);
  Serial.println("吸管夹已归位");
  Serial.println("奶茶杯已进入");
  for (inipos3 = 130; inipos3 >= 60; inipos3 -= 1) {
    myservo3.write(inipos3);
    delay(35); 					
  }
  delay(3000);
  Serial.println("吸管夹爪就位");
  delay(3000);

  Serial.println("合上吸管夹爪");
  for (inipos2 = 5; inipos2 <= 65; inipos2 += 1) {       //pos+=1等价于pos=pos+1
    myservo2.write(inipos2);
    delay(30);					
  }
  Serial.println("已夹住吸管");
  delay(3000);

  Serial.println("移开吸管");
  for (inipos3 = 60; inipos3 <= 130; inipos3 += 1) {       //pos+=1等价于pos=pos+1
      myservo3.write(inipos3);
      delay(35);					
  }

  Serial.println("打开吸管夹爪");
  for (inipos2 = 60; inipos2 >= 5; inipos2 -= 1) {
    myservo2.write(inipos2);
    delay(30); 					
  }
  Serial.println("已放开吸管");
  delay(3000);

  Serial.println("夹吸管过程结束");
  delay(3000);
}

void beishenxuanzhuan_huatai(){
  Serial.println("移开");
  for (inipos4 = 90; inipos4 >= 35; inipos4 -= 1) {
    myservo4.write(inipos4);
    Serial.println(inipos4);
    delay(40); 					
  }
  delay(30);
   Serial.println("移回");
  for (inipos4 = 35; inipos4 <= 90; inipos4 += 1) {       //pos+=1等价于pos=pos+1
    myservo4.write(inipos4);
    Serial.println(inipos4);
    delay(40);					
  }
  delay(30);
}

void beishenxuanzhuan_beishen(){
   Serial.println("倾倒");
  for (inipos = 0; inipos <= 90; inipos += 1) {       //pos+=1等价于pos=pos+1
    myservo.write(inipos);
    Serial.println(inipos);
    delay(40);					
  }
  delay(30);
  Serial.println("回");
  for (inipos = 90; inipos >= 0; inipos -= 1) {
    myservo.write(inipos);
    Serial.println(inipos);
    delay(40); 					
  }
  delay(30);
}

void wholeMotion(){
  //原位
  Serial.println("启动");
  myservo.write(inipos);
  myservo4.write(inipos4);
  //感知奶茶杯已放入

  Serial.println("奶茶杯已进入");

  //夹紧杯身

  // 滑台向下移动，预留吸管位置
  Serial.println("滑台向下移动，预留吸管位置");
  for(int i=0;i<5;i++){
    myStepper.step(-stepsPerRevolution*down1);
    // delay(50);
  }

  // 吸管旋转至滑台位置
  
  for (inipos3 = 0; inipos3 <= 50; inipos3 += 1) {       //pos+=1等价于pos=pos+1
      myservo3.write(inipos3);
      delay(35);					
  }
  delay(1000);
  Serial.println("吸管夹爪就位");
  

  // 滑台向上移动
  Serial.println("滑台向上移动");
  for(int i=0;i<3;i++){
    myStepper.step(stepsPerRevolution*down1);
    // delay(50);
  }

 // 吸管夹紧
  Serial.println("合上吸管夹爪");
  for (inipos2 = 5; inipos2 <= 55; inipos2 += 1) {       //pos+=1等价于pos=pos+1
    myservo2.write(inipos2);
    delay(30);					
  }
  delay(1000);
  Serial.println("已夹住吸管");
  
  // 滑台向下移动，拔吸管
  Serial.println("滑台向下移动，拔吸管");
  for(int i=0;i<12;i++){
    myStepper.step(-stepsPerRevolution*down1);
    // delay(50);
  }

  // 吸管旋转
  Serial.println("移开吸管");
  for (inipos3 = 55; inipos3 >= 0; inipos3 -= 1) {
    myservo3.write(inipos3);
    delay(35); 					
  }
  delay(1000);

  // 松开吸管
  Serial.println("打开吸管夹爪");
  for (inipos2 = 50; inipos2 >= 5; inipos2 -= 1) {
    myservo2.write(inipos2);
    delay(30); 					
  }
  Serial.println("已放开吸管");
  delay(1000);

  // 滑台向下移动，撬杯盖
  Serial.println("滑台向下移动，撬杯盖");
  for(int i=0;i<4;i++){
    myStepper.step(-stepsPerRevolution*down1);
    // delay(50);
  }

  // 滑台旋转90
  Serial.println("移开");
  for (inipos4 = 87; inipos4 >= 35; inipos4 -= 1) {
    myservo4.write(inipos4);
    // Serial.println(inipos4);
    delay(40); 					
  }
  delay(1000);

  // 夹爪旋转90
  Serial.println("倾倒");
  for (inipos = 8; inipos <= 110; inipos += 1) {       //pos+=1等价于pos=pos+1
    myservo.write(inipos);
    // Serial.println(inipos);
    delay(40);					
  }
  delay(1000);

  // 滑台回旋
  Serial.println("移回");
  for (inipos4 = 35; inipos4 <= 60; inipos4 += 1) {       //pos+=1等价于pos=pos+1
    myservo4.write(inipos4);
    // Serial.println(inipos4);
    delay(40);					
  }
  delay(1000);
  for (inipos = 110; inipos <= 140; inipos += 1) {       //pos+=1等价于pos=pos+1
    myservo.write(inipos);
    // Serial.println(inipos);
    delay(40);					
  }
  delay(1000);
   //松开杯身


  Serial.println("摆正杯身");
  for (inipos = 140; inipos >= 8; inipos -= 1) {       //pos+=1等价于pos=pos+1
    myservo.write(inipos);
    // Serial.println(inipos);
    delay(40);					
  }
  delay(1000);

 
 //杯身夹爪复位
  for (inipos4 = 60; inipos4 <= 87; inipos4 += 1) {       //pos+=1等价于pos=pos+1
    myservo4.write(inipos4);
    // Serial.println(inipos4);
    delay(40);					
  }
  delay(1000);

  // 滑台向上移动，预留吸管位置
  Serial.println("滑台向上移动，预留吸管位置");
  for(int i=0;i<17;i++){
    myStepper.step(stepsPerRevolution*down1);
    // delay(50);
  }

  delay(5000);
  // Serial.println("奶茶杯已进入");
  // delay(1000);
}

void resetting(){
  // 复位
  // // 吸管回旋

  // myservo3.write(inipos3);
  // delay(3000);

  // 夹爪回旋
  // myservo4.write(inipos4);
  // delay(3000);

  // 滑台上移
  // myStepper.step(stepsPerRevolution*up);
  // delay(500);
  for(int i=0;i<40;i++){
    myStepper.step(stepsPerRevolution*down1);
    delay(200);
  }
}


void loop() {
  // for(int i=0;i<4;i++){
  //   myStepper.step(-stepsPerRevolution*down1);
  //   // delay(200);
  // }
  // for(int i=0;i<2;i++){
  //   myStepper.step(stepsPerRevolution*down1);
    // delay(200);
  // }
  // for(int i=0;i<10;i++){
  //   myStepper.step(-stepsPerRevolution*down1);
  //   delay(200);
  // }
  // for(int i=0;i<3;i++){
  //   myStepper.step(-stepsPerRevolution*down1);
  //   delay(200);
  // }
  // delay(5000);
  //全流程
  wholeMotion();
  delay(5000);
  // // 复位
  // resetting();
}
