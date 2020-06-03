/**
********************************************************************************        
* \brief       ������(DRIVER)CANģ��(CAN)��ؽӿ�ʵ��.
* \details     Copyright (c) 2019,����/��ɳ�н��Ƶ������޹�˾.
*              All rights reserved.    
* \file        driver_can.c 
* \author      ����<lihj@jkaec.com>
* \version     1.0 
* \date        2019��5��9��
* \note        ʹ��ǰ��ο�ע��.\n
* \since       ����      2019��5��9��     1.0     �½� 
* \par �޶���¼
* -  2019��5��9�� ��ʼ�汾
* \par ��Դ˵��
* - RAM:              
* - ROM:
********************************************************************************
*/

#include "bsp.h"
#include "osapi_freertos.h"
#include "csp_if_can.h"
#include "can.h"
#include "driver_can.h"
#include "stm32f10x.h"
#include "misc.h"
#include <stdint.h>

/*******************************************************************************    
*                                                                           
*                             �ڲ�����                                   
*                                                                            
*******************************************************************************/


driver_can_data_t g_driver_can_rxdata_t[DRIVER_CAN_RXBUFFSIZE];
driver_can_data_t g_driver_can_txdata_t[DRIVER_CAN_TXBUFFSIZE];
driver_can_t g_driver_can_t = {0,0,0,0,0,0,DRIVER_CAN_RXBUFFSIZE,DRIVER_CAN_TXBUFFSIZE,(driver_can_data_t*)(&g_driver_can_rxdata_t[0]),(driver_can_data_t*)(&g_driver_can_txdata_t[0])};
//uint32_t g_driver_idlist_au32[28] = 
//{
//  (uint32_t)1,
//  (uint32_t)2,
//  (uint32_t)3,
//  (uint32_t)4,
//  (uint32_t)5,
//  (uint32_t)6,
//  (uint32_t)7,
//  (uint32_t)8,
//  (uint32_t)9,
//  (uint32_t)10,
//  (uint32_t)11,
//  (uint32_t)12,
//  (uint32_t)13,
//  (uint32_t)14,
//  (uint32_t)15,
//  (uint32_t)16,
//  (uint32_t)17,
//  (uint32_t)18,
//  (uint32_t)19,
//  (uint32_t)20,
//  (uint32_t)21,
//  (uint32_t)22,
//  (uint32_t)23,
//  (uint32_t)24,
//  (uint32_t)25,
//  (uint32_t)26,
//  (uint32_t)27,
//  (uint32_t)28,
//};

uint32_t g_driver_idlist_au32[28] = {0};
/*******************************************************************************    
*                                                                           
*                             �ڲ�����                                   
*                                                                            
*******************************************************************************/


/********************************************************************************
* \fn          static void driver_can_ioinit(void)
* \brief       ��ʼ������.
* \note        . 
********************************************************************************
*/
static void driver_can_ioinit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
  //AFIO->MAPR = (AFIO->MAPR & (uint32_t)(~((uint32_t)3<<13))) | (uint32_t)((uint32_t)2<<13);  //PB8 PB9 CAN1
  GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
  /*ʹ��ʱ��*/

  //RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

  /*���� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PB9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //�����������
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//PB8
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //PA12
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PA11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  
}




/**
********************************************************************************
* \fn          static int8_t driver_can_writemailbox(uint32_t id,uint8_t ide,uint8_t rtr,uint8_t *pdata,uint8_t len)
* \brief       CANд��Ϣ����.
* \note        .
* \param[in]   id ID.
* \param[in]   ide 0:��׼֡ 1 ��չ֡
* \param[in]   rtr 0:����֡ 1 Զ��֡
* \param[in]   pdata   ��������.
* \param[out]  len ���ݳ���
* \return      uint8_t 0�ɹ� 1ʧ��.
* \note        .
********************************************************************************
*/
static int8_t driver_can_writemailbox(driver_can_data_t data)
{
  int8_t res = 0;
  uint8_t dlc = 0;
  if(data.len>8)
  {
     dlc = (uint8_t)8;
  }
  else
  {
     dlc = data.len;
  }
  if(CAN1->TSR & (uint32_t)((uint32_t)1<<26))
  {
    CAN1->sTxMailBox[0].TIR = (data.id<<3)  & (uint32_t)0xFFFFFFFE;  /*����ʹ��һ��ʼ�ͷ�����һ�ε�����*/
    if(0 == (data.type & (uint8_t)((uint8_t)1<<CAN_DATA_RTR_BIT)))
    {
       CAN1->sTxMailBox[0].TIR &= (uint32_t)(~(uint32_t)((uint32_t)1<<1));
    }
    else
    {
       CAN1->sTxMailBox[0].TIR |= (uint32_t)((uint32_t)1<<1);
    }
    if(0 == (data.type & (uint8_t)((uint8_t)1<<CAN_DATA_IDE_BIT)))
    {
       CAN1->sTxMailBox[0].TIR &= (uint32_t)(~(uint32_t)((uint32_t)1<<2));
       //CAN1->sTxMailBox[0].TIR |= (uint32_t)((data.id & (uint32_t)0x7FF)<<21);
    }
    else
    {
       CAN1->sTxMailBox[0].TIR |= (uint32_t)((uint32_t)1<<2);
       //CAN1->sTxMailBox[0].TIR |= (uint32_t)((data.id & (uint32_t)0x1FFFFFFF)<<3);
    }
    /*�������*/
    CAN1->sTxMailBox[0].TDTR = (uint32_t)dlc;
    CAN1->sTxMailBox[0].TDLR = (uint32_t)data.buff[0] | ((uint32_t)data.buff[1]<<8) | ((uint32_t)data.buff[2]<<16) | ((uint32_t)data.buff[3]<<24);
    CAN1->sTxMailBox[0].TDHR = (uint32_t)data.buff[4] | ((uint32_t)data.buff[5]<<8) | ((uint32_t)data.buff[6]<<16) | ((uint32_t)data.buff[7]<<24);
    /*����*/
    CAN1->sTxMailBox[0].TIR |= (uint32_t)0x01;

  }
#if 0  /*ֻʹ��һ��FIFO���� ���ⲻ������*/
  else if(CAN1->TSR & (uint32_t)((uint32_t)1<<27))
  {
    CAN1->sTxMailBox[1].TIR = (data.id<<3)  & (uint32_t)0xFFFFFFFE;  /*����ʹ��һ��ʼ�ͷ�����һ�ε�����*/
    if(0 == (data.type & (uint8_t)((uint8_t)1<<CAN_DATA_RTR_BIT)))
    {
       CAN1->sTxMailBox[1].TIR &= (uint32_t)(~(uint32_t)((uint32_t)1<<1));
    }
    else
    {
       CAN1->sTxMailBox[1].TIR |= (uint32_t)((uint32_t)1<<1);
    }
    if(0 == (data.type & (uint8_t)((uint8_t)1<<CAN_DATA_IDE_BIT)))
    {
       CAN1->sTxMailBox[1].TIR &= (uint32_t)(~(uint32_t)((uint32_t)1<<2));
       //CAN1->sTxMailBox[1].TIR |= (uint32_t)((data.id & (uint32_t)0x7FF)<<21);
    }
    else
    {
       CAN1->sTxMailBox[1].TIR |= (uint32_t)((uint32_t)1<<2);
       //CAN1->sTxMailBox[1].TIR |= (uint32_t)((data.id & (uint32_t)0x1FFFFFFF)<<3);
    }
    /*�������*/
    CAN1->sTxMailBox[1].TDTR = (uint32_t)dlc;
    CAN1->sTxMailBox[1].TDLR = (uint32_t)data.buff[0] | ((uint32_t)data.buff[1]<<8) | ((uint32_t)data.buff[2]<<16) | ((uint32_t)data.buff[3]<<24);
    CAN1->sTxMailBox[1].TDHR = (uint32_t)data.buff[4] | ((uint32_t)data.buff[5]<<8) | ((uint32_t)data.buff[6]<<16) | ((uint32_t)data.buff[7]<<24);
    /*����*/
    CAN1->sTxMailBox[1].TIR |= (uint32_t)0x01;
  }
  else if(CAN1->TSR & (uint32_t)((uint32_t)1<<28))
  {
    CAN1->sTxMailBox[2].TIR = (data.id<<3)  & (uint32_t)0xFFFFFFFE;  /*����ʹ��һ��ʼ�ͷ�����һ�ε�����*/
    if(0 == (data.type & (uint8_t)((uint8_t)1<<CAN_DATA_RTR_BIT)))
    {
       CAN1->sTxMailBox[2].TIR &= (uint32_t)(~(uint32_t)((uint32_t)1<<1));
    }
    else
    {
       CAN1->sTxMailBox[2].TIR |= (uint32_t)((uint32_t)1<<1);
    }
    if(0 == (data.type & (uint8_t)((uint8_t)1<<CAN_DATA_IDE_BIT)))
    {
       CAN1->sTxMailBox[2].TIR &= (uint32_t)(~(uint32_t)((uint32_t)1<<2));
       //CAN1->sTxMailBox[2].TIR |= (uint32_t)((data.id & (uint32_t)0x7FF)<<21);
    }
    else
    {
       CAN1->sTxMailBox[2].TIR |= (uint32_t)((uint32_t)1<<2);
       //CAN1->sTxMailBox[2].TIR |= (uint32_t)((data.id & (uint32_t)0x1FFFFFFF)<<3);
    }
    /*�������*/
    CAN1->sTxMailBox[2].TDTR = (uint32_t)dlc;
    CAN1->sTxMailBox[2].TDLR = (uint32_t)data.buff[0] | ((uint32_t)data.buff[1]<<8) | ((uint32_t)data.buff[2]<<16) | ((uint32_t)data.buff[3]<<24);
    CAN1->sTxMailBox[2].TDHR = (uint32_t)data.buff[4] | ((uint32_t)data.buff[5]<<8) | ((uint32_t)data.buff[6]<<16) | ((uint32_t)data.buff[7]<<24);
    /*����*/
    CAN1->sTxMailBox[2].TIR |= (uint32_t)0x01;
  }
#endif
  else
  {
    res = 1;
  
  }
  return res;
}


/**
 *****************************************************************************
 * \fn          static int8_t is_txmail_empty(void)
 * \brief       �жϷ��������Ƿ����д����.
 * \note        . 
 * \return      1 ����д���� 0������
 *****************************************************************************
 */
static int8_t is_txmail_empty(void)
{
   if(CAN1->TSR & (uint32_t)((uint32_t)1<<26))
   {
       return (int8_t)1;
   }
#if 0
   else if(CAN1->TSR & (uint32_t)((uint32_t)1<<27))
   {
       return (int8_t)1;
   }
   else if((CAN1->TSR & (uint32_t)((uint32_t)1<<28)))
   {
       return (int8_t)1;
   }
#endif
   else
   {
       return (int8_t)0;
   }
}

/**
 *****************************************************************************
 * \fn          static int8_t rx_fifo_in(driver_can_t *p, driver_can_data_t data)
 * \brief       д���ݵ����ջ�����.
 * \note        �����жϷ��������øú���д���ݵ����ջ�����,����������д��. 
 * \param[in]   p ָ��can�ṹ��ʵ����ָ�� \ref driver_can_t
 * \param[in]   data ָ��Ҫд��������������
 * \return      0�ɹ� ����ֵʧ��
 *****************************************************************************
 */
static int8_t rx_fifo_in(driver_can_t *p, driver_can_data_t data)
{
    uint8_t i = 0;
    if((p==0) || (data.len>8))
    {
        return -1;
    }
    /*������δ��д������*/
    if (p->rx_len < p->rxbuf_len) 
    {   
        p->rx_buf[p->rx_in].len = data.len;             /*д������,д��ָ�����*/
        p->rx_buf[p->rx_in].type = data.type;
        p->rx_buf[p->rx_in].id = data.id;
        for(i=0;i<data.len;i++)
        {
          p->rx_buf[p->rx_in].buff[i] = data.buff[i];
        }
        p->rx_in++;
        p->rx_in %= p->rxbuf_len;                       /*�߽紦��,ʵ��Բ�λ�����*/
        p->rx_len++;                                    /*���ݸ�������*/
        return 0;
    }
    else
    {
        return -1;
    }
}


/**
 *****************************************************************************
 * \fn          static int8_t rx_fifo_out(driver_can_t *p, driver_can_data_t* buff,uint32_t timeout)
 * \brief       �ӽ��ջ�����������.
 * \note        Ӧ�ú������øú����ӽ��ջ�����������,����ָ����ʱʱ��. 
 * \param[in]   p ָ��can�ṹ��ʵ����ָ�� \ref driver_can_t
 * \param[out]  buff ָ��can_data�ṹ��ʵ����ָ�� \ref driver_can_data_t
 * \param[in]   timeout ָ����ʱʱ�� 0��ʾһֱ�ȴ� 
 * \retval      int8_t 0�ɹ� ����ֵʧ��.
 *****************************************************************************
 */
static int8_t rx_fifo_out(driver_can_t *p, driver_can_data_t* buff,uint32_t timeout)
{
    uint8_t i = (uint8_t)0;
    if(p==0)
    {
        return 1;
    }
    OSCriticalAlloc();
    /*timeout==0 ��ʾһֱ�ȴ�ֱ��������*/
    if (timeout == 0) 
    {
        while(p->rx_len == 0)
        {
            OsTimeDelay(DRIVER_CAN_DELAY_CHECK);  /*��ʱ*/
        }
    } 
    else /*timeout!=0 ��ʾ�ȴ�ָ����ʱʱ��*/
    {
        while (timeout != 0)
        {
            if (p->rx_len == 0) 
            {
                /*���������,��ʱָ��ʱ����*/
                if (timeout >= DRIVER_CAN_DELAY_CHECK)
                {
                    timeout -= DRIVER_CAN_DELAY_CHECK;
                    OsTimeDelay(DRIVER_CAN_DELAY_CHECK);
                }
                else
                {
                     /*����һ��ʱ������ʱʣ��ʱ��*/
                    OsTimeDelay(timeout);
                    timeout = 0;
                }
            } 
            else
            {
                break;
            }
        }
    }
    /*�ٽ�δ��� ��ֹ��ʱ�ж��ڲ���������*/
    OSCriticalEnter();
    if (p->rx_len != 0) 
    {
        /*�ӻ�������������*/
        p->rx_len--;
        buff->type = p->rx_buf[p->rx_out].type;
        buff->id = p->rx_buf[p->rx_out].id;
        if(p->rx_buf[p->rx_out].len > (uint8_t)8)
        {
            buff->len = (uint8_t)8;
        }
        else
        {
            buff->len = p->rx_buf[p->rx_out].len;
        }
        for(i=0;i<buff->len;i++)
        {
          buff->buff[i] = p->rx_buf[p->rx_out].buff[i];
        }
        p->rx_out++;
        p->rx_out %= p->rxbuf_len;
    }
    else
    {
        OSCriticalExit();
        return 1;
    }
    OSCriticalExit();
    return 0;
}


/**
 *****************************************************************************
 * \fn          static int8_t tx_fifo_in(driver_can_t *p, driver_can_data_t data, uint32_t timeout)
 * \brief       д���ݵ����ͻ�����.
 * \note        Ӧ�õ��øú�����д���ݵ����ͻ�����,���������ȴ�. 
 * \param[in]   p ָ��can�ṹ��ʵ����ָ�� \ref driver_can_t
 * \param[in]   data ָ��can_data�ṹ��ʵ����ָ�� \ref driver_can_data_t
 * \param[in]   timeout �趨��ʱʱ�� 0��ʾһֱ�ȴ�
 * \retval      0 д�ɹ�
 * \retval      1 дʧ��
 *****************************************************************************
 */
static int8_t tx_fifo_in(driver_can_t *p, driver_can_data_t data, uint32_t timeout)
{
    /*ע��ȷ��p�������Ķ�д�����ٽ�α��� */
    int8_t ret = (int8_t)0;
    uint8_t i = (uint8_t)0;
    if((p==0) || (data.len>8))
    {
        return 1;
    }
    OSCriticalAlloc();
    if (timeout == 0) 
    {
       
        while(1)
        {
            OSCriticalEnter();
            if(p->tx_len < p->txbuf_len)
            {
                /*û������д������*/
                p->tx_buf[p->tx_in].len = data.len;             
                p->tx_buf[p->tx_in].type = data.type;
                p->tx_buf[p->tx_in].id = data.id;
                for(i=0;i<data.len;i++)
                {
                  p->tx_buf[p->tx_in].buff[i] = data.buff[i];
                }
                p->tx_in++;
                p->tx_in %= p->txbuf_len;
                p->tx_len++;
                OSCriticalExit();
                ret = (int8_t)0;
                break;
            }
            else
            {
                /*����ȴ�*/
                OSCriticalExit();
                OsTimeDelay(DRIVER_CAN_DELAY_CHECK);  /*��ʱ*/
            }
        }
    } 
    while (timeout != 0)
    {
        OSCriticalEnter();
        if(p->tx_len < p->txbuf_len)
        {
          /*û������д������*/
          p->tx_buf[p->tx_in].len = data.len;             
          p->tx_buf[p->tx_in].type = data.type;
          p->tx_buf[p->tx_in].id = data.id;
          for(i=0;i<data.len;i++)
          {
            p->tx_buf[p->tx_in].buff[i] = data.buff[i];
          }
          p->tx_in++;
          p->tx_in %= p->txbuf_len;
          p->tx_len++;
          OSCriticalExit();
          ret = (int8_t)0;
          break;
        }
        else 
        {
          OSCriticalExit();
          /*���������,��ʱָ��ʱ����*/
          if (timeout >= DRIVER_CAN_DELAY_CHECK)
          {
            timeout -= DRIVER_CAN_DELAY_CHECK;
            OsTimeDelay(DRIVER_CAN_DELAY_CHECK);
            ret = (uint8_t)1; /*���ó�ʱ���� ֮ǰ����û����ret=1 ��timeoutΪDRIVER_CAN_DELAY_CHECK��������ʱ���ǻ᷵��0*/
          }
          else
          {
            /*����һ��ʱ������ʱʣ��ʱ��*/
            OsTimeDelay(timeout);
            timeout = (uint8_t)0;
            ret = 1; /*���ó�ʱ����*/
          }
        }
    }
    return ret;
}


/**
 *****************************************************************************
 * \fn          static int8_t tx_fifo_out(driver_can_t *p, driver_can_data_t *buff)
 * \brief       �ӷ��ͻ�����������.
 * \note        ���ͺ������øú�����ȡ���ͻ���������,���ڷ���. 
 * \param[in]   p ָ��can�ṹ��ʵ����ָ�� \ref driver_can_t
 * \param[out]  buff ָ��can_data�ṹ��ʵ����ָ�� \ref driver_can_data_t
 * \return      int8_t 0�ɹ� ����ֵʧ��  
 *****************************************************************************
 */
static int8_t tx_fifo_out(driver_can_t *p, driver_can_data_t *buff)
{
    uint8_t i = (uint8_t)0;
    if(p==0)
    {
        return 1;
    }
    OSCriticalAlloc();
    OSCriticalEnter();
    if (p->tx_len != 0) 
    {
        p->tx_len--;
        
        buff->type = p->tx_buf[p->tx_out].type;
        buff->id = p->tx_buf[p->tx_out].id;
        if(p->tx_buf[p->tx_out].len > (uint8_t)8)
        {
            buff->len = (uint8_t)8;
        }
        else
        {
            buff->len = p->tx_buf[p->tx_out].len;
        }
        for(i=0;i<buff->len;i++)
        {
          buff->buff[i] = p->tx_buf[p->tx_out].buff[i];
        }
        p->tx_out++;
        p->tx_out %= p->txbuf_len;
        OSCriticalExit();
    } 
    else
    {
        OSCriticalExit();
        return 1;
    }
    return 0;
}


void USB_LP_CAN1_RX0_IRQHandler(void)
{
   /*��������*/
   driver_can_data_t tmp;
   tmp.type = (uint8_t)0;
   uint8_t len = (uint8_t)0;
   uint32_t readdata;
   len = (uint8_t)(CAN1->RF0R & (uint8_t)0x03);  /*�����״̬*/
   while(len--)
   {
     if(0 != (CAN1->sFIFOMailBox[0].RIR & (uint32_t)((uint32_t)1<<1)))
     {
       tmp.type |= (uint8_t)((uint8_t)1<<CAN_DATA_RTR_BIT);
     }
     else
     {
       tmp.type &= (uint8_t)(~(uint8_t)((uint8_t)1<<CAN_DATA_RTR_BIT));
     }
     if(0 !=  (CAN1->sFIFOMailBox[0].RIR & (uint32_t)((uint32_t)1<<2)))
     {
       tmp.type |= (uint8_t)((uint8_t)1<<CAN_DATA_IDE_BIT);
     }
     else
     {
       tmp.type &= (uint8_t)(~(uint8_t)((uint8_t)1<<CAN_DATA_IDE_BIT));
     }
     tmp.id = (CAN1->sFIFOMailBox[0].RIR>>3) & (uint32_t)0x1FFFFFFF;
     tmp.len = (uint8_t)(CAN1->sFIFOMailBox[0].RDTR & (uint8_t)0x0F);
     if(tmp.len > (uint8_t)8)
     {
       tmp.len = (uint8_t)8;
     }
     else
     {}
     readdata = CAN1->sFIFOMailBox[0].RDLR;
     tmp.buff[0] = (uint8_t)(readdata & (uint32_t)0x00FF);
     tmp.buff[1] = (uint8_t)((readdata>>8) & (uint32_t)0x00FF);
     tmp.buff[2] = (uint8_t)((readdata>>16) & (uint32_t)0x00FF);
     tmp.buff[3] = (uint8_t)((readdata>>24) & (uint32_t)0x00FF);
     readdata = CAN1->sFIFOMailBox[0].RDHR;
     tmp.buff[4] = (uint8_t)(readdata & (uint32_t)0x00FF);
     tmp.buff[5] = (uint8_t)((readdata>>8) & (uint32_t)0x00FF);
     tmp.buff[6] = (uint8_t)((readdata>>16) & (uint32_t)0x00FF);
     tmp.buff[7] = (uint8_t)((readdata>>24) & (uint32_t)0x00FF);
     CAN1->RF0R |= (uint32_t)((uint32_t)1<<5);  /*�ͷ�FIFO ��ȡ��һ��*/
     if(rx_fifo_in(&g_driver_can_t, tmp) != 0)
     {
         return;   /*����������дֱ���˳�*/
     }
   }
}

void USB_HP_CAN1_TX_IRQHandler(void)
{


}


 void CAN1_RX1_IRQHandler(void)
 {
   /*��������*/
   driver_can_data_t tmp;
   tmp.type = (uint8_t)0;
   uint8_t len = (uint8_t)0;
   uint32_t readdata;
   len = (uint8_t)(CAN1->RF1R & (uint8_t)0x03);  /*�����״̬*/
   while(len--)
   {
     if(0 != (CAN1->sFIFOMailBox[1].RIR & (uint32_t)((uint32_t)1<<1)))
     {
       tmp.type |= (uint8_t)((uint8_t)1<<CAN_DATA_RTR_BIT);
     }
     else
     {
       tmp.type &= (uint8_t)(~(uint8_t)((uint8_t)1<<CAN_DATA_RTR_BIT));
     }
     if(0 !=  (CAN1->sFIFOMailBox[1].RIR & (uint32_t)((uint32_t)1<<2)))
     {
       tmp.type |= (uint8_t)((uint8_t)1<<CAN_DATA_IDE_BIT);
     }
     else
     {
       tmp.type &= (uint8_t)(~(uint8_t)((uint8_t)1<<CAN_DATA_IDE_BIT));
     }
     tmp.id = (CAN1->sFIFOMailBox[1].RIR>>3) & (uint32_t)0x1FFFFFFF;
     tmp.len = (uint8_t)(CAN1->sFIFOMailBox[1].RDTR & (uint8_t)0x0F);
     if(tmp.len > (uint8_t)8)
     {
       tmp.len = (uint8_t)8;
     }
     else
     {}
     readdata = CAN1->sFIFOMailBox[1].RDLR;
     tmp.buff[0] = (uint8_t)(readdata & (uint32_t)0x00FF);
     tmp.buff[1] = (uint8_t)((readdata>>8) & (uint32_t)0x00FF);
     tmp.buff[2] = (uint8_t)((readdata>>16) & (uint32_t)0x00FF);
     tmp.buff[3] = (uint8_t)((readdata>>24) & (uint32_t)0x00FF);
     readdata = CAN1->sFIFOMailBox[1].RDHR;
     tmp.buff[4] = (uint8_t)(readdata & (uint32_t)0x00FF);
     tmp.buff[5] = (uint8_t)((readdata>>8) & (uint32_t)0x00FF);
     tmp.buff[6] = (uint8_t)((readdata>>16) & (uint32_t)0x00FF);
     tmp.buff[7] = (uint8_t)((readdata>>24) & (uint32_t)0x00FF);
     CAN1->RF1R |= (uint32_t)((uint32_t)1<<5);  /*�ͷ�FIFO ��ȡ��һ��*/
     if(rx_fifo_in(&g_driver_can_t, tmp) != 0)
     {
         return;   /*����������дֱ���˳�*/
     }
   }
 }

 void CAN1_SCE_IRQHandler(void)
 {
    /*���� ��������*/
   CAN1->MSR |= CAN1->MSR; /*�����־*/
   CAN1->TSR |= CAN1->TSR;
   
 }



/*******************************************************************************    
*                                                                           
*                             ����ӿں���ʵ��                                   
*                                                                            
*******************************************************************************/

/**
********************************************************************************
* \fn          void driver_can_init(uint32_t baudrate)
* \brief       ��ʼ��CAN.
* \param[in]   baudrate ������.
* \note        . 
********************************************************************************
*/
void driver_can_init(uint32_t baudrate)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  uint32_t timeout = (uint32_t)10000;
  uint32_t baudset = 0;
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);
  
  driver_can_ioinit();
  /*�ж�����*/
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=14 ;//��ռ���ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;       //�����ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQͨ��ʹ��
  NVIC_Init(&NVIC_InitStructure);  
   
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_SCE_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=14 ;//��ռ���ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;       //�����ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQͨ��ʹ��
  NVIC_Init(&NVIC_InitStructure);  
  
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=14 ;//��ռ���ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;       //�����ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQͨ��ʹ��
  NVIC_Init(&NVIC_InitStructure);  
  
  NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=14 ;//��ռ���ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;       //�����ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQͨ��ʹ��
  NVIC_Init(&NVIC_InitStructure);    
  
  /*1.�����ʼ��ģʽ:��λ����SLEEPģʽ ->SLEEP=0 INRQ=1 �����ʼ��ģʽ*/
  CAN1->MCR &= ~(1<<1); /*SLEEP=0*/
  CAN1->MCR |=  (1<<0); /*INAR=1*/
  while((CAN1->MSR & (1<<0)) ==0);  /*�ȴ�INAK��1  ��ʼ�����*/
  /*2.����*/
  CAN1->MCR &= ~(1<<16); /*����ʱ�䴥��ģʽ*/
  CAN1->MCR &= ~(1<<7); /*����ʱ�䴥��ģʽ*/
  CAN1->MCR |=  (1<<6); /*BUS-off �Զ��ָ�*/
  CAN1->MCR |=  (1<<5); /*ʹ���Զ�����*/
  CAN1->MCR &= ~(1<<4); /*ʹ���Զ��ط�*/
  CAN1->MCR &= ~(1<<3); /*���ջ���������򸲸�*/
  CAN1->MCR &= ~(1<<2); /*�������ȼ���IDȷ��*/
  
  CAN1->MSR |= (1<<4) | (1<<3) | (1<<2); /*�����־*/
  CAN1->TSR |= (uint32_t)0x000F0F0F;/*�����־*/
  CAN1->RF0R |= (1<<4) | (1<<3); /*�����־*/
  CAN1->RF1R |= (1<<4) | (1<<3); /*�����־*/
  
  
  CAN1->IER = 0x00038F7E; /*ʹ�ܳ���������������ж������ж�*/
  
  CAN1->ESR = (uint32_t)0x70;
  
  /*
  BaudRate= 1/NominalBitTime
  NominalBitTime =1xtq + tBS1 + tBS2
  tBS1 = tq x (TS1[3:0] + 1)
  tBS2 = tq x (TS2[2:0] + 1)
  tq = (BRP[9:0] + 1) x tPCLK
   ts1 ts2����Ϊ0
  BaudRate = 1/3tq
  */
  baudset = RCC_Clocks.PCLK1_Frequency/((uint32_t)8 * baudrate) -1;
  //CAN1->BTR = (uint32_t)0xC1230000 | (uint32_t)baudset;  /*�ػ�ģʽ  ���ò�����*/
  CAN1->BTR = (uint32_t)0x01230000 | (uint32_t)baudset;  /*����ģʽ  ���ò�����*/

  /*4.��������ģʽ:INRQ=0*/
  CAN1->MCR &=  ~(1<<0); /*INRQ=0*/
  while((CAN1->MSR & (1<<0)) && (timeout--));  /*�ȴ�INAK��0  �������*/
  
  /*3.���ý��չ���*/
  CAN1->FMR |= (uint32_t)0x01;         /* FINIT=1�������ú����Ĵ���*/
  CAN1->FM1R = (uint32_t)0x00000000;  /*���й�����ʹ��MASKģʽ*/
  CAN1->FS1R = (uint32_t)0x0FFFFFFF;  /*���й�����ʹ��32λƥ��ģʽ*/
  CAN1->FFA1R = (uint32_t)0x0FFFFFFF;  /*����һ��FIFO�������ʱ������*/
  CAN1->FA1R = 0;  /*�رղ�������*/
  /*����ID����*/
  CAN1->sFilterRegister[0].FR1 = (g_driver_idlist_au32[0]<<3) | (uint32_t)0x04;  /*��չ����֡0x04 ��׼����֡0x00  ��չԶ��֡0x03 ��׼Զ��֡0x02  ��׼֡<<21λ*/
  CAN1->sFilterRegister[0].FR2 = (g_driver_idlist_au32[1]<<3) | (uint32_t)0x04;
  CAN1->sFilterRegister[1].FR1 = (g_driver_idlist_au32[2]<<3) | (uint32_t)0x04;
  CAN1->sFilterRegister[1].FR2 = (g_driver_idlist_au32[3]<<3) | (uint32_t)0x04;
  CAN1->sFilterRegister[2].FR1 = (g_driver_idlist_au32[4]<<3) | (uint32_t)0x04;
  CAN1->sFilterRegister[2].FR2 = (g_driver_idlist_au32[5]<<3) | (uint32_t)0x04;
  CAN1->sFilterRegister[3].FR1 = (g_driver_idlist_au32[6]<<3) | (uint32_t)0x04;
  CAN1->sFilterRegister[3].FR2 = (g_driver_idlist_au32[7]<<3) | (uint32_t)0x04;
  CAN1->sFilterRegister[4].FR1 = (g_driver_idlist_au32[8]<<3) | (uint32_t)0x04;
  CAN1->sFilterRegister[4].FR2 = (g_driver_idlist_au32[9]<<3) | (uint32_t)0x04;
  CAN1->sFilterRegister[5].FR1 = (g_driver_idlist_au32[10]<<3) | (uint32_t)0x04;
  CAN1->sFilterRegister[5].FR2 = (g_driver_idlist_au32[11]<<3) | (uint32_t)0x04;
  CAN1->sFilterRegister[6].FR1 = (g_driver_idlist_au32[12]<<3) | (uint32_t)0x04;
  CAN1->sFilterRegister[6].FR2 = (g_driver_idlist_au32[13]<<3) | (uint32_t)0x04;
  CAN1->sFilterRegister[7].FR1 = (g_driver_idlist_au32[14]<<3) | (uint32_t)0x04;
  CAN1->sFilterRegister[7].FR2 = (g_driver_idlist_au32[15]<<3) | (uint32_t)0x04;
  CAN1->sFilterRegister[8].FR1 = (g_driver_idlist_au32[16]<<3) | (uint32_t)0x04;
  CAN1->sFilterRegister[8].FR2 = (g_driver_idlist_au32[17]<<3) | (uint32_t)0x04;
  CAN1->sFilterRegister[9].FR1 = (g_driver_idlist_au32[18]<<3) | (uint32_t)0x04;
  CAN1->sFilterRegister[9].FR2 = (g_driver_idlist_au32[19]<<3) | (uint32_t)0x04;
  CAN1->sFilterRegister[10].FR1 = (g_driver_idlist_au32[20]<<3) | (uint32_t)0x04;
  CAN1->sFilterRegister[10].FR2 = (g_driver_idlist_au32[21]<<3) | (uint32_t)0x04;
  CAN1->sFilterRegister[11].FR1 = (g_driver_idlist_au32[22]<<3) | (uint32_t)0x04;
  CAN1->sFilterRegister[11].FR2 = (g_driver_idlist_au32[23]<<3) | (uint32_t)0x04;
  CAN1->sFilterRegister[12].FR1 = (g_driver_idlist_au32[24]<<3) | (uint32_t)0x04;
  CAN1->sFilterRegister[12].FR2 = (g_driver_idlist_au32[25]<<3) | (uint32_t)0x04;
  CAN1->sFilterRegister[13].FR1 = (g_driver_idlist_au32[26]<<3) | (uint32_t)0x04;
  CAN1->sFilterRegister[13].FR2 = (g_driver_idlist_au32[27]<<3) | (uint32_t)0x04;
  //CAN1->FA1R = (uint32_t)0xFFFFFFF;  /*��ʹ��*/
  CAN1->FMR &= (uint32_t)~0x01;         /* ʹ�ܹ���*/
}



/**
********************************************************************************
* \fn          void driver_can_filter(uint_t mode,uint32_t id,uint32_t mask)
* \brief       ����CAN����.
* \param[in]   ch ͨ��0-13.
* \param[in]   mode 0:MASKģʽ 1:listģʽ.
* \param[in]   id id.
* \param[in]   mask ����.
* \note        . 
********************************************************************************
*/
void driver_can_filter(uint8_t ch,uint8_t mode,uint32_t id,uint32_t mask)
{
  /*3.���ý��չ���*/
  CAN1->FMR |= (uint32_t)0x01;         /* FINIT=1�������ú����Ĵ���*/
  if(mode == (uint8_t)1)
  {
    CAN1->FM1R |= (uint32_t)((uint32_t)1<<ch);  /*ʹ��LISTģʽ*/
  }
  else
  {
    CAN1->FM1R &= (uint32_t)(~(uint32_t)((uint32_t)1<<ch));  /*ʹ��MSKģʽ*/
  }

  CAN1->FA1R &= (uint32_t)(~(uint32_t)((uint32_t)1<<ch));  /*�رղ�������*/
  /*����ID����*/
  CAN1->sFilterRegister[ch].FR1 = id<<3;  /*��չ����֡0x04 ��׼����֡0x00  ��չԶ��֡0x03 ��׼Զ��֡0x02  ��׼֡<<21λ*/
  CAN1->sFilterRegister[ch].FR2 = mask<<3;
  CAN1->FA1R |= (uint32_t)((uint32_t)1<<ch);   /*ʹ��*/
  CAN1->FMR &= (uint32_t)~0x01;         /* ʹ�ܹ���*/
}

/**
 *****************************************************************************
 * \fn          int8_t driver_can_send(driver_can_data_t data, uint32_t timeout)
 * \brief       ����CAN����(д�뻺����).
 * \note        Ӧ�õ��øú�����д���ݵ����ͻ�����,���������ȴ�. 
 * \param[in]   data ָ��can_data�ṹ��ʵ����ָ�� \ref driver_can_data_t
 * \param[in]   timeout �趨��ʱʱ�� 0��ʾһֱ�ȴ�
 * \retval      0 д�ɹ�
 * \retval      1 дʧ��
 *****************************************************************************
 */
int8_t driver_can_send(driver_can_data_t data, uint32_t timeout)
{
    return tx_fifo_in(&g_driver_can_t, data, timeout); 
}

/**
 *****************************************************************************
 * \fn          int8_t driver_can_recv(driver_can_data_t* data, uint32_t timeout)
 * \brief       �ӽ��ջ�����������.
 * \note        Ӧ�ú������øú����ӽ��ջ�����������,����ָ����ʱʱ��. 
 * \param[out]  data ָ��can_data�ṹ��ʵ����ָ�� \ref driver_can_data_t
 * \param[in]   timeout ָ����ʱʱ�� 0��ʾһֱ�ȴ� 
 * \retval      int8_t 0�ɹ� ����ֵʧ��.
 *****************************************************************************
 */
int8_t driver_can_recv(driver_can_data_t* data, uint32_t timeout)
{
  return rx_fifo_out(&g_driver_can_t, data, timeout);
}

/**
 *****************************************************************************
 * \fn          int8_t driver_can_sendloop(void)
 * \brief       ���������Ϣ�����п�,���з�������,�򽫷��ͻ���������д�뷢����Ϣ����.
 * \note        Ӧ�����ڵ��øú������з��ʹ���. 
 * \retval      0 �з���
 * \retval      1 �޷���
 *****************************************************************************
 */
//int8_t driver_can_sendloop(void)
//{
//  int8_t res = (int8_t)0;
//  driver_can_data_t tmp;
//  do
//  {
//    if(is_txmail_empty() != (uint8_t)0)
//    {
//      if(tx_fifo_out(&g_driver_can_t, &tmp) == (int8_t)0)
//      {
//        res = driver_can_writemailbox(tmp);
//      }
//      else
//      {
//        res = (int8_t)1;
//      }
//    }
//    else
//    {
//      res = (int8_t)1;
//    }
//  }while(res == 0);
//  return res;
//}

int8_t driver_can_sendloop(void)
{
    static uint32_t g_check_empty_u32 = 0;
    int8_t res = (int8_t)0;
    driver_can_data_t tmp;
    do
    {
        if(is_txmail_empty() != (uint8_t)0)
        {
            g_check_empty_u32 = 0;
            if(tx_fifo_out(&g_driver_can_t, &tmp) == (int8_t)0)
            {
              res = driver_can_writemailbox(tmp);
            }
            else
            {
              res = (int8_t)1;
            }
        }
        else
        {
            g_check_empty_u32++;
            if(g_check_empty_u32 >= 100)  /*���100�β�ѯ����������ǿ�˵���쳣���³�ʼ��(���䷢��һ֡�����ܳ���100ms)*/
            {
                g_check_empty_u32 = 0;
                res = (int8_t)1;
                driver_can_init((uint32_t)500000);
                driver_can_filter(0,0,(uint32_t)kiss_get_clientaddr()<<13,(uint32_t)0xFF<<13);
                driver_can_filter(1,0,(uint32_t)kiss_get_clientaddr()<<19,(uint32_t)0x1F<<19);
            }
            OsTimeDelay(1);
        }
    }
    while(res == 0);
    return res;
}

