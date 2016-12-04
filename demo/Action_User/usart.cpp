/**
  ******************************************************************************
  * @file    usart.cpp
  * @author  Tian Chang & Luo Xiaoyi 
  * @version V1.0
  * @date    2016.10.26
  * @brief   ���ڿ��ƴ���
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
  * @brief   ��ʼ������������
  * @none    none
  * @retval  none
  */
void USART1_INIT(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9, GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_9; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = 115200;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure); //��ʼ������3
	
  USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���3
	
	//USART_ClearFlag(USART3, USART_FLAG_TC);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

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
	* @brief   Ϊ��ʵ��cout����
	* @none
	* @retval  usart_io
  */
_out_stream& getUsartOut(void)
{
	return usart_io;
}
/**
	* @brief   ����<<�������������ݣ������䷢�͵�dma
	* @param   value: ��Ҫ��������������
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
	* @brief   ����<<���������������ݣ������䷢�͵�dma
	* @param   value: ��Ҫ�����ĸ���������
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
	* @brief   ����<<�������ַ����������䷢�͵�dma
	* @param   value: ��Ҫ�������ַ���
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
	* @brief   ����<<�������ַ������ݣ������䷢�͵�dma
	* @param   value: ��Ҫ�������ַ�������
	* @retval  usart_io
  */
const _out_stream& _out_stream::operator<<(char value) const
{
  USART_PRINT(value);
	return usart_io;
}
#endif
/************************ (C) COPYRIGHT 2016 ACTION *****END OF FILE****/
