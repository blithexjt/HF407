#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "spi.h"
#include "lan8720.h"
#include "drv8889.h"
#include "ucommand.h"
#include "motor.h"
#include "force.h"
#include "queue.h"
#include "limit.h"
#include "timer.h"
#include "malloc.h" 
#include "lwip/netif.h"
#include "lwip_comm.h"
#include "lwipopts.h"

void rcc_clk_init(void);
void tcp_server_init(void);
u32 TickTimer=0;
extern u32 lwip_localtime;

int main(void)
{
    HAL_Init();                   	//初始化HAL库    
    Stm32_Clock_Init(336,8,2,7);  	//设置时钟,168Mhz
	rcc_clk_init();
	delay_init(168);               	//初始化延时函数
	USART_DMA_Init();
		
	TIM1_Init();
    TIM2_Init(1000-1,840-1);        //定时器3初始化,周期10ms
	TIM6_Init(8400-1);
	StepInit();
	
	usart1_init(115200);             	//初始化USART
	usart3_init(115200);             	//初始化USART
	
	LED_Init();						//初始化LED	
	BEEP_Init();					//初始化蜂鸣器
	MotorInit();
	M8128_Init();
	EXTIX_Init();
	
	
	my_mem_init(SRAMIN);			//初始化内部内存池
	my_mem_init(SRAMCCM);			//初始化CCM内存池
	
	while(lwip_comm_init())         //lwip初始化
	{
		delay_ms(1000);
	}

    tcp_server_init();

	while(1)
	{
		lwip_periodic_handle();

        if(lwip_localtime - TickTimer >= 250)
        {
            TickTimer = lwip_localtime;
            LED = !LED;
        }
				//USART MOTOR 
		CheckUartCommand();
//		// USART FORCE
		M8128_Config();	
//		// FORCE receive and paramet
			RevCommnd();
		//printf MOTOR STALL VALUE
//		Debug_printf();	
		
	}
}

void rcc_clk_init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();

	__HAL_RCC_DMA1_CLK_ENABLE();
	__HAL_RCC_DMA2_CLK_ENABLE();
	__HAL_RCC_USART1_CLK_ENABLE();
	__HAL_RCC_USART3_CLK_ENABLE();

	__HAL_RCC_TIM1_CLK_ENABLE();
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_TIM3_CLK_ENABLE();
	__HAL_RCC_TIM4_CLK_ENABLE();
	__HAL_RCC_TIM6_CLK_ENABLE();
	__HAL_RCC_TIM9_CLK_ENABLE();
	__HAL_RCC_TIM10_CLK_ENABLE();

	__HAL_RCC_SPI3_CLK_ENABLE();
	__HAL_RCC_FSMC_CLK_ENABLE();
}
