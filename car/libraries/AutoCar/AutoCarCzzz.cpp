

#include "AutoCarCzzz.h"
AutoCarCzzz::AutoCarCzzz()
{
  pos = 60;   //舵机初始化角度  60-120
  sweepFlag = 1;   //舵机是否回头标识
  actualDistance = 500;  //微声波测距离

    sensorSetup();                   //传感器设置
    //TCs.AddFunc(0, servoSweep, 20);             //舵机定时转
    //TCs.AddFunc(1, checkAuto, 20);              //定时检查是否切回自动模式

  
}


///舵机转动
void AutoCarCzzz::servoSweep()
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
            if(pos>=60 && pos<=120)
            {    
                pos=pos-1;
                myservo.write(pos);
            }
            if(pos<61)  sweepFlag = true;
        }
    
}
                                     
 
//传感器设置
void  AutoCarCzzz::sensorSetup()
{ 
    pinMode(URTRIG,OUTPUT);                     // A low pull on pin COMP/TRIG  //超声波发射
    digitalWrite(URTRIG,HIGH);                  // Set to HIGH 
    pinMode(URPWM, INPUT);                      // Sending Enable PWM mode command  //超声波接收
//  for(int i=0;i<4;i++){
//      Serial.write(EnPwmCmd[i]);
//   } 
}
 
 
 
//测距离
int AutoCarCzzz::measureDistance()
{  
    // a low pull on pin COMP/TRIG  triggering a sensor reading
    digitalWrite(URTRIG, LOW);
    digitalWrite(URTRIG, HIGH);               // reading Pin PWM will output pulses    
    unsigned long distance=pulseIn(URPWM,LOW);    //读取引脚上的高电平脉冲
    if(distance>=50000)
    {              // the reading is invalid.
        distance=200;     // 无效值，默认200cm    
    }
    else
    {
        distance=distance/50;           // every 50us low level stands for 1cm
    }
    return distance;
}



void AutoCarCzzz::autoRun()
{
    int posNow=90;
    int initTurnSpeed=150;
    int initTurnDelay=2000;
    int initAdvanceSpeed=70;
    int initAdvanceDelay=200;
    int initBackSpeed=100;
    int initBackDelay=2000;
    int initStopDelay=1000;
    float modulus=1;  //系数
    int leftSpeed=0;
    int rightSpeed=0;
    int myDelay=0;
  
    actualDistance = measureDistance();
    posNow=pos;
    
    if(actualDistance <= 40)
    {
        modulus=actualDistance/10;
        if (modulus<=1)
        {
            car.stop();
            delay(initStopDelay);
            car.back(initBackSpeed);
            delay(initBackDelay);
        }
        else
        {
            myDelay=(1-(abs((90-pos))/30))*initTurnDelay;
            //modulus+=actualDistance/40
            //  myservo.write(90);
            if(posNow>=90)
            {
                //leftSpeed=initTurnSpeed;
                //rightSpeed=(initTurnSpeed-120)*abs(90-pos)/30;
                car.turnRight(initTurnSpeed);
            }
            else
            {
                //rightSpeed=initTurnSpeed;
                //leftSpeed=(initTurnSpeed-120)*abs((90-pos))/30;
                 car.turnLeft(initTurnSpeed);
            }
            //car.turnLeft( carTurnAdvance(leftSpeed,rightSpeed);    
            delay(myDelay);
        }
    }
    else
    {
        car.forward(initAdvanceSpeed);
        //Serial.println("carAdvance");
        delay(200);
    }
    //zz  carBack(150,150);

}
