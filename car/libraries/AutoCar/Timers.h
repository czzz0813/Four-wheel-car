/*
  Timers.h - Library
  功能：使用TC2定时执行函数
  Created by czad
*/

#if ARDUINO >= 100
#include "Arduino.h"   //用于delay
#else
#include "WProgram.h"  //用于delay
#endif

#ifndef Timers
#define Timers_h

typedef void (*voidFunc)();  //定义一个函数类型
static volatile voidFunc TCfunc[8];  //定义一个8位的函数数组（函数名）
static volatile unsigned long TCdelay[8];  //定义一个8位的函数运行时长（表示函数需每多少ms运行一次）
static volatile bool TCstate[8] = {0,0,0,0,0,0,0,0};  //定义一个8位的函数运行状态（指示函数是否需运行）

class Timers
{
  public:
    Timers();
	void AddFunc(unsigned char TCid, void (*TCuserFunc)(), unsigned long TCuserDelay);
};
extern Timers TCs;

#endif