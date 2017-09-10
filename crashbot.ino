#include<Servo.h>
Servo wservo;
const int M1 = 7;
const int M2 = 4;
const int EN1 = 6; //pwm
const int EN2 = 5;

const int servo = 8;
const int TRIG = 11;
const int ECHO = 10;

int i;
int olddis2 = 0;
int olddis = 0;
int direct = 0;
long dis = 0;
long dis2 = 0;


void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  Serial.begin(9600);  
  wservo.attach(servo);
  // put your setup code here, to run once:

}

void loop() {
  stopmotor();
  wservo.write(90);// 거리 센서 방향 초기화
  delay(500);
  dis = distant();
  
  while(1){
  forward(); 
  olddis2 = dis;
  dis = distant();//앞으로 가면서 가로막힌 곳 체크
  delay(10);


  
  if(abs(olddis2-dis) >= 8){      //센서값 보정
    dis = olddis2;
  }
    Serial.println(dis);
  

  if(dis < 10.0){
    stopmotor();
    wservo.write(0);
    delay(500);
    for(i = 0; i < 180; i++){
      if(i % 15 == 0){
        dis2 = distant();
        if(dis2 > olddis){
        olddis = dis2;
        direct = i;       
        }
      }
    wservo.write(i);
    delay(5);
  }

  Serial.println(direct);
  if(direct > 90){
    right(i);
  } 
  if(direct < 90){
    left(i);
  }
    olddis = 0;
    break;
  }
  

  }
}

void left(int angle)
{

  analogWrite(EN1,0);
  analogWrite(EN2,255);
  digitalWrite(M2, 1);
  delay(abs(angle-90)*7);

}

void right(int angle)

{

  analogWrite(EN1,255);
  analogWrite(EN2,0);
  digitalWrite(M1, 1);  
  delay(abs(angle-90)*7);

}

void stopmotor()
{
  digitalWrite(EN1, LOW);
  digitalWrite(EN2, LOW); 
}

void forward()
{
  analogWrite(EN1,255);
  analogWrite(EN2,255);
  digitalWrite(M1, 1);
  digitalWrite(M2, 1);
}
long distant(){
  digitalWrite(TRIG, LOW);                  
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long time_us = pulseIn(ECHO, HIGH);  // Get echo time in microsecs
  long dist_mm = time_us * 0.17*0.1; 
  return dist_mm;
}


