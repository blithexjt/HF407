#ifndef MOTOR_H
#define MOTOR_H

#include "sys.h"
#include "timer.h"

#define	MOTORX_DIR	PDout(3)


#define MOTORX_DIR_Forward()	    HAL_GPIO_WritePin( DIR_port, DIR_pin,GPIO_PIN_SET);
#define MOTORX_DIR_Backward()	    HAL_GPIO_WritePin( DIR_port, DIR_pin,GPIO_PIN_RESET);

#define MOTORY_DIR_Forward()	    HAL_GPIO_WritePin( DIR_Y_port, Dir_Y_pin,GPIO_PIN_SET);
#define MOTORY_DIR_Backward()	    HAL_GPIO_WritePin( DIR_Y_port, Dir_Y_pin,GPIO_PIN_RESET);
#define DRV8889  1

#define Move_3_2_CM   (200*100*(3.2))

// extern u32 g_positionx = 0;
// extern u32 g_positiony = 0;

//extern  u32 g_speed = 0;

//extern  u32 g_pulsex = 0;
//extern  u32 g_pulsey = 0;

//extern  s32 g_stepx = 0;
//extern  s32 g_stepy = 0;

// extern u8 g_HoldCurrent = 0;
 

typedef enum
{
    HoldCurrent_en = 0,
    HoldCurrent_non,
}HoldCurrent;

//typedef enum
//{
//	Motor_x = 0,
//	Motor_y ,
//}MOTOR_NUM;

void MotorInit(void);

void MotorStart(u8 motor,u8 current);
void MotorStop(u8 motor,u8 current);

void MotorSetSpeed(u8 motor,float speed);
void MotorSetCurrent(u8 Motor, u8 current);

void MoveBackward(u8 Motor,u32 arr,u32 psc,u32 step);
void MoveForward(u8 Motor, u32 arr,u32 psc,u32 step);

void MotorRun(u8 Motor, u32 current,float speed, double length);

//read the stall value and printf it
void ReadMotorSTL(void);

void Set_Fsq_Count(u8 Motor,u8 num);
 #endif
