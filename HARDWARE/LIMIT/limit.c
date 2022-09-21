#include "limit.h"
#include "motor.h"
#include "usart.h"
#include "drv8889.h"

void LIMIT_Init(void)
{
	GPIO_InitTypeDef      GPIO_Initstructure;
	RCC_LIMIT_CLOCK();

	GPIO_Initstructure.Pin = LIMIT1_PIN|LIMIT2_PIN|LIMIT3_PIN|LIMIT4_PIN;
	GPIO_Initstructure.Mode = GPIO_MODE_IT_FALLING;
	GPIO_Initstructure.Pull = GPIO_PULLUP;
	GPIO_Initstructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init( LIMIT_PORT, &GPIO_Initstructure );

	HAL_GPIO_WritePin(LIMIT_PORT,  LIMIT1_PIN|LIMIT2_PIN|LIMIT3_PIN|LIMIT4_PIN,GPIO_PIN_RESET);
}

void EXTIX_Init(void)
{

	LIMIT_Init();
	__HAL_RCC_SYSCFG_CLK_ENABLE();	
	
	//中断线2-PE2
    HAL_NVIC_SetPriority(EXTI2_IRQn,2,0);       //抢占优先级为2，子优先级为0
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);             //使能中断线0
    
    //中断线3-PE3
    HAL_NVIC_SetPriority(EXTI3_IRQn,2,1);       //抢占优先级为2，子优先级为1
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);             //使能中断线2
    
    //中断线4-PE4
    HAL_NVIC_SetPriority(EXTI4_IRQn,2,2);       //抢占优先级为2，子优先级为2
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);             //使能中断线2
    
    //中断线5-PE5
    HAL_NVIC_SetPriority(EXTI9_5_IRQn,2,3);   	//抢占优先级为2，子优先级为3
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);         	//使能中断线4

}




void EXTI2_IRQHandler(void)
{
	if(0 == LIMIT1)
	{
		MotorStop( Motor_x, CURRENT_25_PERCENT);
	}
	__HAL_GPIO_EXTI_CLEAR_IT( LIMIT1_PIN);
}

void EXTI3_IRQHandler(void)
{
	if(0 == LIMIT2)
	{
		MotorStop( Motor_y, CURRENT_25_PERCENT);
	}
	__HAL_GPIO_EXTI_CLEAR_IT( LIMIT2_PIN);
}
void EXTI4_IRQHandler(void)
{
	if(0 == LIMIT3)
	{
		MotorStop( Motor_x, CURRENT_25_PERCENT);
	}
	__HAL_GPIO_EXTI_CLEAR_IT( LIMIT3_PIN);
}
void EXTI9_5_IRQHandler(void)
{
	if(0 == LIMIT4)
	{
		MotorStop( Motor_y, CURRENT_25_PERCENT);
	}
	__HAL_GPIO_EXTI_CLEAR_IT( LIMIT4_PIN);
}
