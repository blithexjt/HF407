#include "timer.h"
#include "led.h"
#include "usart.h"
#include "motor.h"
#include "drv8889.h"
#include "delay.h"

TIM_HandleTypeDef TIM1_Handler;      //定时器句柄 
TIM_HandleTypeDef TIM2_Handler;      //定时器句柄 
TIM_HandleTypeDef TIM3_Handler;      //定时器句柄 
TIM_HandleTypeDef TIM4_Handler;      //定时器句柄 
TIM_HandleTypeDef TIM6_Handler;      //定时器句柄 
TIM_HandleTypeDef TIM9_Handler;      //定时器句柄 
TIM_HandleTypeDef TIM10_Handler;      //定时器句柄 


extern u32 lwip_localtime;	         //lwip本地时间计数器,单位:ms
 u32 CurrentNum_x;
 u32 CurrentNum_y;

void RGBLED_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12   \
												|GPIO_PIN_13|GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	HAL_GPIO_WritePin( GPIOE, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14,GPIO_PIN_RESET);
	
}

void TIM1_Init()
{  
	TIM_OC_InitTypeDef  sXConfigOC = {0};
	
	RGBLED_Init();
	
    TIM1_Handler.Instance=TIM1;                          //通用定时器1
    TIM1_Handler.Init.Prescaler = (168-1);                     //分频系数
    TIM1_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;    //向上计数器
    TIM1_Handler.Init.Period = (1000-1);                        //自动装载值
    TIM1_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM1_Handler);
    
	sXConfigOC.OCMode = TIM_OCMODE_PWM2;
	sXConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sXConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sXConfigOC.Pulse = 500;
	sXConfigOC.OCIdleState = TIM_OCIDLESTATE_SET;
	sXConfigOC.OCNIdleState = TIM_OCNIDLESTATE_SET;
	sXConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&TIM1_Handler, &sXConfigOC, TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(&TIM1_Handler, &sXConfigOC, TIM_CHANNEL_2);
	HAL_TIM_PWM_ConfigChannel(&TIM1_Handler, &sXConfigOC, TIM_CHANNEL_3);
	
	
    HAL_TIM_PWM_Start(&TIM1_Handler,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&TIM1_Handler,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&TIM1_Handler,TIM_CHANNEL_3);

	HAL_TIMEx_PWMN_Start(&TIM1_Handler,TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&TIM1_Handler,TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&TIM1_Handler,TIM_CHANNEL_3);
}



void TIM2_Init(u16 arr,u16 psc)
{  
    TIM2_Handler.Instance=TIM2;                         
    TIM2_Handler.Init.Prescaler=psc;                     //分频系数
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM2_Handler.Init.Period=arr;                        //自动装载值
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM2_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM2_Handler); 
}
void TIM6_Init(u16 psc)
{  
    TIM6_Handler.Instance=TIM6;                          //通用定时器6
    TIM6_Handler.Init.Prescaler=psc;                     //分频系数
    TIM6_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM6_Handler.Init.Period=5000-1;                        //自动装载值
    TIM6_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM6_Handler);
    
    HAL_TIM_Base_Start(&TIM6_Handler); //使能定时器6和定时器6更新中断：TIM_IT_UPDATE   
}

void TIM3_Init()
{  
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_SlaveConfigTypeDef sSlaveConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	
	TIM3_Handler.Instance=TIM3;                          //通用定时器3
    TIM3_Handler.Init.Prescaler = 0;                     //分频系数
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM3_Handler.Init.Period=(500-1);                       //自动装载值
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM3_Handler);
    

	TIM3->SMCR|=0x07;
	sSlaveConfig.SlaveMode = TIM_SLAVEMODE_EXTERNAL1;
	sSlaveConfig.InputTrigger = TIM_TS_ITR3;
	sSlaveConfig.TriggerPolarity = TIM_TRIGGERPOLARITY_RISING;  
    sSlaveConfig.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1;  
	HAL_TIM_SlaveConfigSynchronization(&TIM3_Handler,&sSlaveConfig);
	
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&TIM3_Handler,&sMasterConfig);
	
	__HAL_TIM_URS_ENABLE(&TIM3_Handler);
	HAL_TIM_Base_Stop_IT(&TIM3_Handler);
}
void TIM4_Init()
{  
	TIM_MasterConfigTypeDef  sMasterXConfig = {0};
	TIM_OC_InitTypeDef  sXConfigOC = {0};
	
    TIM4_Handler.Instance=TIM4;                          
    TIM4_Handler.Init.Prescaler = (168-1);                     //分频系数
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM4_Handler.Init.Period=(500-1);                        //自动装载值
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
	HAL_TIM_PWM_Init(&TIM4_Handler); 
	
	sMasterXConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterXConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
	HAL_TIMEx_MasterConfigSynchronization(&TIM4_Handler,&sMasterXConfig);
	
	sXConfigOC.OCMode = TIM_OCMODE_PWM1;
	sXConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
	sXConfigOC.Pulse = (250-1);
	sXConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&TIM4_Handler, &sXConfigOC, TIM_CHANNEL_1);
	
	HAL_TIM_PWM_Stop(&TIM4_Handler, TIM_CHANNEL_1);

}

void TIM9_Init()
{  
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_SlaveConfigTypeDef sSlaveConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	
	TIM9_Handler.Instance=TIM9;                         
    TIM9_Handler.Init.Prescaler = 0;                     //分频系数
    TIM9_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM9_Handler.Init.Period = 5000;                        //自动装载值
    TIM9_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM9_Handler);
    

	TIM9->SMCR|=0x07;
	sSlaveConfig.SlaveMode = TIM_SLAVEMODE_EXTERNAL1;
	sSlaveConfig.InputTrigger = TIM_TS_ITR2;
	sSlaveConfig.TriggerPolarity = TIM_TRIGGERPOLARITY_RISING;  
    sSlaveConfig.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1;  
	HAL_TIM_SlaveConfigSynchronization(&TIM9_Handler,&sSlaveConfig);
	
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&TIM9_Handler,&sMasterConfig);
	
	__HAL_TIM_URS_ENABLE(&TIM9_Handler);
	HAL_TIM_Base_Stop_IT(&TIM9_Handler);
}
void TIM10_Init()
{  
	TIM_MasterConfigTypeDef  sMasterXConfig = {0};
	TIM_OC_InitTypeDef  sXConfigOC = {0};
	
    TIM10_Handler.Instance=TIM10;                          
    TIM10_Handler.Init.Prescaler = 1;                     //分频系数
    TIM10_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM10_Handler.Init.Period= 5000;                        //自动装载值
    TIM10_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM10_Handler);
	HAL_TIM_PWM_Init(&TIM10_Handler); 

	sMasterXConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterXConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
	HAL_TIMEx_MasterConfigSynchronization(&TIM10_Handler,&sMasterXConfig);
	
	sXConfigOC.OCMode = TIM_OCMODE_PWM1;
	sXConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
	sXConfigOC.Pulse = 2500;
	sXConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&TIM10_Handler, &sXConfigOC, TIM_CHANNEL_1);
	
    HAL_TIM_PWM_Stop(&TIM10_Handler, TIM_CHANNEL_1);

}

void MX_PWM_NVIC()
{
	HAL_NVIC_SetPriority(TIM3_IRQn,0,0);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	HAL_NVIC_SetPriority(TIM1_BRK_TIM9_IRQn,0,1);
	HAL_NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim)
{
	if(htim->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();           
		HAL_NVIC_SetPriority(TIM2_IRQn,1,3);   
		HAL_NVIC_EnableIRQ(TIM2_IRQn);            
	}
}

void PWM_GPIOInit()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.Alternate = GPIO_AF3_TIM10;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	MX_PWM_NVIC();
}

void TIM4_Master_TIM3_Slave(u32 PulseNum1,u32 arr,u32 psc)
{
       u32 ccr = (((arr+1)/2)-1);
	CurrentNum_x = PulseNum1;
	if(PulseNum1 > PULSENUM_MAX)	
		PulseNum1 = PULSENUM_MAX;
	CurrentNum_x = CurrentNum_x - PulseNum1;
	__HAL_TIM_CLEAR_FLAG(&TIM3_Handler, TIM_FLAG_UPDATE);
	__HAL_TIM_SET_PRESCALER(&TIM4_Handler,psc);

	__HAL_TIM_SET_AUTORELOAD(&TIM4_Handler, arr);
	
	__HAL_TIM_SET_COMPARE(&TIM4_Handler,TIM_CHANNEL_1, ccr);
	
	__HAL_TIM_SET_AUTORELOAD(&TIM3_Handler, PulseNum1);

	HAL_TIM_Base_Start_IT(&TIM3_Handler);

	HAL_TIM_PWM_Start(&TIM4_Handler, TIM_CHANNEL_1);
	
}
void TIM10_Master_TIM9_Slave(u32 PulseNum1,u32 arr,u32 psc)
{
    u32 ccr = (((arr+1)/2)-1);
	CurrentNum_y = PulseNum1;
	if(PulseNum1 > PULSENUM_MAX)	
		PulseNum1 = PULSENUM_MAX;
	CurrentNum_y = CurrentNum_y - PulseNum1;
	
	TIM9_Init();
	TIM10_Init();
	__HAL_TIM_SET_PRESCALER(&TIM10_Handler,psc);
	__HAL_TIM_SET_AUTORELOAD(&TIM10_Handler, arr);
	__HAL_TIM_SET_COMPARE(&TIM10_Handler,TIM_CHANNEL_1, ccr);
	HAL_TIM_GenerateEvent(&TIM10_Handler,  ENABLE);

	__HAL_TIM_SET_AUTORELOAD(&TIM9_Handler, PulseNum1 );
	HAL_TIM_GenerateEvent(&TIM9_Handler,  ENABLE);

	__HAL_TIM_CLEAR_FLAG(&TIM9_Handler, TIM_FLAG_UPDATE);
	HAL_TIM_Base_Start_IT(&TIM9_Handler);
	HAL_TIM_PWM_Start(&TIM10_Handler, TIM_CHANNEL_1);
	HAL_TIM_Base_Start(&TIM10_Handler);

}
void Motor_PWM(u16 MotorNum,u32 arr,u32 psc,u32 PulseNum)
{
	switch (MotorNum)
	{
		case Motor_x:
			TIM4_Master_TIM3_Slave(PulseNum,arr,psc);
			break;
		case Motor_y:
			TIM10_Master_TIM9_Slave(PulseNum,arr,psc);
			break;
	}
}
void TIM_update(TIM_HandleTypeDef * TIMx,u16 arr,u16 psc,u8 option)
{
	u16 ccr = (((arr+1)/2)-1);
	
	MX_TIM_UpdateDisable(TIMx->Instance,DISABLE);
	
    __HAL_TIM_SET_AUTORELOAD(TIMx, (arr));
    __HAL_TIM_SET_PRESCALER(TIMx,psc);
	__HAL_TIM_SET_COMPARE(TIMx,TIM_CHANNEL_1, (ccr));
	
	MX_TIM_UpdateDisable(TIMx->Instance,ENABLE);
	delay_ms(1);
	if(option == immediately)  
		HAL_TIM_GenerateEvent(TIMx,  ENABLE);
	HAL_TIM_PWM_Start(TIMx, TIM_CHANNEL_1);
}
void StepInit()
{
	PWM_GPIOInit();
	TIM3_Init();
	TIM4_Init();
	TIM9_Init();
	TIM10_Init();
}
void MX_TIM_UpdateDisable(TIM_TypeDef* TIMx,FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    /* Set the Update Disable Bit */
    TIMx->CR1 |= TIM_CR1_UDIS;
  }
  else
  {
    /* Reset the Update Disable Bit */
    TIMx->CR1 &= (uint16_t)~TIM_CR1_UDIS;
  }
}



void TimerX_Relaod(u16 count)
{ 
	__HAL_TIM_SET_AUTORELOAD(&TIM3_Handler, (count-1) );
	HAL_TIM_GenerateEvent(&TIM3_Handler,  ENABLE);
	__HAL_TIM_CLEAR_FLAG(&TIM3_Handler, TIM_FLAG_UPDATE);
	HAL_TIM_Base_Start_IT(&TIM3_Handler);
}

void TimerY_Relaod(u16 count)
{ 
	__HAL_TIM_SET_AUTORELOAD(&TIM9_Handler, (count-1) );
	HAL_TIM_GenerateEvent(&TIM9_Handler,  ENABLE);
	__HAL_TIM_CLEAR_FLAG(&TIM9_Handler, TIM_FLAG_UPDATE);
	HAL_TIM_Base_Start_IT(&TIM9_Handler);
}



void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM2_Handler);
}


//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	if(__HAL_TIM_GET_FLAG(&TIM3_Handler, TIM_FLAG_UPDATE) != RESET)
	{
		__HAL_TIM_CLEAR_FLAG(&TIM3_Handler, TIM_FLAG_UPDATE);
		if(CurrentNum_x)
		{	
			if(CurrentNum_x > PULSENUM_MAX)
			{
				CurrentNum_x -= PULSENUM_MAX;
				TimerX_Relaod(PULSENUM_MAX);		
			}
			else
			{
				TimerX_Relaod(CurrentNum_x);
				CurrentNum_x = 0;
			}
		}
		else
		{
			  HAL_TIM_PWM_Stop(&TIM4_Handler, TIM_CHANNEL_1);
			  HAL_TIM_Base_Stop_IT(&TIM3_Handler);
		}
	  }

}

void TIM1_BRK_TIM9_IRQHandler(void)
{

	if(__HAL_TIM_GET_FLAG(&TIM9_Handler, TIM_FLAG_UPDATE) != RESET)
	{
		__HAL_TIM_CLEAR_FLAG(&TIM9_Handler, TIM_FLAG_UPDATE);
		 if(__HAL_TIM_GET_IT_SOURCE(&TIM9_Handler, TIM_IT_UPDATE) !=RESET)
		    {
		      __HAL_TIM_CLEAR_IT(&TIM9_Handler, TIM_IT_UPDATE);
			  if(CurrentNum_y)
				{	
					if(CurrentNum_y > PULSENUM_MAX)
					{
						CurrentNum_y -= PULSENUM_MAX;
						TimerY_Relaod(PULSENUM_MAX);		
					}
					else
					{
						TimerY_Relaod(CurrentNum_y);
						CurrentNum_y = 0;
					}
				}
				else
				{
					  HAL_TIM_PWM_Stop(&TIM10_Handler, TIM_CHANNEL_1);
					  HAL_TIM_Base_Stop_IT(&TIM9_Handler);
				}
		    }
		
	  }	  

}

//回调函数，定时器中断服务函数调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == (&TIM2_Handler))
    {
       lwip_localtime +=10; //加10
		return;
    }

}


