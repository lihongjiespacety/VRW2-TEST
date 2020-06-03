/**
 *****************************************************************************        
 * \brief       ������(DRIVER)����(UART)��ؽӿ�ʵ��.
 * \details     Copyright (c) 2017,��ɳ��ʤ��Ϣ�������޹�˾.
 *              All rights reserved.    
 * \file        driver_uart.c 
 * \author      ����W18900
 * \version     1.0 
 * \date        2017-07-24
 * \note        ʹ��ǰ��ο�ע��.\n
 *              ����HALʵ��,����OSAPI��ؽӿ�.\n
 * \since       ����      2017-7-24 16:06:52     1.0     �½� 
 * \par �޶���¼
 * - 2017-07-24 ��ʼ�汾
 * \par ��Դ˵��
 * - RAM:              
 * - ROM:
 *****************************************************************************
 */

#define DRIVER_UART_GLOBALS /**< ����driver_uart.h�е����ݽ������*/

#include "driver.h"
#include "osapi_freertos.h"
   
/*****************************************************************************    
 *                                                                           
 *                             �ڲ�����                                   
 *                                                                            
 ****************************************************************************/


/*****************************************************************************    
 *                                                                           
 *                             �ڲ��ӿں���ʵ��                                   
 *                                                                            
 ****************************************************************************/

/*485������ƻص�����:
  ����һ���������8·��������ʵ��8·,����и���·��������Ӽ���
 */
#if UART0_ENABLE
static void uart0_txhook(void)
{
    //HAL_GPIO_WritePin(UART0_DIRCTRL_GPIO_ID,HAL_GPIO_INACTIVE);  /*��Ч��ƽ����*/
}

static void uart0_rxhook(void)
{
    //HAL_GPIO_WritePin(UART0_DIRCTRL_GPIO_ID,HAL_GPIO_ACTIVE);    /*��Ч��ƽ����*/
}
#endif

#if UART1_ENABLE
static void uart1_txhook(void)
{
    //HAL_GPIO_WritePin(UART1_DIRCTRL_GPIO_ID,HAL_GPIO_INACTIVE);  /*��Ч��ƽ����*/
}

static void uart1_rxhook(void)
{
    //HAL_GPIO_WritePin(UART1_DIRCTRL_GPIO_ID,HAL_GPIO_ACTIVE);    /*��Ч��ƽ���� */
}
#endif

#if UART2_ENABLE
static void uart2_txhook(void)
{
    //HAL_GPIO_WritePin(UART2_DIRCTRL_GPIO_ID,HAL_GPIO_INACTIVE);  /*��Ч��ƽ����*/
}

static void uart2_rxhook(void)
{
    //HAL_GPIO_WritePin(UART2_DIRCTRL_GPIO_ID,HAL_GPIO_ACTIVE);    /*��Ч��ƽ���� */
}
#endif

#if UART3_ENABLE
static void uart3_txhook(void)
{
    //HAL_GPIO_WritePin(UART3_DIRCTRL_GPIO_ID,HAL_GPIO_INACTIVE);  /*��Ч��ƽ����*/
}

static void uart3_rxhook(void)
{
    //HAL_GPIO_WritePin(UART3_DIRCTRL_GPIO_ID,HAL_GPIO_ACTIVE);    /*��Ч��ƽ���� */
}
#endif

#if UART4_ENABLE
static void uart4_txhook(void)
{
    //HAL_GPIO_WritePin(UART4_DIRCTRL_GPIO_ID,HAL_GPIO_INACTIVE);  /*��Ч��ƽ����*/
}

static void uart4_rxhook(void)
{
    //HAL_GPIO_WritePin(UART4_DIRCTRL_GPIO_ID,HAL_GPIO_ACTIVE);    /*��Ч��ƽ���� */
}
#endif

#if UART5_ENABLE
static void uart5_txhook(void)
{
    //HAL_GPIO_WritePin(UART5_DIRCTRL_GPIO_ID,HAL_GPIO_INACTIVE);  /*��Ч��ƽ����*/
}

static void uart5_rxhook(void)
{
    //HAL_GPIO_WritePin(UART5_DIRCTRL_GPIO_ID,HAL_GPIO_ACTIVE);    /*��Ч��ƽ���� */
}
#endif



/**
 *****************************************************************************
 * \fn          static int rx_fifo_in(driver_uart_t *p, unsigned char data)
 * \brief       д���ݵ����ջ�����.
 * \note        �����жϷ��������øú���д���ݵ����ջ�����,����������д��. 
 * \param[in]   p ָ��uart�ṹ��ʵ����ָ�� \ref driver_uart_t
 * \param[in]   data ָ��Ҫд��������������
 *****************************************************************************
 */
static int rx_fifo_in(driver_uart_t *p, unsigned char data)
{
    if(p==0)
    {
        return 0;
    }
    /*������δ��д������*/
    if (p->rx_len < (p->rxbuf_len/sizeof(p->rx_buf[0]))) 
    {   
        p->rx_buf[p->rx_in++] = data;                   /*д������,д��ָ�����*/
        p->rx_in %= p->rxbuf_len/sizeof(p->rx_buf[0]); /*�߽紦��,ʵ��Բ�λ�����*/
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
 * \fn          static unsigned char rx_fifo_out(driver_uart_t *p, unsigned int timeout, unsigned char *perro)
 * \brief       �ӽ��ջ�����������.
 * \note        Ӧ�ú������øú����ӽ��ջ�����������,����ָ����ʱʱ��. 
 * \param[in]   p ָ��uart�ṹ��ʵ����ָ�� \ref driver_uart_t
 * \param[in]   timeout ָ����ʱʱ�� 0��ʾһֱ�ȴ� 
 * \param[in]   perro ������0��ʾ���������� 1��ʾû�ж������� 
 * \retval      unsigned char �������ֽ�����.
 *****************************************************************************
 */
static unsigned char rx_fifo_out(driver_uart_t *p, unsigned int timeout, int *perro)
{
    unsigned char data = 0;
    if(p==0)
    {
        *perro = 1;
        return 0;
    }
    OSCriticalAlloc();
    /*timeout==0 ��ʾһֱ�ȴ�ֱ��������*/
    if (timeout == 0) 
    {
        while(p->rx_len == 0)
        {
            OsTimeDelay(DRIVER_UART_DELAY_CHECK);  /*��ʱ*/
        }
    } 
    else /*timeout!=0 ��ʾ�ȴ�ָ����ʱʱ��*/
    {
        while (timeout != 0)
        {
            if (p->rx_len == 0) 
            {
                /*���������,��ʱָ��ʱ����*/
                if (timeout >= DRIVER_UART_DELAY_CHECK)
                {
                    timeout -= DRIVER_UART_DELAY_CHECK;
                    OsTimeDelay(DRIVER_UART_DELAY_CHECK);
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
        data = p->rx_buf[p->rx_out++];
        p->rx_out %= p->rxbuf_len/sizeof(p->rx_buf[0]);
        *perro = 0;
    }
    else
    {
        *perro = 1;
    }
    OSCriticalExit();
    return data;
}


/**
 *****************************************************************************
 * \fn          static int tx_fifo_in(driver_uart_t *p, unsigned int timeout, unsigned char data,unsigned char *perro)
 * \brief       д���ݵ����ͻ�����.
 * \note        Ӧ�õ��øú�����д���ݵ����ͻ�����,���������ȴ�. 
 * \param[in]   p ָ��uart�ṹ��ʵ����ָ�� \ref driver_uart_t
 * \param[in]   timeout �趨��ʱʱ�� 0��ʾһֱ�ȴ�
 * \param[in]   data ָ��Ҫд��������������
 * \retval      0 д�ɹ�
 * \retval      1 дʧ��
 *****************************************************************************
 */
static int tx_fifo_in(driver_uart_t *p, unsigned char data, unsigned int timeout)
{
    /*ע��ȷ��p�������Ķ�д�����ٽ�α��� */
    int ret=0;
    if(p==0)
    {
        return 1;
    }
    OSCriticalAlloc();
    if (timeout == 0) 
    {
       
        while(1)
        {
            OSCriticalEnter();
            if(p->tx_len < (p->txbuf_len/sizeof(p->tx_buf[0])))
            {
                /*û������д������*/
                p->tx_buf[p->tx_in++] = data;
                p->tx_in %= p->txbuf_len/sizeof(p->tx_buf[0]);
                p->tx_len++;
                OSCriticalExit();
                break;
            }
            else
            {
                /*����ȴ�*/
                OSCriticalExit();
                OsTimeDelay(DRIVER_UART_DELAY_CHECK);  /*��ʱ*/
            }
        }
    } 
    while (timeout != 0)
    {
        OSCriticalEnter();
        if (p->tx_len < (p->txbuf_len/sizeof(p->tx_buf[0]))) 
        {   
            /*û������д������*/
            p->tx_buf[p->tx_in++] = data;
            p->tx_in %= p->txbuf_len/sizeof(p->tx_buf[0]);
            p->tx_len++;
            OSCriticalExit();
            ret = 0; /*������ȷ����ֵ*/
            break;
        } 
        else 
        {
            OSCriticalExit();
            /*���������,��ʱָ��ʱ����*/
            if (timeout >= DRIVER_UART_DELAY_CHECK)
            {
                timeout -= DRIVER_UART_DELAY_CHECK;
                OsTimeDelay(DRIVER_UART_DELAY_CHECK);
                ret = 1; /*���ó�ʱ���� ֮ǰ����û����ret=1 ��timeoutΪDRIVER_UART_DELAY_CHECK��������ʱ���ǻ᷵��0*/
            }
            else
            {
                /*����һ��ʱ������ʱʣ��ʱ��*/
                OsTimeDelay(timeout);
                timeout = 0;
                ret = 1; /*���ó�ʱ����*/
            }
        }
    }
    return ret;
}


/**
 *****************************************************************************
 * \fn          static unsigned char tx_fifo_out(driver_uart_t *p, unsigned char *perro)
 * \brief       �ӷ��ͻ�����������.
 * \note        �����жϷ��������øú�����ȡ���ͻ���������,���ڷ���. 
 * \param[in]   p ָ��uart�ṹ��ʵ����ָ�� \ref driver_uart_t
 * \param[out]  perro ������ 0��ʾ�������� 1��ʾû������
 * \return      unsigned char perro=0ʱ��ʾ����������  
 *****************************************************************************
 */
static unsigned char tx_fifo_out(driver_uart_t *p, int *perro)
{
    unsigned char data = 0;
    if(p==0)
    {
        *perro = 1;
        return 0;
    }
    if (p->tx_len != 0) 
    {
        if(p->tx_hook != 0)
        {
            (*(p->tx_hook))();
        }
        p->tx_len--;
        data = p->tx_buf[p->tx_out++];
        p->tx_out %= p->txbuf_len/sizeof(p->tx_buf[0]);
        *perro = 0;
    } 
    else
    {
        *perro = 1;
    }
    return data;
}



/**
 *****************************************************************************
 * \fn          static int driver_uart0_send_isr(unsigned char *pdata)
 * \brief       ���ʹ���.
 * \note        �����жϷ��������øú���,�������������ֹͣ���Ͳ����ûص�����. 
 * \param[out]  pdata ��д��Ҫ���͵�����
 * \retval      0 pdataָ���������Ч(��Ҫ����) 1�޺�����������
 *****************************************************************************
 */
#if UART0_ENABLE
static int driver_uart0_send_isr(unsigned char *pdata)
{
    int erro=0; 
    unsigned char data;
    if(driver_uart_tab[0]==0)
    {
        return 1;
    }
    data = tx_fifo_out(driver_uart_tab[0], &erro);
    if (erro != 0) 
    {  
        /*û�л�ȡ������,ֹͣ����*/
        HAL_UART_EnableTx(driver_uart_tab[0]->id,HAL_UART_INTERRUPT,HAL_UART_DISABLE);
        driver_uart_tab[0]->state = HAL_UART_TXIDLE;
        if(driver_uart_tab[0]->rx_hook != 0)
        {
            driver_uart_tab[0]->rx_hook();
        }   
        return 1;
    } 
    else
    {
        *pdata = data;
        return 0;
    }  
}
#endif

/**
 *****************************************************************************
 * \fn          static int driver_uart1_send_isr(unsigned char *pdata)
 * \brief       ���ʹ���.
 * \note        �����жϷ��������øú���,�������������ֹͣ���Ͳ����ûص�����. 
 * \param[out]  pdata ��д��Ҫ���͵�����
 * \retval      0 pdataָ���������Ч(��Ҫ����) 1�޺�����������
 *****************************************************************************
 */
#if UART1_ENABLE
static int driver_uart1_send_isr(unsigned char *pdata)
{
    int erro=0; 
    unsigned char data;
    if(driver_uart_tab[1]==0)
    {
        return 1;
    }
    data = tx_fifo_out(driver_uart_tab[1], &erro);
    if (erro != 0) 
    {  
        /*û�л�ȡ������,ֹͣ����*/
        HAL_UART_EnableTx(driver_uart_tab[1]->id,HAL_UART_INTERRUPT,HAL_UART_DISABLE);
        driver_uart_tab[1]->state = HAL_UART_TXIDLE;
        if(driver_uart_tab[1]->rx_hook != 0)
        {
            driver_uart_tab[1]->rx_hook();
        }   
        return 1;
    } 
    else
    {
        *pdata = data;
        return 0;
    }  
}
#endif

/**
 *****************************************************************************
 * \fn          static int driver_uart2_send_isr(unsigned char *pdata)
 * \brief       ���ʹ���.
 * \note        �����жϷ��������øú���,�������������ֹͣ���Ͳ����ûص�����. 
 * \param[out]  pdata ��д��Ҫ���͵�����
 * \retval      0 pdataָ���������Ч(��Ҫ����) 1�޺�����������
 *****************************************************************************
 */
#if UART2_ENABLE
static int driver_uart2_send_isr(unsigned char *pdata)
{
    int erro=0; 
    unsigned char data;
    if(driver_uart_tab[2]==0)
    {
        return 1;
    }
    data = tx_fifo_out(driver_uart_tab[2], &erro);
    if (erro != 0) 
    {  
        /*û�л�ȡ������,ֹͣ����*/
        HAL_UART_EnableTx(driver_uart_tab[2]->id,HAL_UART_INTERRUPT,HAL_UART_DISABLE);
        driver_uart_tab[2]->state = HAL_UART_TXIDLE;
        if(driver_uart_tab[2]->rx_hook != 0)
        {
            driver_uart_tab[2]->rx_hook();
        } 
        return 1;
    } 
    else
    {
        *pdata = data;
        return 0;
    }  
}
#endif

/**
 *****************************************************************************
 * \fn          static int driver_uart3_send_isr(unsigned char *pdata)
 * \brief       ���ʹ���.
 * \note        �����жϷ��������øú���,�������������ֹͣ���Ͳ����ûص�����. 
 * \param[out]  pdata ��д��Ҫ���͵�����
 * \retval      0 pdataָ���������Ч(��Ҫ����) 1�޺�����������
 *****************************************************************************
 */
#if UART3_ENABLE
static int driver_uart3_send_isr(unsigned char *pdata)
{
    int erro=0; 
    unsigned char data;
    if(driver_uart_tab[3]==0)
    {
        return 1;
    }
    data = tx_fifo_out(driver_uart_tab[3], &erro);
    if (erro != 0) 
    {  
        /*û�л�ȡ������,ֹͣ����*/
        HAL_UART_EnableTx(driver_uart_tab[3]->id,HAL_UART_INTERRUPT,HAL_UART_DISABLE);
        driver_uart_tab[3]->state = HAL_UART_TXIDLE;
        if(driver_uart_tab[3]->rx_hook != 0)
        {
            driver_uart_tab[3]->rx_hook();
        }   
        return 1;
    } 
    else
    {
        *pdata = data;
        return 0;
    }  
}
#endif

/**
 *****************************************************************************
 * \fn          static int driver_uart4_send_isr(unsigned char *pdata)
 * \brief       ���ʹ���.
 * \note        �����жϷ��������øú���,�������������ֹͣ���Ͳ����ûص�����. 
 * \param[out]  pdata ��д��Ҫ���͵�����
 * \retval      0 pdataָ���������Ч(��Ҫ����) 1�޺�����������
 *****************************************************************************
 */
#if UART4_ENABLE
static int driver_uart4_send_isr(unsigned char *pdata)
{
    int erro=0; 
    unsigned char data;
    if(driver_uart_tab[4]==0)
    {
        return 1;
    }
    data = tx_fifo_out(driver_uart_tab[4], &erro);
    if (erro != 0) 
    {  
        /*û�л�ȡ������,ֹͣ����*/
        HAL_UART_EnableTx(driver_uart_tab[4]->id,HAL_UART_INTERRUPT,HAL_UART_DISABLE);
        driver_uart_tab[4]->state = HAL_UART_TXIDLE;
        if(driver_uart_tab[4]->rx_hook != 0)
        {
            driver_uart_tab[4]->rx_hook();
        }  
        return 1;
    } 
    else
    {
      *pdata = data;
      return 0;
    }  
}
#endif

/**
 *****************************************************************************
 * \fn          static int driver_uart5_send_isr(unsigned char *pdata)
 * \brief       ���ʹ���.
 * \note        �����жϷ��������øú���,�������������ֹͣ���Ͳ����ûص�����. 
 * \param[out]  pdata ��д��Ҫ���͵�����
 * \retval      0 pdataָ���������Ч(��Ҫ����) 1�޺�����������
 *****************************************************************************
 */
#if UART5_ENABLE
static int driver_uart5_send_isr(unsigned char *pdata)
{
    int erro=0; 
    unsigned char data;
    if(driver_uart_tab[5]==0)
    {
        return 1;
    } 
    data = tx_fifo_out(driver_uart_tab[5], &erro);
    if (erro != 0) 
    {  
        /*û�л�ȡ������,ֹͣ����*/
        HAL_UART_EnableTx(driver_uart_tab[5]->id,HAL_UART_INTERRUPT,HAL_UART_DISABLE);
        driver_uart_tab[5]->state = HAL_UART_TXIDLE;
        if(driver_uart_tab[5]->rx_hook != 0)
        {
            driver_uart_tab[5]->rx_hook();
        }   
        return 1;
    } 
    else
    {
        *pdata = data;
        return 0;
    }  
}
#endif




/**
 *****************************************************************************
 * \fn          static int driver_uart0_recv_isr(unsigned char* data)
 * \brief       ���մ���.
 * \note        �����жϷ��������øú���,����������������µ�����. 
 * \param[out]  pdata �µ�����.
 * \retval      ���Ƿ���0
 *****************************************************************************
 */
#if UART0_ENABLE
 static int driver_uart0_recv_isr(unsigned char* data)
{
    if(driver_uart_tab[0]==0)
    {
        return 0;
    }
    driver_uart_tab[0]->rxhavedata =1;
    return rx_fifo_in(driver_uart_tab[0], *data);
}
#endif

/**
 *****************************************************************************
 * \fn          static int driver_uart1_recv_isr(unsigned char* data)
 * \brief       ���մ���.
 * \note        �����жϷ��������øú���,����������������µ�����. 
 * \param[out]  pdata �µ�����.
 * \retval      ���Ƿ���0
 *****************************************************************************
 */
#if UART1_ENABLE
static int driver_uart1_recv_isr(unsigned char* data)
{
    if(driver_uart_tab[1]==0)
    {
        return 0;
    }
    driver_uart_tab[1]->rxhavedata =1;
    return rx_fifo_in(driver_uart_tab[1], *data);
}
#endif

/**
 *****************************************************************************
 * \fn          static int driver_uart2_recv_isr(unsigned char* data)
 * \brief       ���մ���.
 * \note        �����жϷ��������øú���,����������������µ�����. 
 * \param[out]  pdata �µ�����.
 * \retval      ���Ƿ���0
 *****************************************************************************
 */
#if UART2_ENABLE
static int driver_uart2_recv_isr(unsigned char* data)
{
    if(driver_uart_tab[2]==0)
    {
        return 0;
    }
    driver_uart_tab[2]->rxhavedata =1;
    return rx_fifo_in(driver_uart_tab[2], *data);
}
#endif

/**
 *****************************************************************************
 * \fn          static int driver_uart3_recv_isr(unsigned char* data)
 * \brief       ���մ���.
 * \note        �����жϷ��������øú���,����������������µ�����. 
 * \param[out]  pdata �µ�����.
 * \retval      ���Ƿ���0
 *****************************************************************************
 */
#if UART3_ENABLE
static int driver_uart3_recv_isr(unsigned char* data)
{
    if(driver_uart_tab[3]==0)
    {
        return 0;
    }
    driver_uart_tab[3]->rxhavedata =1;
    return rx_fifo_in(driver_uart_tab[3], *data);
}
#endif

/**
 *****************************************************************************
 * \fn          static int driver_uart4_recv_isr(unsigned char* data)
 * \brief       ���մ���.
 * \note        �����жϷ��������øú���,����������������µ�����. 
 * \param[out]  pdata �µ�����.
 * \retval      ���Ƿ���0
 *****************************************************************************
 */
#if UART4_ENABLE
static int driver_uart4_recv_isr(unsigned char* data)
{
    if(driver_uart_tab[4]==0)
    {
        return 0;
    }
    driver_uart_tab[4]->rxhavedata =1;
    return rx_fifo_in(driver_uart_tab[4], *data);
}
#endif

/**
 *****************************************************************************
 * \fn          static int driver_uart5_recv_isr(unsigned char* data)
 * \brief       ���մ���.
 * \note        �����жϷ��������øú���,����������������µ�����. 
 * \param[out]  pdata �µ�����.
 * \retval      ���Ƿ���0
 *****************************************************************************
 */
#if UART5_ENABLE
static int driver_uart5_recv_isr(unsigned char* data)
{
    if(driver_uart_tab[5]==0)
    {
        return 0;
    }
    driver_uart_tab[5]->rxhavedata =1;
    return rx_fifo_in(driver_uart_tab[5], *data);
}
#endif



/**
 *****************************************************************************
 * \fn          static int driver_uart_rxerr_isr(HAL_UART_ID_e id,unsigned char *pdata)
 * \brief       ���մ�����.
 * \note        ���մ����жϷ��������øú���. 
 * \param[in]   pdata ������
 * \retval      0 ���Ƿ���0
 *****************************************************************************
 */
static int driver_uart_rxerr_isr(HAL_UART_ID_e id,unsigned char *pdata)
{
    if(driver_uart_tab[id]==0)
    {
        return 0;
    }
    if(*pdata & HAL_UART_ERR_PER)
    {
        if((driver_uart_tab[id]->err & 0x000000FF) != 0x000000FF)
        {
            driver_uart_tab[id]->err = driver_uart_tab[id]->err + 1;
        }
        else
        {
            HAL_UART_EnableRxErr(id,HAL_UART_INTERRUPT,HAL_UART_DISABLE);
            HAL_UART_EnableRx(id,HAL_UART_INTERRUPT,HAL_UART_DISABLE);
        }
    }
    if(*pdata & HAL_UART_ERR_FER)
    {
        if((driver_uart_tab[id]->err & 0x0000FF00) != 0x0000FF00)
        {
            driver_uart_tab[id]->err = driver_uart_tab[id]->err + 0x100;
        }
        else
        {
            HAL_UART_EnableRxErr(id,HAL_UART_INTERRUPT,HAL_UART_DISABLE);
            HAL_UART_EnableRx(id,HAL_UART_INTERRUPT,HAL_UART_DISABLE);
        }
    }
    if(*pdata & HAL_UART_ERR_ORER)
    {
        if((driver_uart_tab[id]->err & 0x00FF0000) != 0x00FF0000)
        {
            driver_uart_tab[id]->err = driver_uart_tab[id]->err + 0x10000;
        }
        else
        {
            HAL_UART_EnableRxErr(id,HAL_UART_INTERRUPT,HAL_UART_DISABLE);
            HAL_UART_EnableRx(id,HAL_UART_INTERRUPT,HAL_UART_DISABLE);
        }
    }
    return 0;
}

/**
 *****************************************************************************
 * \fn          static int driver_uart0_rxerr_isr(unsigned char *pdata)
 * \brief       ���մ�����.
 * \note        ���մ����жϷ��������øú���. 
 * \param[in]   pdata ������
 * \retval      0 ���Ƿ���0
 *****************************************************************************
 */
#if UART0_ENABLE
static int driver_uart0_rxerr_isr(unsigned char *pdata)
{
    return driver_uart_rxerr_isr(HAL_UART_0,pdata);
}
#endif

/**
 *****************************************************************************
 * \fn          static int driver_uart1_rxerr_isr(unsigned char *pdata)
 * \brief       ���մ�����.
 * \note        ���մ����жϷ��������øú���. 
 * \param[in]   pdata ������
 * \retval      0 ���Ƿ���0
 *****************************************************************************
 */
#if UART1_ENABLE
static int driver_uart1_rxerr_isr(unsigned char *pdata)
{
    return driver_uart_rxerr_isr(HAL_UART_1,pdata);
}
#endif

/**
 *****************************************************************************
 * \fn          static int driver_uart2_rxerr_isr(unsigned char *pdata)
 * \brief       ���մ�����.
 * \note        ���մ����жϷ��������øú���. 
 * \param[in]   pdata ������
 * \retval      0 ���Ƿ���0
 *****************************************************************************
 */
#if UART2_ENABLE
static int driver_uart2_rxerr_isr(unsigned char *pdata)
{
    return driver_uart_rxerr_isr(HAL_UART_2,pdata);
}
#endif

/**
 *****************************************************************************
 * \fn          static int driver_uart3_rxerr_isr(unsigned char *pdata)
 * \brief       ���մ�����.
 * \note        ���մ����жϷ��������øú���. 
 * \param[in]   pdata ������
 * \retval      0 ���Ƿ���0
 *****************************************************************************
 */
#if UART3_ENABLE
static int driver_uart3_rxerr_isr(unsigned char *pdata)
{
    return driver_uart_rxerr_isr(HAL_UART_3,pdata);
}
#endif

/**
 *****************************************************************************
 * \fn          static int driver_uart4_rxerr_isr(unsigned char *pdata)
 * \brief       ���մ�����.
 * \note        ���մ����жϷ��������øú���. 
 * \param[in]   pdata ������
 * \retval      0 ���Ƿ���0
 *****************************************************************************
 */
#if UART4_ENABLE
static int driver_uart4_rxerr_isr(unsigned char *pdata)
{
    return driver_uart_rxerr_isr(HAL_UART_4,pdata);
}
#endif

/**
 *****************************************************************************
 * \fn          static int driver_uart5_rxerr_isr(unsigned char *pdata)
 * \brief       ���մ�����.
 * \note        ���մ����жϷ��������øú���. 
 * \param[in]   pdata ������
 * \retval      0 ���Ƿ���0
 *****************************************************************************
 */
#if UART5_ENABLE
static int driver_uart5_rxerr_isr(unsigned char *pdata)
{
    return driver_uart_rxerr_isr(HAL_UART_5,pdata);
}
#endif


/*****************************************************************************    
 *                                                                           
 *                             ����ӿں���ʵ��                                   
 *                                                                            
 ****************************************************************************/

/**
 *****************************************************************************
 * \fn          static int driver_uart_init(HAL_UART_ID_e id)
 * \brief       ��ʼ������.
 * \note        . 
 * \param[in]   id ����id \ref HAL_UART_ID_e 
 * \retval      0 ���óɹ�
 * \retval      -1��������
 *****************************************************************************
 */
static int driver_uart_init(HAL_UART_ID_e id)
{	
    /*��ʼ���˿ں�Ĭ�ϲ���*/
    HAL_UART_Init(id);
    /*���ûص�����*/
    switch(id)
    {
#if UART0_ENABLE
    case HAL_UART_0:
        HAL_UART_SetTxCmpISRCallBack(HAL_UART_0,driver_uart0_send_isr);
        HAL_UART_SetRxCallBack(HAL_UART_0,driver_uart0_recv_isr);
        HAL_UART_SetRxErrCallBack(HAL_UART_0,driver_uart0_rxerr_isr);
        break;
#endif
#if UART1_ENABLE
    case HAL_UART_1:
        HAL_UART_SetTxCmpISRCallBack(HAL_UART_1,driver_uart1_send_isr);
        HAL_UART_SetRxCallBack(HAL_UART_1,driver_uart1_recv_isr);
        HAL_UART_SetRxErrCallBack(HAL_UART_1,driver_uart1_rxerr_isr);
        break;
#endif
#if UART2_ENABLE   
    case HAL_UART_2:
        HAL_UART_SetTxCmpISRCallBack(HAL_UART_2,driver_uart2_send_isr);
        HAL_UART_SetRxCallBack(HAL_UART_2,driver_uart2_recv_isr);
        HAL_UART_SetRxErrCallBack(HAL_UART_2,driver_uart2_rxerr_isr);
        break;
#endif
#if UART3_ENABLE
    case HAL_UART_3:
        HAL_UART_SetTxCmpISRCallBack(HAL_UART_3,driver_uart3_send_isr);
        HAL_UART_SetRxCallBack(HAL_UART_3,driver_uart3_recv_isr);
        HAL_UART_SetRxErrCallBack(HAL_UART_3,driver_uart3_rxerr_isr);
        break;
#endif
#if UART4_ENABLE
    case HAL_UART_4:
        HAL_UART_SetTxCmpISRCallBack(HAL_UART_4,driver_uart4_send_isr);
        HAL_UART_SetRxCallBack(HAL_UART_4,driver_uart4_recv_isr);
        HAL_UART_SetRxErrCallBack(HAL_UART_4,driver_uart4_rxerr_isr);
        break;
#endif
#if UART5_ENABLE 
    case HAL_UART_5:
        HAL_UART_SetTxCmpISRCallBack(HAL_UART_5,driver_uart5_send_isr);
        HAL_UART_SetRxCallBack(HAL_UART_5,driver_uart5_recv_isr);
        HAL_UART_SetRxErrCallBack(HAL_UART_5,driver_uart5_rxerr_isr);
        break;
#endif
#if UART6_ENABLE
    case HAL_UART_6:
        HAL_UART_SetTxCmpISRCallBack(HAL_UART_6,driver_uart6_send_isr);
        HAL_UART_SetRxCallBack(HAL_UART_6,driver_uart6_recv_isr);
        HAL_UART_SetRxErrCallBack(HAL_UART_6,driver_uart6_rxerr_isr);
        break;
#endif
#if UART7_ENABLE
    case HAL_UART_7:
        HAL_UART_SetTxCmpISRCallBack(HAL_UART_7,driver_uart7_send_isr);
        HAL_UART_SetRxCallBack(HAL_UART_7,driver_uart7_recv_isr);
        HAL_UART_SetRxErrCallBack(HAL_UART_7,driver_uart7_rxerr_isr);
        break;
#endif
    default:
        return -1;
        break;
    }
    return 0;
}

/**
 *****************************************************************************
 * \fn          unsigned int driver_uart_getrxlen(HAL_UART_ID_e id)
 * \brief       ��ȡ���ջ�������Ч���ݳ���.
 * \note        . 
 * \param[in]   id ����id \ref HAL_UART_ID_e 
 * \return      unsigned int ���ջ���������
 *****************************************************************************
 */
unsigned int driver_uart_getrxlen(HAL_UART_ID_e id)
{
    if(driver_uart_tab[id]==0)
    {
        return 0;
    }
    return driver_uart_tab[id]->rx_len;
}


/**
 *****************************************************************************
 * \fn          unsigned int driver_uart_flush(HAL_UART_ID_e id)
 * \brief       ���������.
 * \note        . 
 * \param[in]   id ����id \ref HAL_UART_ID_e 
 * \return      int ���Ƿ���0
 *****************************************************************************
 */
unsigned int driver_uart_flush(HAL_UART_ID_e id)
{
    if(driver_uart_tab[id]!=0)
    {
        driver_uart_tab[id]->rx_in  = 0;
        driver_uart_tab[id]->rx_out = 0;
        driver_uart_tab[id]->rx_len = 0;
        driver_uart_tab[id]->tx_in  = 0;
        driver_uart_tab[id]->tx_out = 0;
        driver_uart_tab[id]->tx_len = 0;
        driver_uart_tab[id]->err = 0x00;
    }
    return 0;
}

/**
 *****************************************************************************
 * \fn          int driver_uart_set(HAL_UART_CFG_t* cfg)
 * \brief       ���ô��ڲ���.
 * \note        . 
 * \param[in]   cfg ���ò���
 * \return      int \ref HAL_UART_SetCfg �ķ���ֵ
 *****************************************************************************
 */
int driver_uart_set(HAL_UART_CFG_t* cfg)
{
    int ret;
    if(driver_uart_tab[cfg->id]==0)
    {
        return 0;
    }
    driver_uart_init(cfg->id);
    ret = HAL_UART_SetCfg(cfg->id,cfg);
    HAL_UART_EnableRx(cfg->id,HAL_UART_INTERRUPT,HAL_UART_ENABLE);
    driver_uart_flush(cfg->id);
    return ret;
}

/**
 *****************************************************************************
 * \fn          int driver_uart_recv(HAL_UART_ID_e id, unsigned char *pdata, unsigned int len, unsigned int timeout, int *perro)
 * \brief       ���ڽ���ָ����������,�������ó�ʱʱ��.
 * \note        Ӧ�õ��øú�����������,��������������ݾ͵ȴ�.
 * \param[in]   id ָ���Ĵ���id.
 * \param[in]   pdata ��洢�������ݵĻ�����.
 * \param[in]   len   ���յ����ݳ���.
 * \param[in]   timeout ����Ϊ0��ʾһֱ�ȵ����յ�ָ����������,����Ϊ����ֵ��ʾ�ȴ�ָ����ʱʱ��(��λmS)
 * \param[out]  perro ������ 0��ʾ�ɹ� -1��ʾ�������� 1��ʾ��ʱ
 * \return      int ����ʵ�ʶ��������ݳ���
 *****************************************************************************
 */
int driver_uart_recv(HAL_UART_ID_e id, unsigned char *pdata, unsigned int len, unsigned int timeout, int *erro)
{
    unsigned char data=0;
    unsigned int readlen=0;
    unsigned int tmptimeout=0;
    if (id >= HAL_UART_MAX)
    {
        *erro = -1;
        return 0;
    }
    if(driver_uart_tab[id]==0)
    {
        *erro = -1;
        return 0;
    }
    if(len ==0)
    {
        *erro = -1;
        return 0;
    }
    tmptimeout = timeout; 
    while(len !=0)
    {
        data = rx_fifo_out(driver_uart_tab[id], tmptimeout, erro);
        if (*erro == 0) 
        {       
            tmptimeout = DRIVER_UART_DELAY_FRAME;
            *pdata++ = data;   
            len--;
            readlen++;
        } 
        else
        {
            *erro = 1;
            break;
        }
    }
    return readlen; 
}

/**
 *****************************************************************************
 * \fn          int driver_uart_send(HAL_UART_ID_e id, unsigned char *pdata, unsigned int len,unsigned int timeout,int *perro)
 * \brief       ���ڷ���ָ����������,�������ó�ʱʱ��.
 * \note        Ӧ�õ��øú�����������,������������͵ȴ�.
 * \param[in]   id ָ���Ĵ���id.
 * \param[in]   pdata ��Ҫ���͵����ݻ�����.
 * \param[in]   len   ��Ҫ���͵����ݳ���.
 * \param[in]   timeout ����Ϊ0��ʾһֱ�ȵ�ȫ��д�뻺����,����Ϊ����ֵ��ʾ�ȴ�ָ����ʱʱ��(��λmS)
 * \param[out]  perro ������ 0��ʾ�ɹ� -1��ʾ�������� 1��ʾ��ʱ
 * \return      int ����ʵ�ʷ��͵�����
 *****************************************************************************
 */
int driver_uart_send(HAL_UART_ID_e id, unsigned char *pdata, unsigned int len,unsigned int timeout,int *erro)
{
    unsigned char full=0;
    driver_uart_t *p=0;
    unsigned char data=0;
    unsigned int sendlen=0;
    OSCriticalAlloc();
    if (id >= HAL_UART_MAX)
    {
        *erro = -1;
        return 0;
    }
    if(driver_uart_tab[id]==0)
    {
        *erro = -1;
        return 0;
    }
    if(len ==0)
    {
         *erro = -1;
        return 0;
    }
    p = (driver_uart_t *)driver_uart_tab[id];
    if(p->tx_hook !=0)
    {
        p->tx_hook();   /*����׼�����ͻص�����*/
    }
    driver_uart_tab[id]->txhavedata =1;
    if(timeout == 0)
    {
        while(len !=0)
        {
            full = tx_fifo_in(p,*pdata,DRIVER_UART_DELAY_CHECK);
            if (full == 0) 
            {
                pdata++;
                len--;
                sendlen++;
            } 
            else 
            {
                OSCriticalEnter();
                /*��������������ҵ�ǰ����û�д��ڷ���״̬Ҫ�ȷ���һ���ֽڴ��������ж���*/
                if(p->state == HAL_UART_TXIDLE)
                {
                    data = tx_fifo_out(p, erro);
                    if(*erro == 0)
                    {
                        if(p->tx_hook !=0)
                        {
                            p->tx_hook();   /*����׼�����ͻص�����*/
                        }
                        p->state = HAL_UART_TXBUSY; /*ע��:p->state�Ķ�д��Ҫ�ٽ籣��*/
#ifdef FM33A
                        HAL_UART_EnableTx(id,HAL_UART_INTERRUPT,HAL_UART_ENABLE);
                        HAL_UART_Send(p->id,data);   /*ע��:�������ʹ��TX��,����ʹ��TE�����������д�������,�������ж� ������k20��оƬ��ͬ*/
#else
                        HAL_UART_Send(p->id,data);  /*ע��:�������ʹ��TXǰ,дTDR�����TEND��־,����ʹ��TE�����ϲ����������ж�*/
                        HAL_UART_EnableTx(id,HAL_UART_INTERRUPT,HAL_UART_ENABLE);      
#endif
                    }
                }
                OSCriticalExit();
                OsTimeDelay(DRIVER_UART_DELAY_CHECK);
            }
        }
        *erro = 0;
    }
    else
    {
        while ((len != 0) && (timeout!=0)) 
        {
            full = tx_fifo_in(p,*pdata,DRIVER_UART_DELAY_CHECK);
            if (full == 0) 
            {
                pdata++;
                len--;
                sendlen++;
            } 
            else 
            {
                OSCriticalEnter();
                /*��������������ҵ�ǰ����û�д��ڷ���״̬Ҫ�ȷ���һ���ֽڴ��������ж���*/
                if(p->state == HAL_UART_TXIDLE)
                {
                    data = tx_fifo_out(p, erro);
                    if(*erro == 0)
                    {
                        if(p->tx_hook !=0)
                        {
                            p->tx_hook();   /*����׼�����ͻص�����*/
                        }
                        p->state = HAL_UART_TXBUSY; /*ע��:p->state�Ķ�д��Ҫ�ٽ籣��*/
#ifdef FM33A
                        HAL_UART_EnableTx(id,HAL_UART_INTERRUPT,HAL_UART_ENABLE);
                        HAL_UART_Send(p->id,data);   /*ע��:�������ʹ��TX��,����ʹ��TE�����������д�������,�������ж� ������k20��оƬ��ͬ*/
#else
                        HAL_UART_Send(p->id,data);  /*ע��:�������ʹ��TXǰ,дTDR�����TEND��־,����ʹ��TE�����ϲ����������ж�*/
                        HAL_UART_EnableTx(id,HAL_UART_INTERRUPT,HAL_UART_ENABLE);      
#endif
                    }
                    OSCriticalExit();
                }
                else /*�����ǰ���ڷ���״̬*/
                {
                    OSCriticalExit();
                    /*��������������Ҵ��ڴ��ڷ���״̬����ʱָ��ʱ�����ȴ��жϷ�����*/
                    if (timeout >= DRIVER_UART_DELAY_CHECK)
                    {
                        timeout -= DRIVER_UART_DELAY_CHECK;
                        OsTimeDelay(DRIVER_UART_DELAY_CHECK);
                    }
                    else
                    {
                        /*����һ��ʱ������ʱʣ��ʱ��*/
                        OsTimeDelay(timeout);
                        timeout = 0;
                    }
                }
            }
        }
        /*���û���������ó�ʱ������*/
        if(len != 0)
        {
            *erro = 1;
        }
    }
    

    OSCriticalEnter();
    /*��������������ݶ��ҵ�ǰ����û�д��ڷ���״̬Ҫ�ȷ���һ���ֽڴ��������ж���*/
    if((p->state == HAL_UART_TXIDLE) && (p->tx_len!=0))
    {
        data = tx_fifo_out(p, erro);
        if(*erro == 0)
        {
            if(p->tx_hook !=0)
            {
                p->tx_hook();   /*����׼�����ͻص�����*/
            }
            p->state = HAL_UART_TXBUSY; /*ע��:p->state�Ķ�д��Ҫ�ٽ籣��*/
#ifdef FM33A
            HAL_UART_EnableTx(id,HAL_UART_INTERRUPT,HAL_UART_ENABLE);
            HAL_UART_Send(p->id,data);   /*ע��:�������ʹ��TX��,����ʹ��TE�����������д�������,�������ж� ������k20��оƬ��ͬ*/
#else
            HAL_UART_Send(p->id,data);  /*ע��:�������ʹ��TXǰ,дTDR�����TEND��־,����ʹ��TE�����ϲ����������ж�*/
            HAL_UART_EnableTx(id,HAL_UART_INTERRUPT,HAL_UART_ENABLE);      
#endif
        }
    }
    OSCriticalExit();
    return sendlen;
}

/**
 *****************************************************************************
 * \fn          int driver_uart_haverxdata(HAL_UART_ID_e id)
 * \brief       �������Ƿ��������յ�������.
 * \note        Ӧ�õ��øú��������ж��Ƿ��������ָʾ��.�������״̬.
 * \param[in]   id ָ���Ĵ���id.
 * \retval      1 ���������н�������
 * \retval      0 ����û�н�������
 *****************************************************************************
 */
int driver_uart_haverxdata(HAL_UART_ID_e id)
{
    unsigned char havedata=0;
    if(driver_uart_tab[id]==0)
    {
        return 0;
    }
    havedata = driver_uart_tab[id]->rxhavedata;
    driver_uart_tab[id]->rxhavedata =0;
    return havedata;
}

/**
 *****************************************************************************
 * \fn          int driver_uart_havetxdata(HAL_UART_ID_e id)
 * \brief       �������Ƿ��������͹�����.
 * \note        Ӧ�õ��øú��������ж��Ƿ��������ָʾ��.�������״̬.
 * \param[in]   id ָ���Ĵ���id.
 * \retval      1 ���������з�������
 * \retval      0 ����û�з�������
 *****************************************************************************
 */
int driver_uart_havetxdata(HAL_UART_ID_e id)
{
    unsigned char havedata=0;
    if(driver_uart_tab[id]==0)
    {
        return 0;
    }
    havedata = driver_uart_tab[id]->txhavedata;
    driver_uart_tab[id]->txhavedata =0;
    return havedata;
}

/**
 *****************************************************************************
 * \fn          int driver_uart_geterr(HAL_UART_ID_e id,unsigned int* err)
 * \brief       �����ڴ���״̬��.
 * \note        Ӧ�õ��øú��������жϴ����Ƿ��쳣,���err�ĵ������ֽ���������ֽ�ΪFF���ʾ�쳣��Ҫ����.
 * \param[in]   id ָ���Ĵ���id \ref HAL_UART_ID_e .
 * \param[out]  err �洢������
                0xxxxxxxFF��ʾУ�����  
                0xxxxxFFxx��ʾ֡����   
                0xxxFF00xx��ʾ�������  
 * \retval      0 ���Ƿ���0
 *****************************************************************************
 */
int driver_uart_geterr(HAL_UART_ID_e id,unsigned int* err)
{
    if(driver_uart_tab[id]==0)
    {
        return 0;
    }
    *err = driver_uart_tab[id]->err;
    return 0;
}

