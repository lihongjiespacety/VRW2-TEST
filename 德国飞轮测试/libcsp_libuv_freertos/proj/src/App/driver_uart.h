/**
 *****************************************************************************        
 * \brief       ������(DRIVER)����ģ��(UART)������ݽṹ�ͽӿ�����.
 * \details     Copyright (c) 2017,xxx.
 *              All rights reserved.    
 * \file        driver_uart.h 
 * \author      ����
 * \version     1.0 
 * \date        2017-07-24
 * \note        ʹ��ǰ��ο�ע��.\n
 *              ����HALʵ��,����OSAPI��ؽӿ�.\n
 * \since       ����      2017-7-24 16:07:47     1.0     �½� 
 * \par �޶���¼
 * - 2017-07-24 ��ʼ�汾
 * \par ��Դ˵��
 * - RAM:              
 * - ROM:
 *****************************************************************************
 */

#ifndef __DRIVER_UART_H
#define __DRIVER_UART_H

#ifdef __cplusplus
 extern "C" {
#endif
 
#include "hal_uart.h"
/** \addtogroup DRIVER ������(DRIVER)
 *  \{
 */
         
/** \addtogroup DRIVER_UART ������(DRIVER)����ģ��(UART)
 *  \{
 */
/*****************************************************************************    
 *                                                                           *
 *                             ���ݽṹ����                                  *
 *                                                                          *
 ****************************************************************************/
/** \defgroup DRIVER_UART_data ������(DRIVER)����ģ��(UART)���ݽṹ 
  * \{
  */
/*
UART0  HAL_UART_0 ����2
UART1  HAL_UART_1 ��MBUS/ά��
UART2  HAL_UART_2 ����3
UART5  HAL_UART_3 ����1
UART6  HAL_UART_4 ��MBUS
UART8  HAL_UART_5 ����485      
UART9  HAL_UART_6 �����ز�
UART12 HAL_UART_7 ����485
 */
     
#define UART0_ENABLE          1  /**< ͨ��0 ʹ�� */ 
#define UART1_ENABLE          1  /**< ͨ��1 ʹ�� */ 
#define UART2_ENABLE          1  /**< ͨ��2 ʹ�� */ 
#define UART3_ENABLE          1  /**< ͨ��3 ʹ�� */ 
#define UART4_ENABLE          1  /**< ͨ��4 ʹ�� */ 
#define UART5_ENABLE          1  /**< ͨ��5 ʹ�� */ 

     

     


#define DRIVER_UART0_TXBUF_SIZE 1      /**< UART0���ͻ�������С  */    
#define DRIVER_UART0_RXBUF_SIZE 1      /**< UART0���ջ�������С  */ 
#define DRIVER_UART1_TXBUF_SIZE 1      /**< UART1���ͻ�������С  */    
#define DRIVER_UART1_RXBUF_SIZE 1      /**< UART1���ջ�������С  */ 
#define DRIVER_UART2_TXBUF_SIZE 64       /**< UART2���ͻ�������С  */    
#define DRIVER_UART2_RXBUF_SIZE 128       /**< UART2���ջ�������С  */ 
#define DRIVER_UART3_TXBUF_SIZE 1       /**< UART3���ͻ�������С  */    
#define DRIVER_UART3_RXBUF_SIZE 1       /**< UART3���ջ�������С  */ 
#define DRIVER_UART4_TXBUF_SIZE (1024+128)       /**< UART4���ͻ�������С  */    
#define DRIVER_UART4_RXBUF_SIZE (1024+128)       /**< UART4���ջ�������С  */ 
#define DRIVER_UART5_TXBUF_SIZE (128)       /**< UART5���ͻ�������С  */    
#define DRIVER_UART5_RXBUF_SIZE (128)       /**< UART5���ջ�������С  */ 

#define DRIVER_UART_DELAY_FIFO    (20)    /**< ������������ʱʱ��       */   
#define DRIVER_UART_DELAY_CHECK   (1)      /**< ��������ʱ��ѯʱ����   */
#define DRIVER_UART_DELAY_FRAME   (20)    /**< �ֽڼ����ڸ�ֵ��Ϊ֡����   */
         
typedef void (*driver_uart_hook)(void);      /**< ����/���ջص�����   */   

/**
 * \struct driver_uart_t
 * UART�������ýṹ��.
 */
typedef struct 
{
    HAL_UART_ID_e id;                 /**< UART id   */   
    unsigned int rx_in;              /**< ���ջ�����д��ָ��       */ 
    unsigned int rx_out;             /**< ���ջ���������ָ��       */ 
    unsigned int rx_len;             /**< ���ջ�������Ч���ݴ�С   */ 
    unsigned int tx_in;              /**< ���ͻ�����д��ָ��       */ 
    unsigned int tx_out;             /**< ���ͻ���������ָ��      */ 
    unsigned int tx_len;             /**< ���ͻ�������Ч���ݴ�С   */ 
    unsigned short rxbuf_len;        /**< ���ջ�������С           */   
    unsigned short txbuf_len;        /**< ���ͻ�������С          */ 
    driver_uart_hook tx_hook;         /**< ׼�����ͻص�����          */ 
    driver_uart_hook rx_hook;         /**< ������ת���ջص�����          */ 
    unsigned char *rx_buf;           /**< ���ջ�����              */ 
    unsigned char *tx_buf;           /**< ���ͻ�����                */ 
    unsigned int bit_len;            /**< ���ڷ�������ʱ(�з������жϲ���Ҫ)    */ 
    HAL_UART_TXSTATE_e state;         /**< ����״̬��־�Ƿ��ڷ���                */ 
    unsigned char rxhavedata;        /**< �Ƿ��н��յ�����                */ 
    unsigned char txhavedata;        /**< �Ƿ��з�������                */ 
    unsigned int err;                /**< �����������               */ 
}driver_uart_t;


/*****************************************************************************    
 *                                                                           *
 *                             ģ�����ݶ���                                  *
 *                                                                          *
 ****************************************************************************/

#ifdef   DRIVER_UART_GLOBALS

/*485������ƻص�����:
  ����һ���������8·��������ʵ��8·,����и���·��������Ӽ���
 */
void uart5_txhook(void);
void uart5_rxhook(void);
void uart4_txhook(void);
void uart4_rxhook(void);
void uart3_txhook(void);
void uart3_rxhook(void);
void uart2_txhook(void);
void uart2_rxhook(void);
void uart1_txhook(void);
void uart1_rxhook(void);
void uart0_txhook(void);
void uart0_rxhook(void);

static unsigned char driver_uart0_rxbuf[DRIVER_UART0_RXBUF_SIZE];
static unsigned char driver_uart0_txbuf[DRIVER_UART0_TXBUF_SIZE];
static unsigned char driver_uart1_rxbuf[DRIVER_UART1_RXBUF_SIZE];
static unsigned char driver_uart1_txbuf[DRIVER_UART1_TXBUF_SIZE];
static unsigned char driver_uart2_rxbuf[DRIVER_UART2_RXBUF_SIZE];
static unsigned char driver_uart2_txbuf[DRIVER_UART2_TXBUF_SIZE];
static unsigned char driver_uart3_rxbuf[DRIVER_UART3_RXBUF_SIZE];
static unsigned char driver_uart3_txbuf[DRIVER_UART3_TXBUF_SIZE];
static unsigned char driver_uart4_rxbuf[DRIVER_UART4_RXBUF_SIZE];
static unsigned char driver_uart4_txbuf[DRIVER_UART4_TXBUF_SIZE];
static unsigned char driver_uart5_rxbuf[DRIVER_UART5_RXBUF_SIZE];
static unsigned char driver_uart5_txbuf[DRIVER_UART5_TXBUF_SIZE];


static driver_uart_t driver_uart0 = {HAL_UART_0, 0,0,0, 0,0,0, sizeof(driver_uart0_rxbuf), sizeof(driver_uart0_txbuf),uart0_txhook,uart0_rxhook,driver_uart0_rxbuf, driver_uart0_txbuf, 0,HAL_UART_TXIDLE,0};
static driver_uart_t driver_uart1 = {HAL_UART_1, 0,0,0, 0,0,0, sizeof(driver_uart1_rxbuf), sizeof(driver_uart1_txbuf),uart1_txhook,uart1_rxhook,driver_uart1_rxbuf, driver_uart1_txbuf, 0,HAL_UART_TXIDLE,0};
static driver_uart_t driver_uart2 = {HAL_UART_2, 0,0,0, 0,0,0, sizeof(driver_uart2_rxbuf), sizeof(driver_uart2_txbuf),uart2_txhook,uart2_rxhook,driver_uart2_rxbuf, driver_uart2_txbuf, 0,HAL_UART_TXIDLE,0};
static driver_uart_t driver_uart3 = {HAL_UART_3, 0,0,0, 0,0,0, sizeof(driver_uart3_rxbuf), sizeof(driver_uart3_txbuf),uart3_txhook,uart3_rxhook,driver_uart3_rxbuf, driver_uart3_txbuf, 0,HAL_UART_TXIDLE,0};
static driver_uart_t driver_uart4 = {HAL_UART_4, 0,0,0, 0,0,0, sizeof(driver_uart4_rxbuf), sizeof(driver_uart4_txbuf),uart4_txhook,uart4_rxhook,driver_uart4_rxbuf, driver_uart4_txbuf, 0,HAL_UART_TXIDLE,0};
static driver_uart_t driver_uart5 = {HAL_UART_5, 0,0,0, 0,0,0, sizeof(driver_uart5_rxbuf), sizeof(driver_uart5_txbuf),uart5_txhook,uart5_rxhook,driver_uart5_rxbuf, driver_uart5_txbuf, 0,HAL_UART_TXIDLE,0};


static driver_uart_t *driver_uart_tab[] = {&driver_uart0, &driver_uart1, &driver_uart2,&driver_uart3, &driver_uart4, &driver_uart5};


#endif

/**
  * \}
  */
/*****************************************************************************    
 *                                                                           
 *                             �ӿں�������                                   
 *                                                                            
 ****************************************************************************/

/** \defgroup DRIVER_UART_if ������(DRIVER)����ģ��(UART)�ӿ�
  * \{
  */
      
/**
 *****************************************************************************
 * \fn          unsigned int driver_uart_getrxlen(HAL_UART_ID_e id)
 * \brief       ��ȡ���ջ�������Ч���ݳ���.
 * \note        . 
 * \param[in]   id ����id \ref HAL_UART_ID_e 
 * \return      unsigned int ���ջ���������
 *****************************************************************************
 */
unsigned int driver_uart_getrxlen(HAL_UART_ID_e id);

/**
 *****************************************************************************
 * \fn          unsigned int driver_uart_flush(HAL_UART_ID_e id)
 * \brief       ���������.
 * \note        . 
 * \param[in]   id ����id \ref HAL_UART_ID_e 
 * \return      int ���Ƿ���0
 *****************************************************************************
 */
unsigned int driver_uart_flush(HAL_UART_ID_e id);

/**
 *****************************************************************************
 * \fn          int driver_uart_set(HAL_UART_CFG_t* cfg)
 * \brief       ���ô��ڲ���.
 * \note        . 
 * \param[in]   cfg ���ò���
 * \return      int \ref HAL_HWT_SetCfg() �ķ���ֵ
 *****************************************************************************
 */
int driver_uart_set(HAL_UART_CFG_t* cfg);

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
int driver_uart_recv(HAL_UART_ID_e id, unsigned char *pdata, unsigned int len, unsigned int timeout, int *erro);

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
int driver_uart_send(HAL_UART_ID_e id, unsigned char *pdata, unsigned int len,unsigned int timeout,int *erro);

/**
 *****************************************************************************
 * \fn          int driver_uart_haverxdata(HAL_UART_ID_e id)
 * \brief       �������Ƿ��������յ�������.
 * \note        Ӧ�����ڵ��øú��������ж��Ƿ��������ָʾ��.�������״̬.
 * \param[in]   id ָ���Ĵ���id.
 * \retval      1 ���������н�������
 * \retval      0 ����û�н�������
 *****************************************************************************
 */
int driver_uart_haverxdata(HAL_UART_ID_e id);

/**
 *****************************************************************************
 * \fn          int driver_uart_havetxdata(HAL_UART_ID_e id)
 * \brief       �������Ƿ��������͹�����.
 * \note        Ӧ�����ڵ��øú��������ж��Ƿ��������ָʾ��.�������״̬.
 * \param[in]   id ָ���Ĵ���id.
 * \retval      1 ���������з�������
 * \retval      0 ����û�з�������
 *****************************************************************************
 */
int driver_uart_havetxdata(HAL_UART_ID_e id);

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
int driver_uart_geterr(HAL_UART_ID_e id,unsigned int* err);
/**
  * \}
  */

/** \defgroup DRIVER_UART_samp ������(DRIVER)����ģ��(UART)ʹ������
  * \{
  */

/*****************************************************************************    
 *                                                                           
 *                             ʹ������                                   
 *                                                                            
 ****************************************************************************/

/**
 *****************************************************************************
 * \fn          void driver_uart_samp(void)
 * \brief       ʹ������.
 * \par   �μ�  ��ʾ������ǩ�µġ�driver_uart_samp.c��
 *****************************************************************************
 */
void driver_uart_samp(void);

/**
 *\example driver_uart_samp.c
 * ������(DRIVER)����ģ��(UART)ʹ������.\n
 */

/**
  * \}
  */

/**
  * \}
  */

/**
  * \}
  */

#ifdef __cplusplus
}
#endif

#endif


