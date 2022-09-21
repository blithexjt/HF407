#ifndef __LIMIT_H
#define __LIMIT_H
#include "sys.h"

#define  LIMIT_PORT       GPIOE

#define  LIMIT1_PIN	     GPIO_PIN_2
#define  LIMIT2_PIN	     GPIO_PIN_3
#define  LIMIT3_PIN	     GPIO_PIN_4
#define  LIMIT4_PIN	     GPIO_PIN_5

#define  LIMIT1               HAL_GPIO_ReadPin(LIMIT_PORT,  LIMIT1_PIN)
#define  LIMIT2               HAL_GPIO_ReadPin(LIMIT_PORT,  LIMIT2_PIN)
#define  LIMIT3               HAL_GPIO_ReadPin(LIMIT_PORT,  LIMIT3_PIN)
#define  LIMIT4               HAL_GPIO_ReadPin(LIMIT_PORT,  LIMIT4_PIN)

#define  RCC_LIMIT_CLOCK()      __HAL_RCC_GPIOE_CLK_ENABLE()

void LIMIT_Init(void);

void EXTIX_Init(void);
#endif
