
#ifndef AutoCarCzzz_h
#define AutoCarCzzz_h

#include <Arduino.h>

#include "Timers.h"     //定时任务
#include "MetroCzzz.h"
#include "Servo.h"      //伺服电机
#define LEFTCODE 1
#define RIGHTCODE 0



class AutoCarCzzz
{
  public:
  AutoCarCzzz();
  void autoRun();
void lwheelSpeed();
  void rwheelSpeed();
   void sensorSetup();
  private:
  static const int sweepServoPin=9;
  static const int URPWM = 13; // PWM Output 0－25000US，Every 50US represent 1cm  速度
  static const int URTRIG= 12; // PWM trigger pin       PWM引脚触发

  unsigned long actualDistance=500; //微声波测距离
  int pos=60;//舵机初始化角度  60-120
  boolean sweepFlag=true;   //舵机是否回头标识
  MetroCzzz car;
 Servo myservo;  // create servo object to control a servo  舵机  
  int measureDistance();
  
 
  
  void servoSweep();
  void checkWheelStoped();  
  long coder[2] = {
  0,0};
int lastSpeed[2] = {
  0,0};  
  
};

#endif

