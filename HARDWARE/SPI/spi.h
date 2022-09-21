#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

extern SPI_HandleTypeDef hspi3;

void SPI3_Init(void);

u8 SPI3_ReadWriteByte(u8 TxData);

void SPI3_SetSpeed(u8 SPI_BaudRatePrescaler);

#endif


