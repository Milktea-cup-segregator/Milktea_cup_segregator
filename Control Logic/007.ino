//判断是否工作
bool flag = false;
int x=1;

//判断是否启动
#define IN5 16

// 压电传感器相关
int sensorPin = A0;   // select the input pin for the potentiometer
const int numReadings  = 20;
int readings [numReadings];
int readIndex  = 0;
long total  = 0;
//Variables
int aisVal  = 0;
int a;
bool flagIn=true;
bool flagOut=false;
int k=0;

//继电器
int incomedate = 0;
int relayPin = 15; //继电器引脚


// 丝杆电机驱动定义
#define IN1 3  //定义IN1为3口
#define IN2 4  //定义IN2为4口
#define  ENA  5 //定义ENA为5口
// 旋转速度
const int cupClampSpeed = 50;

//滑台位置确定·开关
#define IN3 14  //定义IN1为14口
const int numReadings1  = 10;
int readings1 [numReadings1];
int readIndex1  = 0;
long total1  = 0;
//Variables

// 滑台设定
#include <Stepper.h>
const int stepsPerRevolution = 400;  // change this to fit the number of steps per revolution
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
  pinMode(IN5,INPUT_PULLUP);
  // 传感器和丝杆电机设定
  // Serial.begin(115200);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(ENA,OUTPUT);
  analogWrite(ENA,60);

  //继电器设定
  pinMode(relayPin, OUTPUT);

  // 滑台设定
  // set the speed at 60 rpm:
  // max 3000
    myStepper.setSpeed(51200);
  // initialize the serial port:

  //滑台位置确定
  pinMode(IN3,INPUT_PULLUP);
  // pinMode(IN4,OUTPUT);

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
  Serial.print("夹紧杯子中");
  for(int i=0;i<200;i++){
    a=smooth();
  }
  while(k<=30){
    Serial.print("a=");
    Serial.println(a);
    Serial.print("k=");
    Serial.println(k);
    Serial.println(smooth());
    if(flagIn==true && smooth()<(a-10)){
      digitalWrite(IN1,LOW);
      digitalWrite(IN2,HIGH);
      analogWrite(ENA,250);
      k=k+1;
      delay(100);
    }
  }
  flagIn=false;
  flagOut=true;
  analogWrite(ENA,0);
}

void cupHold(){
  analogWrite(ENA,250);
}
void cupStop(){
  analogWrite(ENA,0);
}

void cupLoose(){
    // 松开夹爪
  Serial.print("松开杯子中");
  while(k>=5){
    if(flagOut==true){
      Serial.print("a=");
      Serial.println(a);
      Serial.print("k=");
      Serial.println(k);
      Serial.println(smooth());
      digitalWrite(IN2,LOW);
      digitalWrite(IN1,HIGH);
      analogWrite(ENA,250);
      k--;
      delay(100);
    }
  }  
  flagOut=false;
  flagIn=true;
  k=0;
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  analogWrite(ENA,0);
}

void cupback(){
  Serial.print("归位中");
  while(k<=12){
    Serial.print("a=");
    Serial.println(a);
    Serial.print("k=");
    Serial.println(k);
    Serial.println(smooth());
    if(flagIn==true){
      digitalWrite(IN1,LOW);
      digitalWrite(IN2,HIGH);
      analogWrite(ENA,250);
      k=k+1;
      delay(100);
    }
  }
  k=0;
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  analogWrite(ENA,0);
}

void cupback1(){
  Serial.print("归位中");
  while(k<=10){
    Serial.print("a=");
    Serial.println(a);
    Serial.print("k=");
    Serial.println(k);
    Serial.println(smooth());
    if(flagIn==true){
      digitalWrite(IN2,LOW);
      digitalWrite(IN1,HIGH);
      analogWrite(ENA,250);
      k=k+1;
      delay(100);
    }
  }
  k=0;
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  analogWrite(ENA,0);
  delay(100);
}

long smooth() { /* function smooth */
  ////Perform average on sensor readings
  long average;
  // subtract the last reading:
  total = total - readings[readIndex];
  // read the sensor:
  readings[readIndex] = analogRead(sensorPin);
  // add value to total:
  total = total + readings[readIndex];
  // handle index
  readIndex = readIndex + 1;
  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  // calculate the average:
  average = total / numReadings;

  return average;
}

long smooth1() { /* function smooth */
  ////Perform average on sensor readings
  long average1;
  // subtract the last reading:
  total1 = total1 - readings1[readIndex1];
  // read the sensor:
  if(digitalRead(IN3)==LOW){
    readings1[readIndex1] = 1;
  }
  else{
    readings1[readIndex1] = 0;
  }
  // add value to total:
  total1 = total1 + readings1[readIndex1];
  // handle index
  readIndex1 = readIndex1 + 1;
  if (readIndex1 >= numReadings1) {
    readIndex1 = 0;
  }
  // calculate the average:
  average1 = total1 / numReadings1;
  return total1;
}

void wholeMotion(){
  //微动开关就位
  // digitalWrite(IN4,LOW);
  //原位
  Serial.println("启动");
  myservo.write(inipos);
  myservo4.write(inipos4);
  

  Serial.println("移到位置上");
  for (inipos4 = 87; inipos4 >= 80; inipos4 -= 1) {
    myservo4.write(inipos4);
    // Serial.println(inipos4);
    delay(15); 					
  }
  delay(1000);

  //感知奶茶杯已放入
  
  Serial.println("奶茶杯已进入");

  //夹紧杯身
  Serial.println("夹紧杯身");
  cupClamp();

  Serial.println("移开回原位");
  for (inipos4 = 80; inipos4 <= 87; inipos4 += 1) {
    myservo4.write(inipos4);
    // Serial.println(inipos4);
    delay(15); 					
  }
  delay(1000);

  // 滑台向下移动，预留吸管位置
  Serial.println("滑台向下移动，预留吸管位置");
  for(int i=0;i<4;i++){
    myStepper.step(-stepsPerRevolution*down1);
    // delay(50);
  }

  // 吸管旋转至滑台位置
  
  for (inipos3 = 0; inipos3 <= 38; inipos3 += 1) {       //pos+=1等价于pos=pos+1
      myservo3.write(inipos3);
      delay(15);					
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
    delay(15);					
  }
  delay(1000);
  Serial.println("已夹住吸管");
  
  // 滑台向下移动，拔吸管
  Serial.println("滑台向下移动，拔吸管");
  for(int i=0;i<9;i++){
    myStepper.step(-stepsPerRevolution*down1);
    // delay(50);
  }

  // 吸管旋转
  Serial.println("移开吸管");
  for (inipos3 = 38; inipos3 >= 0; inipos3 -= 1) {
    myservo3.write(inipos3);
    delay(15); 					
  }
  delay(1000);

  // 松开吸管
  Serial.println("打开吸管夹爪");
  for (inipos2 = 50; inipos2 >= 5; inipos2 -= 1) {
    myservo2.write(inipos2);
    delay(15); 					
  }
  Serial.println("已放开吸管");
  delay(1000);

  // 开风扇、握紧杯子
  Serial.println("开风扇");
  digitalWrite(relayPin, HIGH);
  cupHold();
  delay(500);

  // 滑台向下移动，撬杯盖
  Serial.println("滑台向下移动，撬杯盖");
  for(int i=0;i<10;i++){
    smooth1();
  }
  while(smooth1()<6){
    myStepper.step(-stepsPerRevolution*1);
    Serial.println(smooth1());
    Serial.println("滑台移动");
  }
  Serial.println("滑台停止向下移动");

  //关风扇、停止握紧杯子
  Serial.println("关风扇");
  digitalWrite(relayPin, LOW);
  cupStop();

  // 滑台旋转90
  Serial.println("移开");
  for (inipos4 = 87; inipos4 >= 20; inipos4 -= 1) {
    myservo4.write(inipos4);
    // Serial.println(inipos4);
    delay(15); 					
  }
  delay(1000);

  // 夹爪旋转90
  Serial.println("倾倒");
  for (inipos = 8; inipos <= 110; inipos += 1) {       //pos+=1等价于pos=pos+1
    myservo.write(inipos);
    // Serial.println(inipos);
    delay(15);					
  }
  delay(3000);

  // 滑台回旋
  Serial.println("移回");
  for (inipos4 = 20; inipos4 <= 60; inipos4 += 1) {       //pos+=1等价于pos=pos+1
    myservo4.write(inipos4);
    // Serial.println(inipos4);
    delay(15);					
  }
  delay(1000);
  for (inipos = 110; inipos <= 140; inipos += 1) {       //pos+=1等价于pos=pos+1
    myservo.write(inipos);
    // Serial.println(inipos);
    delay(15);					
  }
  delay(1000);
   //松开杯身
  Serial.println("松开杯身");
  cupLoose();

  Serial.println("摆正杯身+调整杯口");
  for (inipos = 140; inipos >= 8; inipos -= 1) {       //pos+=1等价于pos=pos+1
    myservo.write(inipos);
    // Serial.println(inipos);
    delay(15);					
  }
  delay(1000);
  cupback();
 
 //杯身夹爪复位
  for (inipos4 = 60; inipos4 <= 87; inipos4 += 1) {       //pos+=1等价于pos=pos+1
    myservo4.write(inipos4);
    // Serial.println(inipos4);
    delay(15);					
  }
  delay(1000);


  // 滑台向上移动，预留吸管位置
  myStepper.step(stepsPerRevolution*20);
  Serial.println("滑台向上移动，预留吸管位置");
  for(int i=0;i<10;i++){
    smooth1();
  }
  while(smooth1()<6){
    myStepper.step(stepsPerRevolution*1);
    Serial.println(smooth1());
    Serial.println("滑台移动");
  }
  Serial.println("滑台停止向上移动");
  
  Serial.print(x);
  Serial.println("个奶茶杯已分离完毕AvA");
  x++;
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
  while(smooth1()<6){
    myStepper.step(stepsPerRevolution*1);
    Serial.println(smooth1());
    Serial.println("滑台移动");
  }
}

void loop() {
  //开关控制
  if(digitalRead(IN5)==LOW){
    Serial.println("开始");
    wholeMotion();
    delay(5000);
  }else{
    Serial.println("不动");
  }
  // 复位
  Serial.println("复位");
  resetting();
  // cupback();
  // k=30;
  // flagOut=true;
  // cupLoose();
  delay(500);
}