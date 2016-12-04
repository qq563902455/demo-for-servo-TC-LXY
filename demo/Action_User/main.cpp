/**  
  ******************************************************************************
  * @file    main.cpp
  * @author  Tian Chang & Luo Xiaoyi 
  * @version V1.0
  * @date    2016.10.27
  * @brief   程序的初始
  ******************************************************************************
  * @attention
  ******************************************************************************
  */ 
/* Includes -------------------------------------------------------------------*/
#include "timer.h"
#include "misc.h"
#include "action_driver.h"
#include "can.h"
#include "stm32f4xx_gpio.h"
#include "motionCtr.h"
#include "usart.h"
/* Private  typedef -----------------------------------------------------------*/
/* Private  define ------------------------------------------------------------*/
/* Private  macro -------------------------------------------------------------*/
/* Private  variables ---------------------------------------------------------*/
/* Extern   variables ---------------------------------------------------------*/
/* Extern   function prototypes -----------------------------------------------*/
/* Private  function prototypes -----------------------------------------------*/
static void running(void);
/* main functions -------------------------------------------------------------*/
/**
  * @brief   代码开始执行的地方 
  * @none    none
  * @retval  none
  */
int main(void)
{	
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2);
  TIM_Init(TIM2,999,839,0,0);
	CAN_Config(CAN1,500,GPIOD,GPIO_Pin_0,GPIO_Pin_1);
	USART1_INIT();
	
	Action_Set_Operational_Mode(1);
	Action_Set_Operational_Mode(2);
	Action_Set_Operational_Mode(3);
	
	Action_Config_Vel(1,200000,200000);
	Action_Config_Vel(2,200000,200000);
	Action_Config_Vel(3,200000,200000);
	
	TIM_Delayms(TIM3,10000);
	
  running();
}
/* Private  functions ---------------------------------------------------------*/
/**
  * @brief   初始化配置各种外设
  * @none    none
  * @retval  none
  */
static void running(void)
{
	motionSt pos;
	pos.origin();
	pos.ex_vell=100;
	pos.ward=90;
	while(1)
	{
		while(getTimeFlag());
		
		
		pos.oLine();
		
//		pos.point(0,1500);
//		
//		if(pos.getStatus()==IDLE)
//		{
//			pos.ex_vell=0;
//			pos.oLine();
//			while(1);
//		}
		
	}
}


/************************ (C) COPYRIGHT 2016 ACTION *****END OF FILE****/
