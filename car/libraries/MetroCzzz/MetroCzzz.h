
#ifndef MetroCzzz_h
#define MetroCzzz_h

class MetroCzzz
{
public:
  MetroCzzz(int slpin,int srpin,int dlpin,int drpin); //
  void forward(int speed);
  void back(int speed);
  void trunLeft(int speed);
  void trunRight(int speed);
  void stop();
  void trunLeftBack(int speed);
  void trunRightBack(int speed);

  

private:
  int speedLeftPin;
  int speedRightpin;
  int dirLeftPin;
  int dirRightPin;
  //uint8_t autoreset;
  //unsigned long  previous_millis, interval_millis;
};
#endif



