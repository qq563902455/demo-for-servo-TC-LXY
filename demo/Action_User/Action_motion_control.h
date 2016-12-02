/**
******************************************************************************
* @file    Action_motion_control.h
* @author  Action
* @version V1.0
* @date    2016.04.04
* @brief   This file contains the headers of motion function
******************************************************************************
*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AMC_H
#define __AMC_H

#ifdef __cplusplus  //为C提供接口
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
/* Exported types ------------------------------------------------------------*/
typedef struct
{
	float v1;
	float v2;
	float v3;
	uint8_t status;
}wheel_speed;
/* Exported constants --------------------------------------------------------*/
#define SUCCESS_A    1
#define FAILURE_A    0
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void setOrigin(float pos_x, float pos_y);

wheel_speed basicLine(float vel, float ward, float Rotate,float selfAngle);

wheel_speed closeLoopLine(float vel,float ward,float ex_Ang, float act_Ang,float pos_x,float pos_y);

wheel_speed point_to_point(float vel,
	                       float end_posx, float end_posy,
	                       float pos_x, float pos_y,
	                       float ex_Ang, float act_Ang);
#ifdef __cplusplus //为C提供接口
}
#endif
#endif

/******************* (C) COPYRIGHT 2015 ACTION *****END OF FILE****/
