
#ifndef MetroCzzz_h
#define MetroCzzz_h
#define SLPIN = 5;     //M1 Speed Control 速度控制引脚
#define SRPIN = 6;     //M2 Speed Control  速度控制引脚
#define DLPIN = 4;     //M1 Direction Control 方向控制引脚
#define DRPIN = 7;     //M1 Direction Control  方向控制引脚
class MetroCzzz
{
public:
  MetroCzzz(int slpin,int srpin,int dlpin,int drpin); //
  MetroCzzz();
  void forward(int speed);
  void back(int speed);
  void turnLeft(int speed);
  void turnRight(int speed);
  void stop();
  void turnLeftBack(int speed);
  void turnRightBack(int speed);

  

private:
  int speedLeftPin;
  int speedRightpin;
  int dirLeftPin;
  int dirRightPin;
  //uint8_t autoreset;
  //unsigned long  previous_millis, interval_millis;
};
#endif



