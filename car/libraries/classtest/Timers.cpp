/*
  Timers.h - Library
  ���ܣ�ʹ��TC2��ʱִ�к���
  Created by czad
*/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "Timers.h"

void Run();
unsigned char _TCidNum;  //�Ѿ�����ĺ�������
unsigned long _TC_count;  //ÿ��λΪ1ms��֧�����4294967295ms��49.71�죩

ISR (TIMER2_COMPA_vect)  //TC2���������ƥ��A�жϳ���
{
	if(_TC_count++ > 0xFFFFFFFD) _TC_count = 0;  //����ﵽ���ֵʱ����
	
	for(unsigned char i = 0; i < _TCidNum; i++)
	{
		if(_TC_count % TCdelay[i] == 0) TCstate[i] = 1;
	}

	Run();
}

Timers::Timers()
{
	//������TC2��8MHz��ÿ����0.125us��16MHz��ÿ����0.0625us
	TCCR2B = _BV(CS22);    //TC2�Ĵ�����64��Ƶ��8MHz��ÿλ8us��16MHz��ÿλ4us
	OCR2A  = 250;          //TC2�ȽϼĴ���A��125-8MHz/64��Ƶ��Ϊ1msһ���ڣ�16MHz�����Ϊ250
	TIMSK2 = _BV(OCIE2A);  //TC2�ж����μĴ�����ʹ������Ƚ�ƥ��A�ж�
	
	_TC_count = 0;
	_TCidNum = 0;
}

void Timers::AddFunc(unsigned char TCid, void (*TCuserFunc)(), unsigned long TCuserDelay)
{
	if(TCid < 8)
	{
		TCfunc[TCid] = TCuserFunc;  //������������һ������
		TCdelay[TCid] = TCuserDelay;  //��������ʱ����������һ��
		_TCidNum++;  //�Ѷ���ĺ���������һ
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