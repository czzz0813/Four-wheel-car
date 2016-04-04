
#include "MetroCzzz.h"
#include <Arduino.h>


MetroCzzz::MetroCzzz(int slpin,int srpin,int dlpin,int drpin)
{
speedLeftPin= slpin;     //M1 Speed Control 速度控制引脚
speedRightpin=srpin;
dirLeftPin=dlpin;
dirRightPin=drpin;
}

void MetroCzzz::forward(int speed)
{
    analogWrite (speedLeftPin,speed);              //PWM Speed Control
    digitalWrite(dirLeftPin,HIGH);    
    analogWrite (speedRightpin,speed);    
    digitalWrite(dirRightPin,HIGH);

}
void MetroCzzz::back(int speed)
{
   analogWrite (speedLeftPin,speed);              //PWM Speed Control
    digitalWrite(dirLeftPin,LOW);    
    analogWrite (speedRightpin,speed);    
    digitalWrite(dirRightPin,LOW);
}
void MetroCzzz::trunLeft(int speed)
{
   analogWrite (speedLeftPin,0);              //PWM Speed Control
    digitalWrite(dirLeftPin,HIGH);    
    analogWrite (speedRightpin,speed);    
    digitalWrite(dirRightPin,HIGH);
}
void MetroCzzz::trunRight(int speed)
{
   analogWrite (speedLeftPin,speed);              //PWM Speed Control
    digitalWrite(dirLeftPin,HIGH);    
    analogWrite (speedRightpin,0);    
    digitalWrite(dirRightPin,HIGH);
}
void MetroCzzz::stop()
{
   analogWrite (speedLeftPin,0);              //PWM Speed Control
    digitalWrite(dirLeftPin,LOW);    
    analogWrite (speedRightpin,0);    
    digitalWrite(dirRightPin,LOW);

}
  void MetroCzzz::trunLeftBack(int speed)
  {
    analogWrite (speedLeftPin,0);              //PWM Speed Control
    digitalWrite(dirLeftPin,LOW);    
    analogWrite (speedRightpin,speed);    
    digitalWrite(dirRightPin,LOW);
    
  }
  void MetroCzzz::trunRightBack(int speed)
  {
    analogWrite (speedLeftPin,speed);              //PWM Speed Control
    digitalWrite(dirLeftPin,LOW);    
    analogWrite (speedRightpin,0);    
    digitalWrite(dirRightPin,LOW);
  }



