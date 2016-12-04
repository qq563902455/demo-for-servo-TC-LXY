/**
  ******************************************************************************
  * @file    usart.cpp
  * @author  Tian Chang & Luo Xiaoyi 
  * @version V1.0
  * @date    2016.10.26
  * @brief   用于控制串口
  ******************************************************************************
  * @attention
  *
  *
  *
  * 
  ******************************************************************************
  */ 
/* Includes -------------------------------------------------------------------*/
#include "usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "string.h"
#include "stdio.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"
#include "motionCtr.h"
#include "math.h"
/* Private  typedef -----------------------------------------------------------*/
/* Private  define ------------------------------------------------------------*/

#define USART_PRINT(a)  (USART_SendData(USART1,a))

/* Private  macro -------------------------------------------------------------*/
/* Private  variables ---------------------------------------------------------*/
#ifdef DEBUG
  static _out_stream  usart_io;
#endif
/* Extern   variables ---------------------------------------------------------*/
/* Extern   function prototypes -----------------------------------------------*/
/* Private  function prototypes -----------------------------------------------*/
/* Private  functions ---------------------------------------------------------*/

/* Exported function prototypes -----------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/
/**
  * @brief   初始化驱动器串口
  * @none    none
  * @retval  none
  */
void USART1_INIT(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9, GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_9; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = 115200;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART1, &USART_InitStructure); //初始化串口3
	
  USART_Cmd(USART1, ENABLE);  //使能串口3
	
	//USART_ClearFlag(USART3, USART_FLAG_TC);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

}

void USART1_IRQHandler(void)
{
	static uint8_t ch;
	static union
  {
	 uint8_t data[24];
	 float ActVal[6];
  }posture;
	static uint8_t count=0;
	static uint8_t i=0;
	
	static float raw_x,raw_y;
	
  static motionSt pos;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE)==SET)   
	{
		USART_ClearITPendingBit( USART1,USART_IT_RXNE);
		ch=USART_ReceiveData(USART1);
		 switch(count)
		 {
			 case 0:
				 if(ch==0x0d)
					 count++;
				 else
					 count=0;
				 break;
				 
			 case 1:
				 if(ch==0x0a)
				 {
					 i=0;
					 count++;
				 }
				 else if(ch==0x0d);
				 else
					 count=0;
				 break;
				 
			 case 2:
				 posture.data[i]=ch;
			   i++;
			   if(i>=24)
				 {
					 i=0;
					 count++;
				 }
				 break;
				 
			 case 3:
				 if(ch==0x0a)
					 count++;
				 else
					 count=0;
				 break;
				 
			 case 4:
				 if(ch==0x0d)
				 {
           pos.setActAng(posture.ActVal[0]);
			     raw_x = posture.ActVal[3];
			     raw_y = posture.ActVal[4];

					 pos[0]=static_cast<float>(raw_x*cos(60.0f/180*3.1415926f)-raw_y*cos(30.0f/180*3.1415926f));
					 pos[1]=static_cast<float>(raw_y*cos(60.0f/180*3.1415926f)+raw_x*cos(30.0f/180*3.1415926f));
				 }
			   count=0;
				 break;
			 
			 default:
				 count=0;
			   break;		 
		 }
		 
		 
	 }
}




#ifdef DEBUG
/**
	* @brief   为了实现cout的逐步
	* @none
	* @retval  usart_io
  */
_out_stream& getUsartOut(void)
{
	return usart_io;
}
/**
	* @brief   重载<<，解析整型数据，并将其发送到dma
	* @param   value: 需要解析的整型数据
	* @retval  usart_io
  */
const _out_stream& _out_stream::operator<<(int value) const
{
	char buf[10];
	uint8_t len;
	len=sprintf(buf,"%d",value);
	for(uint8_t i=0;i<len;i++)
	{
		USART_PRINT(buf[i]);
	}
	return usart_io;
}
/**
	* @brief   重载<<，解析浮点型数据，并将其发送到dma
	* @param   value: 需要解析的浮点型数据
	* @retval  usart_io
  */
const _out_stream& _out_stream::operator<<(float value) const
{
	char buf[10];
	uint8_t len;
	len=sprintf(buf,"%f",value);
	for(uint8_t i=0;i<len;i++)
	{
		USART_PRINT(buf[i]);
	}
	return usart_io;
}
/**
	* @brief   重载<<，解析字符串，并将其发送到dma
	* @param   value: 需要解析的字符串
	* @retval  usart_io
  */
const _out_stream& _out_stream::operator<<(const char* value) const
{
	uint8_t len=0;
	while(1)
	{
		if(value[len]!='\0')
		  USART_PRINT(value[len]);
		else
			break;
		len++;
		
		if(len>100)
			break;
	}
	return usart_io;
}
/**
	* @brief   重载<<，解析字符型数据，并将其发送到dma
	* @param   value: 需要解析的字符型数据
	* @retval  usart_io
  */
const _out_stream& _out_stream::operator<<(char value) const
{
  USART_PRINT(value);
	return usart_io;
}
#endif
/************************ (C) COPYRIGHT 2016 ACTION *****END OF FILE****/
