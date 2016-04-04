/*
  Timers.h - Library
  功能：使用TC2定时执行函数
  Created by czad
*/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "Timers.h"

void Run();
unsigned char _TCidNum;  //已经定义的函数个数
unsigned long _TC_count;  //每单位为1ms，支持最大4294967295ms（49.71天）

ISR (TIMER2_COMPA_vect)  //TC2计数器输出匹配A中断程序
{
	if(_TC_count++ > 0xFFFFFFFD) _TC_count = 0;  //当快达到最大值时归零
	
	for(unsigned char i = 0; i < _TCidNum; i++)
	{
		if(_TC_count % TCdelay[i] == 0) TCstate[i] = 1;
	}

	Run();
}

Timers::Timers()
{
	//计数器TC2，8MHz下每周期0.125us，16MHz下每周期0.0625us
	TCCR2B = _BV(CS22);    //TC2寄存器，64分频，8MHz下每位8us，16MHz下每位4us
	OCR2A  = 250;          //TC2比较寄存器A，125-8MHz/64分频下为1ms一周期，16MHz下需改为250
	TIMSK2 = _BV(OCIE2A);  //TC2中断屏蔽寄存器，使能输出比较匹配A中断
	
	_TC_count = 0;
	_TCidNum = 0;
}

void Timers::AddFunc(unsigned char TCid, void (*TCuserFunc)(), unsigned long TCuserDelay)
{
	if(TCid < 8)
	{
		TCfunc[TCid] = TCuserFunc;  //向函数数组增加一个函数
		TCdelay[TCid] = TCuserDelay;  //向函数运行时长数组增加一个
		_TCidNum++;  //已定义的函数个数加一
	}
}

void Run()
{
	for(unsigned char i = 0; i < _TCidNum; i++)
	{
		if(TCstate[i])
		{
			TCfunc[i]();
			TCstate[i] = 0;
		}
	}
}

Timers TCs;

// SIGNAL(INT0_vect)
// {
	// if(TCfunc[EXTERNAL_INT_0])
	// TCfunc[EXTERNAL_INT_0]();
// }