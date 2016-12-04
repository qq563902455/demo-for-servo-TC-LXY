/**
  ******************************************************************************
  * @file    usart.h
  * @author  Tian Chang & Luo Xiaoyi 
  * @version V1.0
  * @date    2016.10.26
  * @brief   This file contains the headers of usart.cpp
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
#ifndef __USART_H
#define __USART_H

/* C&C++ ---------------------------------------------------------------------*/
#ifdef __cplusplus
 extern "C" {
#endif

//#define DEBUG	 
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
	 
#ifdef DEBUG
	#define cout (getUsartOut())
	#define endl ("\r\n")	 	 
#endif	 
/* Exported functions ------------------------------------------------------- */
void USART1_INIT(void);	 
void USART1_IRQHandler(void);	 
#ifdef __cplusplus
}
/* Exported functions ------------------------------------------------------- */
  #ifdef DEBUG 
  //为实现cout,而实现的一个类
	class _out_stream
	{
		public:
			//重载需要使用的运算符
			const _out_stream& operator<<(int value) const;
			const _out_stream& operator<<(float value) const;
			const _out_stream& operator<<(char value) const;
			const _out_stream& operator<<(const char* value) const;
	};
  _out_stream& getUsartOut(void);
  #endif
#endif


#endif

/******************* (C) COPYRIGHT 2016 ACTION *****END OF FILE****/
