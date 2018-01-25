#include "Hybrid_Sch.h"

#define SCH_MAX_TASKS	PORT_MASK_TASKS


sTask	SCH_task_G[SCH_MAX_TASKS];
tuByte	Error_code_G;

/*------------------------------------------------------------------*-
hSCH_Start()
Scheduler initialization function. Prepares scheduler
data structures and sets up timer interrupts at required rate.
Must call this function before using the scheduler.
-*------------------------------------------------------------------*/
void hSCH_Start()
{
	vPort_SysTick_Init();
}

/*------------------------------------------------------------------*-
SCH_Add_Task()
Causes a task (function) to be executed at regular intervals
or after a user-defined delay
-*------------------------------------------------------------------*/
tuByte hSCH_Add_Task(void (* pFunc)(), const tuWord Delay, const tuWord Period, const tuByte Co_op)
{
	tuByte	Index = 0;
	
	while((SCH_task_G[Index].Task_p != 0) && (Index < SCH_MAX_TASKS))
	{
		Index++;
	}
	
	if(Index == SCH_MAX_TASKS)
	{
		Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
		
		return SCH_MAX_TASKS;
	}
	
	SCH_task_G[Index].Task_p = pFunc;
	SCH_task_G[Index].Delay = Delay;
	SCH_task_G[Index].Period = Period;
	SCH_task_G[Index].Co_op = Co_op;
	
	return Index;
}

/*------------------------------------------------------------------*-
SCH_Delete_Task()
Delect a task by Index
-*------------------------------------------------------------------*/
void SCH_Delete_Task(tuByte Index)
{
	SCH_task_G[Index].Task_p = 0;
}

/*------------------------------------------------------------------*-
系统进入IDEL状态

-*------------------------------------------------------------------*/
void SCH_Go_To_Sleep(void)
{
	tuByte i;
	i++;			//避免编译器报错
	vPort_IDEL_Hook()
	
}

/*------------------------------------------------------------------*-
hSCH_Dispatch_Tasks()
Causes a task (function) to be executed at regular intervals
or after a user-defined delay
-*------------------------------------------------------------------*/
void hSCH_Dispatch_Tasks(void)
{
	tuByte Index;
	
	for(Index = 0; Index <SCH_MAX_TASKS; Index++)
	{
		if(SCH_task_G[Index].Ready_Flag > 0)
		{
			(*SCH_task_G[Index].Task_p)();
			SCH_task_G[Index].Ready_Flag -= 1;
			
			if(SCH_task_G[Index].Period == 0)
			{
				SCH_Delete_Task(Index);
			}
		}
	}
	SCH_Go_To_Sleep();
}

void hSCH_UpData(void) interrupt T3INT
{
	tuByte	Index;
	for(Index = 0; Index <SCH_MAX_TASKS; Index++)
	{
		if(SCH_task_G[Index].Task_p)
		{
			if(SCH_task_G[Index].Delay == 0)
			{
				SCH_task_G[Index].Ready_Flag += 1;
				if(SCH_task_G[Index].Period)
				{
					SCH_task_G[Index].Delay = SCH_task_G[Index].Period;
				}

			}
			else
			{
				SCH_task_G[Index].Delay -= 1;
			}
		}
	}
}
