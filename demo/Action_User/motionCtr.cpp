/**
  ******************************************************************************
  * @file    motionCtr.cpp  
  * @author  Tc & Lxy
  * @version v1.0
  * @date    2016.12.2
  * @brief   用于完成运动控制
  ******************************************************************************
  * @attention
  ******************************************************************************
  */ 
/* Includes -------------------------------------------------------------------*/
#include "motionCtr.h"
#include "Action_motion_control.h"
#include "action_driver.h"
/* Private  typedef -----------------------------------------------------------*/
/* Private  define ------------------------------------------------------------*/

#define CNT_CYCLE 30000
#define DIAMETER_WHEEL 127

#define PI 3.1415926f

/* Private  macro -------------------------------------------------------------*/
/* Private  variables ---------------------------------------------------------*/
float   motionSt::pos[2]={0,0};
float   motionSt::actAng=0.0f;
m_status motionSt::status=IDLE;

float motionSt::ex_vell=0;	
float   motionSt::exAngle=0.0f;  
float   motionSt::ward=0.0f;

/* Extern   variables ---------------------------------------------------------*/
/* Extern   function prototypes -----------------------------------------------*/
/* Private  function prototypes -----------------------------------------------*/
/* Private  functions ---------------------------------------------------------*/

static inline int32_t mmTcnt(float val)
{
	return val/PI/DIAMETER_WHEEL*CNT_CYCLE;
}

/* Exported function prototypes -----------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/
void motionSt::oLine(int32_t rotate)
{
	wheel_speed speed;
	speed=basicLine(ex_vell,ward,rotate,actAng);
	
	Action_Set_Velocity(1,mmTcnt(speed.v1));
	Action_Set_Velocity(2,mmTcnt(speed.v2));
	Action_Set_Velocity(3,mmTcnt(speed.v3));
}
void motionSt::origin(int32_t x,int32_t y)
{
	setOrigin(x,y);
}
void motionSt::cLine(void)
{
	wheel_speed speed;
	speed=closeLoopLine(ex_vell,ward,exAngle,actAng,pos[0],pos[1]);
	Action_Set_Velocity(1,mmTcnt(speed.v1));
	Action_Set_Velocity(2,mmTcnt(speed.v2));
	Action_Set_Velocity(3,mmTcnt(speed.v3));
}
void motionSt::point(int32_t x,int32_t y)
{
	wheel_speed speed;
	speed=point_to_point(ex_vell,x,y,pos[0], pos[1],exAngle,actAng);
	if(speed.status!=SUCCESS_A)
	{
		status=BUSY;
	}
	else
	{
		status=IDLE;
	}
	Action_Set_Velocity(1,mmTcnt(speed.v1));
	Action_Set_Velocity(2,mmTcnt(speed.v2));
	Action_Set_Velocity(3,mmTcnt(speed.v3));
}
float& motionSt::operator[](uint8_t num)
{
	if(num<=2)
		return pos[num];
	else
		while(1);
}

/************************ (C) COPYRIGHT 2016 ACTION *****END OF FILE****/
