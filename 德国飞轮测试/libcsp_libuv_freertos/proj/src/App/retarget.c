/**
 *****************************************************************************        
 * \brief       ��׼��������ײ�ӿ��ض���.
 * \details     Copyright (c) 2017,��ɳ��ʤ��Ϣ�������޹�˾.
 *              All rights reserved.    
 * \file        retarget.c 
 * \author      ����W18900
 * \version     1.0 
 * \date        2017-07-26
 * \note        ʹ��ǰ��ο�ע��.\n
 *              ʵ�ֹ�printf scanf���õĵײ�ӿ�.\n
 * \since       ����      2017-7-26 08:40:34     1.0     �½� 
 * \par �޶���¼
 * - 2017-07-26 ��ʼ�汾
 * \par ��Դ˵��
 * - RAM:              
 * - ROM:
 *****************************************************************************
 */
#include "hal.h"
#include "driver.h"
#include "retarget.h"
#include <stddef.h>
static HAL_UART_ID_e stdiouart=HAL_UART_MAX;
///**
// *****************************************************************************
// * \fn          int putchar(int ch)
// * \brief       �ַ�����ӿ�ʵ��.
// * \note        ��printf����,����driver_uartʵ��. 
// * \param[in]   ch ��Ҫ������ַ�
// * \return      int ʵ��������ַ�
// *****************************************************************************
// */
//int putchar(int ch)
//{
//    int erro=0;
//    unsigned char data=ch;
//    driver_uart_send(stdiouart, &data, 1,0,&erro);
//    return ch;   
//}

/**
 *****************************************************************************
 * \fn          size_t __write(int handle,const unsigned char *buf,size_t bufSize)
 * \brief       ��׼����ӿ�ʵ��.
 * \note        ��printf�Ⱥ�������,����driver_uartʵ��. 
 * \param[in]   handle ����
 * \param[in]   buf ���������
 * \param[in]   bufSize ��Ҫ������ֽ���
 * \return      size_t ʵ������ĳ���
 *****************************************************************************
 */
size_t __write(int handle,const unsigned char *buf,size_t bufSize)
{ 
    size_t nChars = 0;   /* Check for the command to flush all handles */ 
    //int erro=0;
    if (handle == -1)  
    {    
        return 0;  
    }    /* Check for stdout and stderr (only necessary if FILE descriptors are enabled.) */  
    if (handle != 1 && handle != 2)  
    {    
        return -1;  
    }  
    for (/* Empty */; bufSize > 0; --bufSize)  
    {    
        //driver_uart_send(stdiouart,(unsigned char *)buf, 1,0,&erro); 
        /*��Ϊ��ѯ��ʽ����(�жϷ�ʽ�������жϷ������е���)*/
        //HAL_UART_EnableTx(stdiouart,HAL_UART_INTERRUPT,HAL_UART_DISABLE);
        HAL_UART_EnableTx(stdiouart,HAL_UART_CHECK,HAL_UART_ENABLE);
        HAL_UART_CheckSend(stdiouart,(unsigned char)*buf); 
        //HAL_UART_EnableTx(stdiouart,HAL_UART_INTERRUPT,HAL_UART_ENABLE);
        ++buf;    
        ++nChars;  
    } 
    return nChars;
}

///**
// *****************************************************************************
// * \fn          int getchar(void)
// * \brief       �ַ�����ӿ�ʵ��.
// * \note        ��scanf����,����driver_uartʵ��. 
// * \return      int ���յ����ַ�
// *****************************************************************************
// */
//int getchar(void)
//{
//    int erro=0;
//    unsigned char ch=0;
//    driver_uart_recv(stdiouart, &ch, 1,0,&erro);
//    putchar(ch);
//    if (ch == '\r')
//    {
//        putchar('\n');
//    }
//    return ch;
//}

/**
 *****************************************************************************
 * \fn          size_t __read(int handle,unsigned char *buf,size_t bufSize)
 * \brief       ��׼����ӿ�ʵ��.
 * \note        ��scanf�Ⱥ�������,����driver_uartʵ��. 
 * \param[in]   handle ����
 * \param[in]   buf ���뻺����
 * \param[in]   bufSize ��Ҫ������ֽ���
 * \return      size_t ʵ������ĳ���
 *****************************************************************************
 */
size_t __read(int handle,unsigned char *buf,size_t bufSize)
{  
    size_t nChars = 0;  
    int erro=0;
    unsigned char ch=0;
    if (handle != 0)  
    {    
        return -1;  
    }   
    for (/*Empty*/; bufSize > 0; --bufSize)  
    {      
        driver_uart_recv(stdiouart, &ch, 1,0,&erro);
        *buf++ = ch;    
        ++nChars;  
        //driver_uart_send(stdiouart,&ch, 1,0,&erro);
        HAL_UART_CheckSend(stdiouart,(unsigned char)ch); 
        if (ch == '\r')
        {
            ch = '\n'; 
            HAL_UART_CheckSend(stdiouart,(unsigned char)ch); 
        }
    }   
    return nChars;
}
/**
 *****************************************************************************
 * \fn          void setstdiouart(HAL_UART_ID_e id)
 * \brief       ����stdioʹ�õĴ���.
 * \note        Ĭ�϶˿���Ч,��Ҫ���øú���ȥӳ��ʵ�ʶ�Ӧ�Ķ˿�. 
 * \param[in]   id stdioӳ�䵽�������ں�,�������Ҫʹ��stdio����,�˿�����ΪHAL_UART_MAX
 *****************************************************************************
 */
void setstdiouart(HAL_UART_ID_e id)
{
    stdiouart = id; 
}

/**
 *****************************************************************************
 * \fn          HAL_UART_ID_e getstdiouart(void)
 * \brief       ��ȡstdioʹ�õĴ���.
 * \note        Ĭ�϶˿���Ч,��Ҫ����setstdiouartȥӳ��ʵ�ʶ�Ӧ�Ķ˿�. 
 * \return      ����ʹ�õĴ���ID \ref HAL_UART_ID_e
 *****************************************************************************
 */
HAL_UART_ID_e getstdiouart(void)
{
    return stdiouart; 
}