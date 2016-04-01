#include <Timers.h>
#include <Servo.h>   //伺服电机 
#include <Metro.h>
#include "GoBLE.h"
#include <EEPROM.h>
Metro measureDistance = Metro(50);
Metro sweepServo = Metro(20);

int speedPin_M1 = 5;     //M1 Speed Control 速度控制引脚
int speedPin_M2 = 6;     //M2 Speed Control  速度控制引脚
int directionPin_M1 = 4;     //M1 Direction Control 方向控制引脚
int directionPin_M2 = 7;     //M1 Direction Control  方向控制引脚
boolean isAuto=false;   //需要保持状态  eprom
unsigned long actualDistance = 0;
boolean apress5=false;
boolean bpress5=false;

Servo myservo;  // create servo object to control a servo 
int pos = 60; 
int sweepFlag = 1;
int b=0;

int URPWM = 3; // PWM Output 0－25000US，Every 50US represent 1cm  速度
int URTRIG= 10; // PWM trigger pin       PWM引脚触发
uint8_t EnPwmCmd[4]={0x44,0x02,0xbb,0x01};    // distance measure command  距离测量命令


int joystickX, joystickY;
int buttonState[6];

void setup(){                                 // Serial initialization

  isAuto=EEPROM.read(0);                    //持久化上次关机后状态读出
  
  myservo.attach(9);                          //伺服电机 引脚初始化
  Serial.begin(115200);    
  Goble.begin();
   // Sets the baud rate to 9600
  SensorSetup();                              //传感器设置
TCs.AddFunc(0, servoSweep, 100);
TCs.AddFunc(1, checkAuto, 20);
}
 
void loop(){

  if (isAuto) 
  { 
  //  checkAuto();
  //manRun();
      autoRun();
  }
  else
  {
     // carStop();
      manRun();
  }   
}

void checkAuto()
{
 //Serial.println("start");
 if(Goble.available()) 
 {
 Serial.println("lanya ok");
 int x=Goble.readSwitchSelect();
 Serial.println(x); 
    if (x==PRESSED)
    {
      carStop();
      Serial.println("fuck");
      isAuto=false;
      EEPROM.write(0,0);        
    }
      //Serial.println(Goble.readSwitchSelect());
//      if (apress5==false)
//        isAuto=!isAuto;
//      
//      apress5=true; 
//    Serial.println(isAuto); 
//    }
//    else
//    {
//      apress5=false;
//      }
//  
//  }
}
}

void manRun()
{

  if(Goble.available()){
    joystickX = Goble.readJoystickX();
    joystickY = Goble.readJoystickY();
    
    buttonState[SWITCH_UP]     = Goble.readSwitchUp();
    buttonState[SWITCH_DOWN]   = Goble.readSwitchDown();
    buttonState[SWITCH_LEFT]   = Goble.readSwitchLeft();
    buttonState[SWITCH_RIGHT]  = Goble.readSwitchRight();
    buttonState[SWITCH_SELECT] = Goble.readSwitchSelect();
    buttonState[SWITCH_START]  = Goble.readSwitchStart();
    
//    Serial.print("Joystick Value: ");
//    Serial.print(joystickX);
//    Serial.print("  ");
//    Serial.println(joystickY);
//    
//    for (int i = 1; i < 7; i++) {
//      Serial.print("Button ID: ");
//      Serial.print(i);
//      Serial.print("\t State: ");
//      if (buttonState[i] == PRESSED)   Serial.println("Pressed!");
//      if (buttonState[i] == RELEASED)  Serial.println("Released!");
//      
//    }
int i=1;
for ( i=1;i<6;i++)
{
  if (buttonState[i] == PRESSED)
  { b=i;
    break;
  }
 }
 if (i==6) b=0;
 Serial.println(b);
 switch (b)
 {
  case 1:
   bpress5=false;
   carAdvance(100,100);
  
   break;
   case 2:
   bpress5=false;
   carTurnLeft(0,220);
   break;
   case 3:
   bpress5=false;
     carBack(100,100);
     break;
    case 4:
    bpress5=false;
    carTurnLeft(220,0);
    break;
    case 0:
    bpress5=false;
    carStop();
    break;
    case 5:
    if (bpress5==false)
    {
      isAuto=true;
       EEPROM.write(0,1);
    }
    bpress5=true;
    break;
  }
   //delay(500);  
  
 }
  }


void autoRun()
{
 // servoSweep();
  int posNow=90;
 // checkAuto();
  //delay(100);
 if(measureDistance.check() == 1){
      actualDistance = MeasureDistance();
      posNow=pos;
   // Serial.println(actualDistance);
     
      delay(100);
      
      
 }
 

 //Serial.println(actualDistance);
 if(actualDistance <= 30){
          //  myservo.write(90);
            if(posNow>=90){
              carStop();
              delay(500);
              //carBack(100,100);   //home              
                 carBack(100,100);
                  //Serial.println("carBack");
                  delay(2000);
                  //carTurnLeft(0,150);  //home                 
                  carTurnLeft(0,200);    
                  
                 //Serial.println("carTurnRight");
                  delay(2000);
                 // delay(1000);  //home
               }else{
                   carStop();
              delay(500);     
              //carBack(100,100);   //home
                  carBack(100,100);
                  //Serial.println("carBack");
                  delay(2000);
                  //carTurnLeft(150,0);  //home
                     carTurnLeft(200,0);
                  //Serial.println("carTurnLeft");
                   delay(2000);
                   //delay(1000);  //home
               }
    }else{
                   carAdvance(70,70);
                 //Serial.println("carAdvance");
                   delay(200);
     }
    //zz  carBack(150,150);

  }
                                     
 
void SensorSetup(){ 
  pinMode(URTRIG,OUTPUT);                     // A low pull on pin COMP/TRIG
  digitalWrite(URTRIG,HIGH);                  // Set to HIGH 
  pinMode(URPWM, INPUT);                      // Sending Enable PWM mode command
//  for(int i=0;i<4;i++){
//      Serial.write(EnPwmCmd[i]);
//   } 
}

 
int MeasureDistance(){  // a low pull on pin COMP/TRIG  triggering a sensor reading
    digitalWrite(URTRIG, LOW);
    digitalWrite(URTRIG, HIGH);               // reading Pin PWM will output pulses    
    unsigned long distance=pulseIn(URPWM,LOW);    
    if(distance==50000){              // the reading is invalid.
     // Serial.print("Invalid");    
    }else{
      distance=distance/50;           // every 50us low level stands for 1cm
    }
    return distance;
}


void carStop(){                 //  Motor Stop
  digitalWrite(speedPin_M2,0); 
  digitalWrite(directionPin_M1,LOW);    
  digitalWrite(speedPin_M1,0);   
  digitalWrite(directionPin_M2,LOW);    
}   

void carTurnLeft(int leftSpeed,int rightSpeed){         //Turn Left
  analogWrite (speedPin_M2,leftSpeed);              //PWM Speed Control
  digitalWrite(directionPin_M1,HIGH);    
  analogWrite (speedPin_M1,rightSpeed);    
  digitalWrite(directionPin_M2,HIGH);
} 

void carTurnRight(int leftSpeed,int rightSpeed){       //Turn Right
  analogWrite (speedPin_M2,leftSpeed);
  digitalWrite(directionPin_M1,HIGH);   
  analogWrite (speedPin_M1,rightSpeed);    
  digitalWrite(directionPin_M2,HIGH);
}

void carAdvance (int leftSpeed,int rightSpeed){             //Move backward
  analogWrite (speedPin_M2,leftSpeed);
  digitalWrite(directionPin_M1,HIGH);    
  analogWrite (speedPin_M1,rightSpeed);    
  digitalWrite(directionPin_M2,HIGH);
}
void carBack(int leftSpeed,int rightSpeed){           //Move forward
  analogWrite (speedPin_M2,leftSpeed);
  digitalWrite(directionPin_M1,LOW);    
  analogWrite (speedPin_M1,rightSpeed);    
  digitalWrite(directionPin_M2,LOW);
}

void servoSweep()
{
 // while(1)
 // { 
   if(sweepServo.check() == 1)
   {
  
 
  if(sweepFlag)
  {  
     if(pos>=60 && pos<=120)
     {                   
        pos=pos+1;                                  // in steps of 1 degree 
        myservo.write(pos);                         // tell servo to go to position in variable 'pos' 
     }
      if(pos>119)  sweepFlag = false;                       // assign the variable again
  }
   else 
   {                                       
      if(pos>=60 && pos<=120){    
        pos=pos-1;
        myservo.write(pos);
      }
      if(pos<61)  sweepFlag = true;
    }
   }
  // delay(50);
//  }
}

