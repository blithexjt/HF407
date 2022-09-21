#include "motor.h"
#include "timer.h"
#include "drv8889.h"
#include "delay.h"
#include "usart.h"
#include "ucommand.h"


const u32 Fsq[11] = {1,1,2,2,4,8,16,32,64,128,256};
static u8 Fsq_X_Count;
static u8 Fsq_Y_Count;


void MotorInit()
{
	DRV8889_Init();
}

 void MotorSetCurrent(u8 Motor, u8 current)
{
	u8 temp1 = Motor;
	u8 temp2 = current;
	#ifdef DRV8889
	Drv8889_SetCurrent (temp1,temp2);

	#else


	#endif
	
}

void MotorSetSpeed(u8 motor,float speed)
{
	u32 arr = 0xffffff;
	u32 freq = 0;
	u32 psc = 1;
	TIM_HandleTypeDef* TIMERX;
	
	if(motor == Motor_x)
	{	
		if(0 == speed)
		{
			HAL_TIM_PWM_Stop(&TIM4_Handler, TIM_CHANNEL_1);
			return ;
		}
		freq = speed*200*Fsq[Fsq_X_Count];
		TIMERX = &TIM4_Handler;
	}
	if(motor == Motor_y)   
	{
		if(0 == speed)
		{
			HAL_TIM_PWM_Stop(&TIM10_Handler, TIM_CHANNEL_1);
			return ;
		}
		freq = speed*200*Fsq[Fsq_Y_Count];
		TIMERX = &TIM10_Handler;
	}

	while(arr >= 65535)
	{
		arr = 84000000/(psc+1)/freq - 1;
		psc++;
	}
	psc = psc-1;
	TIM_update(TIMERX,arr,psc,AutoUpdate);	
}

void MotorRun(u8 Motor, u32 current,float speed, double length)
{
	volatile int num = length*(200*Fsq[Fsq_X_Count]);
	volatile u32 arr = 0xffffff;
	volatile u32 freq = 0;
	volatile u32 psc = 1;
	
	switch(Motor)
	{
		case Motor_x:		
			num = length*(200*Fsq[Fsq_X_Count]);
			freq = speed*200*Fsq[Fsq_X_Count];
			break;
		case Motor_y:		
			num = length*(200*Fsq[Fsq_Y_Count]);
			freq = speed*200*Fsq[Fsq_Y_Count];
			break;
	}
	MotorSetCurrent( Motor, current);	
	while(arr >= 65535)
	 {
		  arr = (84000000/(psc+1)/freq) - 1;
		  psc++;
	 }
	psc = psc-1;
	if(0 == num) return;
	if(num < 0)
	{
		num = -num;
		MoveBackward (Motor,arr,psc,num);
	}
	else
	{
		MoveForward(Motor,arr,psc,num);
	}
}

void MotorStart(u8 Motor,u8 current)
{
		switch(Motor)
		{
			case	Motor_x:
							DRV8889_EN_X_en();
							MotorSetCurrent( Motor, current);
							break;
			case	Motor_y:
							DRV8889_EN_Y_en();
							MotorSetCurrent( Motor, current);
							break;
		}
}


void MotorStop(u8 Motor,u8 current)
{
	switch(Motor)
	{
		case	Motor_x:
							HAL_TIM_PWM_Stop(&TIM4_Handler, TIM_CHANNEL_1);
							HAL_TIM_Base_Stop_IT(&TIM3_Handler);
							break;
		case	Motor_y:
							HAL_TIM_PWM_Stop(&TIM10_Handler, TIM_CHANNEL_1);
							HAL_TIM_Base_Stop_IT(&TIM9_Handler); 
							break;
	}
	MotorSetCurrent( Motor, current);
}	


void Set_Fsq_Count(u8 Motor,u8 num)
{
	switch(Motor)
	{
		case	Motor_x:
							Fsq_X_Count = num;
							break;
		case	Motor_y:
							Fsq_Y_Count = num;
							break;
	}
}

void ReadMotorSTL(void)
{
	u8 STL_XStatus =0;
	u8 STL_YStatus =0;
	STL_XStatus = Drv8889_ReadSTLStatus(Motor_x);
	STL_YStatus = Drv8889_ReadSTLStatus(Motor_y);
	printf("the value of MOTORX STL is %d\r\n",STL_XStatus);
	printf("the value of MOTORY STL is %d\r\n",STL_YStatus);
}

void MoveBackward(u8 Motor,u32 arr,u32 psc,u32 step)
{
	u32 pulse = step;
	switch(Motor)
	{
		case Motor_x: 
			MOTORX_DIR_Backward();		
			Motor_PWM(Motor_x,arr,psc,pulse);
			break;
		case Motor_y: 
			MOTORY_DIR_Backward();		
			Motor_PWM(Motor_y,arr,psc,pulse);
			break;
	}
}

void MoveForward(u8 Motor,u32 arr,u32 psc,u32 step)
{
	u32 pulse = step;
	switch(Motor)
	{
		case Motor_x:  
			MOTORX_DIR_Forward();		
			Motor_PWM(Motor_x,arr,psc,pulse);
			break;
		case Motor_y:  
			MOTORY_DIR_Forward();		
			Motor_PWM(Motor_y,arr,psc,pulse);
			break;
	}	
}



