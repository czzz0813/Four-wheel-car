/*
  Timers.h - Library
  ���ܣ�ʹ��TC2��ʱִ�к���
  Created by czad
*/

#if ARDUINO >= 100
#include "Arduino.h"   //����delay
#else
#include "WProgram.h"  //����delay
#endif

#ifndef Timers
#define Timers_h

typedef void (*voidFunc)();  //����һ����������
static volatile voidFunc TCfunc[8];  //����һ��8λ�ĺ������飨��������
static volatile unsigned long TCdelay[8];  //����һ��8λ�ĺ�������ʱ������ʾ������ÿ����ms����һ�Σ�
static volatile bool TCstate[8] = {0,0,0,0,0,0,0,0};  //����һ��8λ�ĺ�������״̬��ָʾ�����Ƿ������У�

class Timers
{
  public:
    Timers();
	void AddFunc(unsigned char TCid, void (*TCuserFunc)(), unsigned long TCuserDelay);
};
extern Timers TCs;

#endif