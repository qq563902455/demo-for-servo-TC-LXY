  /**
  ******************************************************************************
  * @file    timer.c
  * @author  Calcus Lee
  * @version V1.0.1
  * @date    9-August-2013
  * @brief   functions of time
  ******************************************************************************
  * @attention
  * three are some great and accurency time delay function in timer.h
  *
**/

#include "timer.h"
#include "misc.h"
#include "stm32f4xx_rcc.h"



//精确延时函数调用wait
void  wait(uint32_t n)
{
	do{
	n--;
	}while(n);
}

/**
  * @brief  Configures the TIMx's interrupt time.
  * @param  TIMx: where x can be 1-14. 
			@note TIM1 and TIM 9,10,11;TIM2 and TIM12,13,14 have some sharing IRQ handle functions.
  * @param  arr: the period value to be loaded into the active
                 Auto-Reload Register at the next update event, range from 1 to 65535
  * @param  psr: the prescaler value used to divide the TIM clock, range from 1 to 65535
  * @note   1.the TIMx's PreemptionPriority and SubPriority has been predefined in this function,
			change them according to your need.
			2.Tout= ((arr+1)*(psc+1))/Tclk s, 
			  for TIM2-7 and TIM12-14, Tclk=84M
			  for TIM1,8,9,10,11 Tclk=168M
			  exp: for TIM2, if arr=999, psc=83, then Tout=(1000*84)/84M=1ms
  * @retval None
  * @author Calcus Lee
  */
void TIM_Init(TIM_TypeDef * TIMx, uint16_t arr, uint16_t psr,uint16_t prepri,uint16_t subpri)
{
	TIM_TimeBaseInitTypeDef TIMx_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
		
	switch((uint32_t)TIMx)
	{
		//APB2 TIM
		case TIM1_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_TIM10_IRQn ;     
			
			break;
		}
		case TIM8_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel=TIM8_UP_TIM13_IRQn ;    
			
			break;
		}
		case TIM9_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel=TIM1_BRK_TIM9_IRQn ;    
			
			break;
		}
		case TIM10_BASE:
		{
			RCC_APB2PeriphClockCmd(TIM1_UP_TIM10_IRQn, ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel=TIM8_UP_TIM13_IRQn ;    
			
			break;
		}
		case TIM11_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel=TIM1_TRG_COM_TIM11_IRQn ;   
			
			break;
		}
		//APB1 TIM
		case TIM2_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn ;     

			break;
		}
		case TIM3_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn ;    

			break;
		}
		case TIM4_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn ;    

			break;
		}
		case TIM5_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn ;     

			break;
		}
		case TIM6_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn ;     

			break;
		}
		case TIM7_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn ;    

			break;
		}
		case TIM12_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel=TIM8_BRK_TIM12_IRQn ;    

			break;
		}
		case TIM13_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel=TIM8_UP_TIM13_IRQn ;   

			break;
		}
		case TIM14_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel=TIM8_TRG_COM_TIM14_IRQn ;     

			break;
		}
				
		default: break;
	}
	
	//定时器TIMx初始化
	TIMx_TimeBaseStructure.TIM_Period=arr;						//设置自动重转载寄存器周期的值
	TIMx_TimeBaseStructure.TIM_Prescaler=psr;        		    //设置时钟分频除数的预分频值
	TIMx_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;      //设置时钟分割
	TIMx_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //TIM向上计数
	TIM_TimeBaseInit(TIMx, &TIMx_TimeBaseStructure);            //初始化TIM1
	TIM_ClearITPendingBit(TIMx, TIM_IT_Update);                 //初始化时必须将溢出中断清0,必须在开溢出中断之前
	TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE);                    //允许溢出中断
	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=prepri;		//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=subpri;            //从优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;               //使能IRQ通道
	NVIC_Init(&NVIC_InitStructure);                             //初始化NVIC寄存器
	
	TIM_Cmd(TIMx,ENABLE);                                       //使能TIM1
}


/**
  * @brief  accurency time delay dunction with TIMx
  * @param  TIMx: where x can be 1-14. 
  * @param  DelayMs: n us you want to delay, range from 1 to 65535
			@note	TIM2 and TIM5 are 32bit timer, you may change the code to delay longer;
					every time when time= 168/168M s,TIMCounter++ 
  * @retval None
  * @authonr Calcus Lee
  */
void TIM_Delayus(TIM_TypeDef * TIMx, uint16_t Delayus)
{
    uint16_t  TIMCounter = Delayus;
	TIM_TimeBaseInitTypeDef TIMx_TimeBaseStructure;
	
	switch((uint32_t)TIMx)
	{
		//APB2 TIM
		case TIM1_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=168;        		    //设置时钟分频除数的预分频值

			break;
		}
		case TIM8_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=168;        		    //设置时钟分频除数的预分频值

			break;
		}
		case TIM9_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=168;        		    //设置时钟分频除数的预分频值

			break;
		}
		case TIM10_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=168;        		    //设置时钟分频除数的预分频值

			break;
		}
		case TIM11_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=168;        		    //设置时钟分频除数的预分频值

			break;
		}
		//APB1 TIM
		case TIM2_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=84;        		    //设置时钟分频除数的预分频值
			
			break;
		}
		case TIM3_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=84;        		    //设置时钟分频除数的预分频值
			
			break;
		}
		case TIM4_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=84;        		    //设置时钟分频除数的预分频值
			
			break;
		}
		case TIM5_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=84;        		    //设置时钟分频除数的预分频值
			
			break;
		}
		case TIM6_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=84;        		    //设置时钟分频除数的预分频值
			
			break;
		}
		case TIM7_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=84;        		    //设置时钟分频除数的预分频值
			
			break;
		}
		case TIM12_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=84;        		    //设置时钟分频除数的预分频值
			
			break;
		}
		case TIM13_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=84;        		    //设置时钟分频除数的预分频值
			
			break;
		}
		case TIM14_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=84;        		    //设置时钟分频除数的预分频值
			
			break;
		}
		default: break;
	}

	TIMx_TimeBaseStructure.TIM_Period=1;						//设置自动重转载寄存器周期的值
	TIMx_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;      //设置时钟分割
	TIMx_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //TIM向上计数
	TIM_TimeBaseInit(TIMx, &TIMx_TimeBaseStructure);            //初始化TIM1
	 									
	TIM_Cmd(TIMx,ENABLE);
	TIM_SetCounter(TIMx, 65535-TIMCounter);
	
	while (TIMCounter<65535)
	{
		TIMCounter = TIM_GetCounter(TIMx);
	}

	TIM_Cmd(TIMx, DISABLE);	
	
}


/**
  * @brief  accurency time delay dunction with TIMx
  * @param  TIMx: where x can be 1-14. 
  * @param  DelayMs: n 100us you want to delay, range from 1 to 65535
			@note	TIM2 and TIM5 are 32bit timer, you may change the code to delay longer
  * @retval None
  * @authonr Calcus Lee
  */
void TIM_Delay100us(TIM_TypeDef * TIMx, uint16_t Delay100us)
{
    uint16_t  TIMCounter = Delay100us;
	TIM_TimeBaseInitTypeDef TIMx_TimeBaseStructure;
	
	switch((uint32_t)TIMx)
	{
		//APB2 TIM
		case TIM1_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=16800;        		    //设置时钟分频除数的预分频值

			break;
		}
		case TIM8_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=16800;        		    //设置时钟分频除数的预分频值

			break;
		}
		case TIM9_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=16800;        		    //设置时钟分频除数的预分频值

			break;
		}
		case TIM10_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=16800;        		    //设置时钟分频除数的预分频值

			break;
		}
		case TIM11_BASE:
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=16800;        		    //设置时钟分频除数的预分频值

			break;
		}
		//APB1 TIM
		case TIM2_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=8400;        		    //设置时钟分频除数的预分频值
			
			break;
		}
		case TIM3_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=8400;        		    //设置时钟分频除数的预分频值
			
			break;
		}
		case TIM4_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=8400;        		    //设置时钟分频除数的预分频值
			
			break;
		}
		case TIM5_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=8400;        		    //设置时钟分频除数的预分频值
			
			break;
		}
		case TIM6_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=8400;        		    //设置时钟分频除数的预分频值
			
			break;
		}
		case TIM7_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=8400;        		    //设置时钟分频除数的预分频值
			
			break;
		}
		case TIM12_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=8400;        		    //设置时钟分频除数的预分频值
			
			break;
		}
		case TIM13_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=8400;        		    //设置时钟分频除数的预分频值
			
			break;
		}
		case TIM14_BASE:
		{
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
			TIMx_TimeBaseStructure.TIM_Prescaler=8400;        		    //设置时钟分频除数的预分频值
			
			break;
		}
		default: break;
	}

	TIMx_TimeBaseStructure.TIM_Period=1;						//设置自动重转载寄存器周期的值
	TIMx_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;      //设置时钟分割
	TIMx_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //TIM向上计数
	TIM_TimeBaseInit(TIMx, &TIMx_TimeBaseStructure);            //初始化TIM1
 					
	TIM_Cmd(TIMx,ENABLE);
	TIM_SetCounter(TIMx, 65535-TIMCounter);
	
	while (TIMCounter<65535)
	{
		TIMCounter = TIM_GetCounter(TIMx);
	}

	TIM_Cmd(TIMx, DISABLE);

}

/**
  * @brief  accurency time delay dunction with TIMx
  * @param  TIMx: where x can be 1-14. 
  * @param  DelayMs:
  * @retval None
  * @authonr Calcus Lee
  */
void TIM_Delayms(TIM_TypeDef * TIMx, uint32_t DelayMs)
{
    uint32_t i=0;

	for(i=0;i<DelayMs;i++)
	{
		TIM_Delay100us(TIMx,10);
	}
}

static uint8_t timeFlag=0;
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update)==SET)
  {	
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		timeFlag=1;
  }	 
}
uint8_t getTimeFlag(void)
{
	if(timeFlag)
	{
		timeFlag=0;
		return 1;
	}
	return 0;
}
