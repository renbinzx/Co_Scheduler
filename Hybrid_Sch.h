#ifndef __HYBRID_SCH_H__
#define __HYBRID_SCH_H__
#include "port.h"

/*
任务数据结构
*/
typedef struct
{
	void (* Task_p)(void); //任务函数指针
	tuWord 	Delay;			//任务启动前延时时间
	tuWord	Period;			//任务运行周期
	tuByte	Ready_Flag;          //任务ready标志位
	tuByte	Co_op;			//抢占式任务ready标志位	
}sTask;
	

#define ERROR_SCH_TOO_MANY_TASKS	0xff

void hSCH_Start(void);
tuByte hSCH_Add_Task(void (* pFunc)(), const tuWord Delay, const tuWord Period, const tuByte Co_op);
void hSCH_Dispatch_Tasks(void);
#endif