/*******************************************************************************
 * (c) Copyright 2013, Foxtone Inc., Shenzhen
 * File:        modbus.c
 * Describe:
 * Author:      LiangXiJie
 * Date:        2013-2-26
 *******************************************************************************/
#include "modbus.h"


#define LOBYTE(x)	((unsigned char)(x))
#define HIBYTE(x)	(((unsigned short)(x))>>8)
#define FILL_VAL(buf,val)	buf[0]=HIBYTE(val);buf[1]=LOBYTE(val)

unsigned char GetRegs(unsigned short addr,unsigned short count,unsigned char *buff);
unsigned char SetReg(unsigned short addr,unsigned short val);


/******************************************************************************
 * Function: Modbus
 * Describe: 
 ******************************************************************************/
void Modbus(unsigned char *buff, unsigned char *len)
{
	ADU_REQ *pRequest = (ADU_REQ*)buff;
	ADU_RES *pResponse = (ADU_RES*)buff;
	ADU_ERR *pError = (ADU_ERR*)buff;
	unsigned short addr;
	unsigned short val;
	unsigned char err;

	if(*len != sizeof(ADU_REQ)) pError->func |= 0x80;

	switch(pRequest->func)
	{
		case 0x03:
		case 0x04:
			addr = pRequest->addr_hi;
			addr *= 256;
			addr += pRequest->addr;
			val = pRequest->count;
			err = GetRegs(addr,val,pResponse->data);
			if(!err){
				pResponse->bytecount = val * 2;
				*len = pResponse->bytecount + 1 + 1 + sizeof(MBAP);
			}else{
				pError->func |= 0x80;
				pError->code = err;
				*len = 1 + 1 + sizeof(MBAP);
			}
			break;

		case 0x06:
			addr = pRequest->addr_hi;
			addr *= 256;
			addr += pRequest->addr;
			val = pRequest->count_hi;
			val *= 256;
			val += pRequest->addr;
			err = SetReg(addr,val);
			if(!err){
				//return back;
			}else{
				pError->func |= 0x80;
				pError->code = err;
				*len = 1 + 1 + sizeof(MBAP);
			}
			break;

		default:
			pError->func |= 0x80;
			pError->code = 1;
			*len = 1 + 1 + sizeof(MBAP);
			break;
	}
}

/******************************************************************************
 * Function: GetRegs
 * Describe: 
 ******************************************************************************/
unsigned char GetRegs(unsigned short addr,unsigned short count,unsigned char *buff)
{
	unsigned short i;
	//unsigned short k;
	for(i = 0; i < count; i++)
	{
		switch(addr+1)
		{
			default:
				FILL_VAL(buff, -1);
				break;
		}
		addr ++;
		buff += 2;
	}
	return 0;
}

/******************************************************************************
 * Function: SetRegs
 * Describe: 
 ******************************************************************************/
unsigned char SetReg(unsigned short addr,unsigned short val)
{
	switch(addr+1)
	{
		default:
			return 2;
	}
	return 0;
}


/*******************************************************************************
 *                                End of File
 *******************************************************************************/
