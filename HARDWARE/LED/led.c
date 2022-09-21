#include "led.h"
/**************************************************
 * file		led.c
 * ver		V1.0
 * date		2022.07.20
 ***************************************************/

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();           //����GPIOFʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_6; 
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);	//PA6��1��Ĭ�ϳ�ʼ�������
}


