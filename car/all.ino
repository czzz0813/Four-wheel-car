#include <Timers.h>     //定时任务
#include <Servo.h>      //伺服电机 
#include <Metro.h>      //定时任务
#include "GoBLE.h"      //蓝牙手柄
#include <EEPROM.h>     //断电存储

///czzz 定时任务  好像没起作用
Metro measureDistance = Metro(50);
Metro sweepServo = Metro(20);
///czzz 要测试

int speedPin_M1 = 5;     //M1 Speed Control 速度控制引脚
int speedPin_M2 = 6;     //M2 Speed Control  速度控制引脚
int directionPin_M1 = 4;     //M1 Direction Control 方向控制引脚
int directionPin_M2 = 7;     //M1 Direction Control  方向控制引脚
boolean isAuto=false;   //需要保持状态  eprom
unsigned long actualDistance = 0;  //微声波测距离
boolean bpress5=false;    //自动切手动 防止多次响应

Servo myservo;  // create servo object to control a servo  舵机 
int pos = 60;   //舵机初始化角度  60-120
int sweepFlag = 1;   //舵机是否回头标识

int URPWM = 3; // PWM Output 0－25000US，Every 50US represent 1cm  速度
int URTRIG= 10; // PWM trigger pin       PWM引脚触发
uint8_t EnPwmCmd[4]={0x44,0x02,0xbb,0x01};    // distance measure command  距离测量命令


int joystickX, joystickY;   //蓝牙摇杆
int buttonState[6];         //蓝牙按钮

void setup()
{                                 
    isAuto=EEPROM.read(0);                    //持久化上次关机后状态读出
    myservo.attach(9);                          //伺服电机 引脚初始化
    Serial.begin(115200);                       //串口初始化    
    Goble.begin();                              //蓝牙初始化
    SensorSetup();                              //传感器设置
    TCs.AddFunc(0, servoSweep, 20);             //舵机定时转
    TCs.AddFunc(1, checkAuto, 20);              //定时检查是否切回自动模式
}
 
void loop()
{
    if (isAuto) 
    { 
        // autoRun();           //老自动巡航系统
        autoRunNew();           //自动巡航
    }
    else
    {
        manRun();               //手动模式            
    }   
}

///检查是否切回自动模式
void checkAuto()
{
    if(Goble.available()) 
    {
 
        int x=Goble.readSwitchSelect();
        if (x==PRESSED)
        {

            carStop();
            isAuto=false;
            EEPROM.write(0,0);        
        }
    }
}

//手动模式
void manRunByButton()
{
  int pressNum=0;
  for(int i=1;i<5;i++)
  {
    pressNum++;
    bpress5=false;
  }

  ///瞎鸡巴按
  if (pressNum>=3)  {carStop();bpress5=false;}

  if (pressNum==2)
  {
    ///瞎鸡巴按
    if (  (buttonState[SWITCH_UP]  == PRESSED && buttonState[SWITCH_DOWN] ==PRESSED)  || (buttonState[SWITCH_LEFT]==PRESSED && buttonState[SWITCH_RIGHT]==PRESSED ) )  
      {
        carStop();
      }
      //左前方走
      else if (buttonState[SWITCH_UP]  == PRESSED  && buttonState[SWITCH_LEFT]==PRESSED)
      {
        carTurnAdvance(100,220);
      }
      //右前方走
      else if (buttonState[SWITCH_UP]  == PRESSED  && buttonState[SWITCH_RIGHT]==PRESSED)
      {
        carTurnAdvance(220,100);
      }
    //左后方走
    else if (buttonState[SWITCH_DOWN]  == PRESSED  && buttonState[SWITCH_RIGHT]==PRESSED)
    {
      carTurnAdvance(100,220);
    }
     //右后方走
    else if (buttonState[SWITCH_DOWN]  == PRESSED  && buttonState[SWITCH_RIGHT]==PRESSED)
    {
      carTurnAdvance(220,100);
    }
    bpress5=false;
  }
  else 
  {
    int i=1;
    int b=0;
    for ( i=1;i<5;i++)
    {
      if (buttonState[i] == PRESSED)
      { 
        b=i;
        break;
      }
     }
     if (i==5) b=0; 
     switch (b)
     {
       case 1:
         bpress5=false;
         carAdvance(100,100);
         break;
       case 2:
         bpress5=false;
         carTurnAdvance(220,0);
         break;
       case 3:
         bpress5=false;
         carBack(100,100);
         break;
       case 4:
         bpress5=false;
         carTurnAdvance(0,220);
         break;
        case 0:
         bpress5=false;
         carStop();
         break;
      }
       //delay(500);  
  }
}


void manRun()
{

  if(Goble.available()){
    joystickX = Goble.readJoystickY();
    joystickY = Goble.readJoystickX();
    
    buttonState[SWITCH_UP]     = Goble.readSwitchUp();
    buttonState[SWITCH_DOWN]   = Goble.readSwitchDown();
    buttonState[SWITCH_LEFT]   = Goble.readSwitchLeft();
    buttonState[SWITCH_RIGHT]  = Goble.readSwitchRight();
    buttonState[SWITCH_SELECT] = Goble.readSwitchSelect();
    buttonState[SWITCH_START]  = Goble.readSwitchStart();
    
   Serial.print("Joystick Value: ");
    Serial.print(joystickX);
    Serial.print("  ");
    Serial.println(joystickY);
//    
//    for (int i = 1; i < 7; i++) {
//      Serial.print("Button ID: ");
//      Serial.print(i);
//      Serial.print("\t State: ");
//      if (buttonState[i] == PRESSED)   Serial.println("Pressed!");
//      if (buttonState[i] == RELEASED)  Serial.println("Released!");
      
 //   }

//switch auto or shoudong  切换手自动
if (buttonState[SWITCH_SELECT]==PRESSED)
{
     if (bpress5==false)
    {
      isAuto=true;
       EEPROM.write(0,1);
    }
    bpress5=true;
}
//Serial.print ("")
//摇杆还是安按钮
if ( joystickX==128 && joystickY==128 )
{
  manRunByButton();
}
else
{
  manRunByJoystick();
  
}

}
}

void manRunByJoystick()
{
   int SpeedX=2*joystickX-256;
   int SpeedY=2*joystickY-256;
   carGo(SpeedX,SpeedY);
}


void carGo(int x,int y)
{
  Serial.print("x,y value:  ");
  Serial.print(x);
  Serial.print("  ");
  Serial.println(y);
  int leftSpeed=0,rightSpeed=0;
  if (y>=0)
  {
    Serial.println("go ahead");
    digitalWrite(directionPin_M1,HIGH); 
    digitalWrite(directionPin_M2,HIGH); 
  }
  else
  {
    Serial.print("go back");
    digitalWrite(directionPin_M1,LOW); 
    digitalWrite(directionPin_M2,LOW); 
  }
  y=abs(y);
  //前进或后退
  if (x==0)
  {
    
    leftSpeed=y;
    rightSpeed=y;
  }
  //拐弯
  else
  {
    int angle=getAngle(abs(x),y);
    if (y<30) 
    {
      y=30;
    }
    if (x>0)
    {
      leftSpeed=y;
      rightSpeed=y*angle/90;
    }
    else
    {
      rightSpeed=y;
      leftSpeed=y*angle/90;
    }
  }
  analogWrite (speedPin_M1,leftSpeed);              //PWM Speed Control
  analogWrite (speedPin_M2,rightSpeed);
  Serial.print(leftSpeed);
  Serial.print("   ");
  Serial.println(rightSpeed); 
}


int getAngle(int x,int y)
{
      
    float  g=atan(y/x)*180/PI;
    return int(g);
 }

void autoRunNew()
{
      // servoSweep();
    int posNow=90;
    int initTurnSpeed=100;
    int initTurnDelay=2000;
    int initAdvanceSpeed=70;
    int initAdvanceDelay=200;
    int initBackSpeed=100;
    int initBackDelay=1000;
    int initStopDelay=1000;
    float modulus=1;  //系数
    int leftSpeed=0;
    int rightSpeed=0;
    int myDelay=0;
    
    if(measureDistance.check() == 1)
    {
        actualDistance = MeasureDistance();
        posNow=pos;
        // Serial.println(actualDistance);
        delay(100);
    }
    //Serial.println(actualDistance);
    if(actualDistance <= 40)
    {
        modulus=actualDistance/10;
        if (modulus<=1)
        {
            carStop();
            delay(initStopDelay);
            carBack(initBackSpeed,initBackSpeed);
            delay(initBackDelay);
        }
        else
        {
            myDelay=(1-(abs((90-pos))/30))*initTurnDelay;
            //modulus+=actualDistance/40
            //  myservo.write(90);
            if(posNow>=90)
            {
                leftSpeed=initTurnSpeed;
                rightSpeed=initTurnSpeed*abs(90-pos)/30;
            }
            else
            {
                rightSpeed=initTurnSpeed;
                leftSpeed=initTurnSpeed*abs((90-pos))/30;
            }
            carTurnAdvance(leftSpeed,rightSpeed);    
            delay(myDelay);
        }
    }
    else
    {
        carAdvance(70,70);
        //Serial.println("carAdvance");
        delay(200);
    }
    //zz  carBack(150,150);
}

void autoRun()
{
    
    // servoSweep();
    int posNow=90;
    // checkAuto();
    //delay(100);
    if(measureDistance.check() == 1)
    {
        actualDistance = MeasureDistance();
        posNow=pos;
        // Serial.println(actualDistance);
        delay(100);
    }
 

    //Serial.println(actualDistance);
    if(actualDistance <= 30)
    {
        //  myservo.write(90);
        if(posNow>=90)
        {
            carStop();
            delay(500);
            //carBack(100,100);   //home              
            carBack(100,100);
            //Serial.println("carBack");
            delay(2000);
            //carTurnLeft(0,150);  //home                 
            carTurnAdvance(200,0);    
            //Serial.println("carTurnRight");
            delay(2000);
            // delay(1000);  //home
               
        }
        else
        {
            carStop();
            delay(500);     
            //carBack(100,100);   //home
            carBack(100,100);
            //Serial.println("carBack");
            delay(2000);
            //carTurnLeft(150,0);  //home
            carTurnAdvance(0,200);
            //Serial.println("carTurnLeft");
            delay(2000);
            //delay(1000);  //home
        }
    }
    else
    {
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

void carTurnAdvance(int leftSpeed,int rightSpeed){         //Turn Left
  analogWrite (speedPin_M1,leftSpeed);              //PWM Speed Control
  digitalWrite(directionPin_M1,HIGH);    
  analogWrite (speedPin_M2,rightSpeed);    
  digitalWrite(directionPin_M2,HIGH);
} 


void carTurnBack(int leftSpeed,int rightSpeed){       //Turn Right
  analogWrite (speedPin_M1,leftSpeed);
  digitalWrite(directionPin_M1,LOW);   
  analogWrite (speedPin_M2,rightSpeed);    
  digitalWrite(directionPin_M2,LOW);
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


