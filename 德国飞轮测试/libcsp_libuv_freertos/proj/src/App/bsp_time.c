#include "bsp_time.h"
#include <stdint.h>
#include "stm32f10x.h"
#include "misc.h"
#include <stdint.h>
static bsp_time_callback s_time_callback_pfun = 0;
/*
frq ��ʱ��Ƶ�� hz
period�ж����� us

*/
void bsp_time6_init(uint32_t frq,uint32_t period)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
  TIM6->CR1 = 0x04;  /*ARR�Ĵ��������� д������Ч ����ģʽ  ֻ��������ж�  ʹ�ܸ����¼�*/
  //TIM6->CR2
  TIM6->DIER = 0x01;  /*ʹ�ܸ����¼��ж�*/
  //TIM6->SR
  TIM6->CNT = 0;      /*���ϼ�����  �����¼�ʱ����ARR�Ĵ�����ֵ  ���Լ���������ARR-0xFFFF*/
  TIM6->PSC = RCC_Clocks.PCLK1_Frequency/frq - 1;      /*��ʱ��Ƶ�� = fCK_PSC/(PSC[15:0] + 1)  �üĴ���ֵ���¼�����ʱ д�뵽ʵ�ʹ�����PSR�Ĵ���*/
  TIM6->ARR = (uint16_t)((uint32_t)65536 - (period*1000000)/frq);      /**/
  TIM6->EGR |= 0x01;  /*�ֶ����������¼� ���¼Ĵ��� ����CR1ֻʹ��������ж� �������ﲻ���ж�*/
  
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=14 ;//��ռ���ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;       //�����ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQͨ��ʹ��
  NVIC_Init(&NVIC_InitStructure);  
  
  TIM6->CR1 |= 0x01;  /*������ʱ��*/
  
}

void bsp_time6_setcallback(bsp_time_callback callbackfun)
{
  s_time_callback_pfun = callbackfun;

}

void TIM6_IRQHandler(void)
{
  if(TIM6->SR & 0x01)
  {
    TIM6->SR = 0x00;
    if(s_time_callback_pfun != 0)
    {
      s_time_callback_pfun();
    }
  }
}