/*******************************************************************************
 * (c) Copyright 2013, Foxtone Inc., Shenzhen
 * File:        modbus.h
 * Describe:
 * Author:      LiangXiJie
 * Date:        2020/1/7
 *******************************************************************************/
#ifndef _MODBUS_H_
#define _MODBUS_H_


typedef struct
{
	unsigned char id_hi;
	unsigned char id_lo;
	unsigned char pro_hi;
	unsigned char pro_lo;
	unsigned char len_hi;
	unsigned char len;
	unsigned char slave;
}MBAP;

typedef struct
{
	MBAP			mbap;
	unsigned char	func;
	unsigned char	addr_hi;
	unsigned char	addr;
	unsigned char	count_hi;
	unsigned char	count;
}ADU_REQ;

typedef struct
{
	MBAP			mbap;
	unsigned char	func;
	unsigned char	bytecount;
	unsigned char	data[256];
}ADU_RES;

typedef struct
{
	MBAP			mbap;
	unsigned char	func;
	unsigned char	code;
}ADU_ERR;


void Modbus(unsigned char *buff, unsigned char *len);


#endif//_MODBUS_H_
/*******************************************************************************
 *                                End of File
 *******************************************************************************/
