
#ifndef AutoCarCzzz_h
#define AutoCarCzzz_h

#include <Arduino.h>

#include "Timers.h"     //定时任务
#include "MetroCzzz.h"
#include "Servo.h"      //伺服电机


class AutoCarCzzz
{
  public:
  AutoCarCzzz();
  void autoRun();

  private:
  static const int sweepServoPin=9;
  static const int URPWM = 13; // PWM Output 0－25000US，Every 50US represent 1cm  速度
  static const int URTRIG= 12; // PWM trigger pin       PWM引脚触发

  unsigned long actualDistance ; //微声波测距离
  int pos;//舵机初始化角度  60-120
  int sweepFlag;   //舵机是否回头标识
  MetroCzzz car;
 Servo myservo;  // create servo object to control a servo  舵机  
  int measureDistance();
  
  void  sensorSetup();
  void servoSweep();
  
};

#endif

