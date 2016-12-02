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
/* Private  typedef -----------------------------------------------------------*/
/* Private  define ------------------------------------------------------------*/
/* Private  macro -------------------------------------------------------------*/
/* Private  variables ---------------------------------------------------------*/
int32_t motionSt::pos[2]={0,0};
float   motionSt::actAng=0.0f;
m_status motionSt::status=IDLE;

int32_t motionSt::ex_vell=0;	
float   motionSt::exAngle=0.0f;  
float   motionSt::ward=0.0f;

/* Extern   variables ---------------------------------------------------------*/
/* Extern   function prototypes -----------------------------------------------*/
/* Private  function prototypes -----------------------------------------------*/
/* Private  functions ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/
inline void motionSt::oLine(int32_t rotate)
{
	basicLine(ex_vell,ward,rotate,actAng);
}
inline void motionSt::origin(int32_t x,int32_t y)
{
	setOrigin(x,y);
}
inline void motionSt::cLine(void)
{
	closeLoopLine(ex_vell,ward,exAngle,actAng,pos[0],pos[1]);
}
inline void motionSt::point(int32_t x,int32_t y)
{
	wheel_speed speed;
	speed=point_to_point(ex_vell,x,y,pos[0], pos[1],exAngle,actAng);
	if(speed.status!=SUCCESS)
	{
		status=BUSY;
	}
	else
	{
		status=IDLE;
	}
}
int32_t motionSt::operator[](uint8_t num)
{
	if(num<=2)
		return pos[num];
	else
		while(1);
}

/************************ (C) COPYRIGHT 2016 ACTION *****END OF FILE****/
