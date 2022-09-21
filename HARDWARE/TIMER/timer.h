#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"

#define PULSENUM_MAX	60000



extern TIM_HandleTypeDef TIM1_Handler;      //定时器句柄 
extern TIM_HandleTypeDef TIM2_Handler;      //定时器句柄 
extern TIM_HandleTypeDef TIM3_Handler;      //定时器句柄 
extern TIM_HandleTypeDef TIM4_Handler;      //定时器句柄 
extern TIM_HandleTypeDef TIM6_Handler;      //定时器句柄 
extern TIM_HandleTypeDef TIM9_Handler;      //定时器句柄 
extern TIM_HandleTypeDef TIM10_Handler;      //定时器句柄 

typedef enum
{
	Motor_x,
	Motor_y,
}MOTO_NUM;



typedef enum
{
    AutoUpdate = 0,
    immediately = 1,
}TIM_Update;

void RGBLED_Init(void);

void TIM1_Init(void);
void TIM2_Init(u16 arr,u16 psc);
void TIM6_Init(u16 psc);

void TIM3_Init(void);
void TIM4_Init(void);
void TIM9_Init(void);
void TIM10_Init(void);

void PWM_GPIOInit(void);
void TIM4_Master_TIM3_Slave(u32 PulseNum1,u32 arr,u32 psc);
void TIM10_Master_TIM9_Slave(u32 PulseNum1,u32 arr,u32 psc);
void StepInit(void);

void Motor_PWM(u16 MotorNum,u32 arr,u32 psc,u32 PulseNum);
void TIM_update(TIM_HandleTypeDef * TIMx,u16 arr,u16 psc,u8 option);

void MX_TIM_UpdateDisable(TIM_TypeDef* TIMx,FunctionalState NewState);

void TimerX_Relaod(u16 count);
void TimerY_Relaod(u16 count);
void MX_PWM_NVIC(void);

#endif

