/**
  ******************************************************************************
  * @file    *.h
  * @author  Lxy Action
  * @version 
  * @date   
  * @brief   This file contains the headers of 
  ******************************************************************************
  * @attention
  *
  *
  * 
  * 
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CONTROL_H
#define __CONTROL_H

/* C&C++ ---------------------------------------------------------------------*/
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"	 
/* Exported types ------------------------------------------------------------*/
enum m_status
{
	BUSY=0,
	IDLE
};
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#ifdef __cplusplus
}
#endif
/* Includes C++ ---------------------------------------------------------------*/
/* Exported Class -------------------------------------------------------------*/
class motionSt
{
	private:
		static int32_t pos[2];
	  static float   actAng;
	  static m_status status;
	public:
    static int32_t ex_vell;	
	  static float   exAngle;  
	  static float   ward;
	
	  void oLine(int32_t rotate=0);
	  
	  void origin(int32_t x=motionSt::pos[0],int32_t y=motionSt::pos[1]);
	  void cLine(void);
	  void point(int32_t,int32_t);
	
	  inline m_status getStatus(void) const{return  motionSt::status;}
	  inline float    getActAng(void) const{return  motionSt::actAng;}
	  inline void     setActAng(float ang){motionSt::actAng=ang;}
	
	  int32_t  operator[] (uint8_t);
};
#endif
/******************* (C) COPYRIGHT 2016 ACTION *****END OF FILE****/
