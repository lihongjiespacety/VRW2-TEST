/**
 *****************************************************************************        
 * \brief       Ӳ�������(HAL)����ģ��(UART)������ݽṹ�ͽӿ�����.
 * \details     Copyright (c) 2017,��ɳ��ʤ��Ϣ�������޹�˾.
 *              All rights reserved.    
 * \file        hal_uart.h  
 * \author      ����W18900
 * \version     1.0 
 * \date        2017-07-23
 * \note        ʹ��ǰ��ο�ע��.\n
 *              HALֻ����������,������Դ��BSPʵ��.\n
 * \since       ����      2017-7-23 14:24:11     1.0     �½� 
 * \par �޶���¼
 * - 2017-07-23 ��ʼ�汾
 * \par ��Դ˵��
 * - RAM:              
 * - ROM:
 *****************************************************************************
 */

#ifndef __HAL_UART_H
#define __HAL_UART_H

#ifdef __cplusplus
 extern "C" {
#endif

     
/** \addtogroup HAL
 *  \{
 */    
     
/** \addtogroup HAL_UART Ӳ�������(HAL)����ģ��(UART)
 *  \{
 */
/*****************************************************************************    
 *                                                                           *
 *                             ���ݽṹ����                                  *
 *                                                                          *
 ****************************************************************************/
   

/** \defgroup HAL_UART_data Ӳ�������(HAL)����ģ��(UART)���ݽṹ
  * \{
  */
     
typedef enum  hal_uart_txstate HAL_UART_TXSTATE_e; /**< UART ����״̬ö�ٶ���  */
   	      
/**
 * \enum hal_uart_txstate
 * UART ����״̬.
 */
enum hal_uart_txstate
{
    HAL_UART_TXIDLE=0,      /**< UART ���Ϳ��� */ 
    HAL_UART_TXBUSY=1,      /**< UART ���ڷ��� */ 
};


typedef enum  hal_uart_enable HAL_UART_ENABLE_e; /**< UART ʹ��ö�ٶ���  */
   	      
/**
 * \enum hal_uart_enable
 * UART ʹ��ö�ٶ���.
 */
enum hal_uart_enable
{
    HAL_UART_DISABLE=0,     /**< UART ��ʹ�� */ 
    HAL_UART_ENABLE=1,      /**< UART ʹ�� */ 
};

typedef enum  hal_uart_type HAL_UART_TYPE_e; /**< UART ����ʽö�ٶ���  */
   	      
/**
 * \enum hal_uart_type
 * UART ����ʽö�ٶ���.
 */
enum hal_uart_type
{
    HAL_UART_INTERRUPT=0,     /**< UART �жϷ�ʽ */ 
    HAL_UART_CHECK=1,         /**< UART ��ѯ��ʽ */ 
};


     
typedef enum  hal_uart_id HAL_UART_ID_e; /**< UART IDö�ٶ���  */
   	      
/**
 * \enum hal_uart_id
 * UART IDö�ٶ���.
 * \note id��0��ʼ��������,HALԤ���㹻���id��\n
 *       ʵ���õ���id����BSPӲ����Դʵ�־���.
 */
enum hal_uart_id
{
    HAL_UART_0=0,           /**< UART ID0 */ 
    HAL_UART_1=1,           /**< UART ID1 */
    HAL_UART_2=2,           /**< UART ID2 */ 
    HAL_UART_3=3,           /**< UART ID3 */
    HAL_UART_4=4,           /**< UART ID4 */ 
    HAL_UART_5=5,           /**< UART ID5 */
    HAL_UART_6=6,           /**< UART ID6 */ 
    HAL_UART_7=7,           /**< UART ID7 */
    HAL_UART_MAX=8,         /**< UART ���� */
    HAL_UART_INV=0xFF,      /**< ��ЧID */
};

typedef enum  hal_uart_parity HAL_UART_PARITY_e; /**< UART������żУ��ö�ٶ���  */
   	      
/**
 * \enum hal_uart_parity
 * UART������żУ��ö�ٶ���.
 * \note HAL���������б�׼У�鷽ʽ,\n
 *       ʵ���õ���У����BSPӲ����Դʵ�־���.
 */
enum hal_uart_parity
{
    HAL_UART_PARITY_NONE=0,       /**< UART�޼��� */ 
    HAL_UART_PARITY_EVEN=1,       /**< UARTżУ�� */
    HAL_UART_PARITY_ODD=2,        /**< UART��У�� */
    HAL_UART_PARITY_HIG=3,        /**< UARTУ��̶�1  */
    HAL_UART_PARITY_LOW=4,        /**< UARTУ��̶�0  */
};

typedef enum  hal_uart_datalen HAL_UART_DATALEN_e; /**< UART���ݳ���ö�ٶ���  */
   	      
/**
 * \enum hal_uart_datalen
 * UART���ݳ���ö�ٶ���.
 * \note HAL���������б�׼���ݳ���,\n
 *       ʵ���õ������ݳ�����BSPӲ����Դʵ�־���.
 */
enum hal_uart_datalen
{
    HAL_UART_DATA_9=9,       /**< UART 9λ���� */ 
    HAL_UART_DATA_8=8,       /**< UART 8λ���� */ 
    HAL_UART_DATA_7=7,       /**< UART 7λ���� */
    HAL_UART_DATA_6=6,       /**< UART 6λ���� */ 
    HAL_UART_DATA_5=5,       /**< UART 5λ���� */
};

typedef enum  hal_uart_stopb HAL_UART_STOPB_e; /**< UARTֹͣλö�ٶ���  */
   	      
/**
 * \enum hal_uart_stopb
 * UARTֹͣλö�ٶ���.
 * \note HAL���������б�׼ֹͣλ����,\n
 *       ʵ���õ���ֹͣλ������BSPӲ����Դʵ�־���.
 */
enum hal_uart_stopb
{
    HAL_UART_STOPB_1P5=0,       /**< UART 1.5λֹͣλ */
    HAL_UART_STOPB_1=1,         /**< UART 1λֹͣλ */ 
    HAL_UART_STOPB_2=2,         /**< UART 2λֹͣλ */
};


typedef enum  hal_uart_baud HAL_UART_BAUD_e; /**< UART������ö�ٶ���  */
   	      
/**
 * \enum hal_uart_baud
 * UART������ö�ٶ���.
 * \note HAL�����˳���������,\n
 *       ʵ��֧�ֵĲ�������BSPӲ����Դʵ�־���.
 */
enum hal_uart_baud
{
    HAL_UART_BAUD_600=600,        /**< UART������600 */ 
    HAL_UART_BAUD_1200=1200,      /**< UART������1200 */
    HAL_UART_BAUD_2400=2400,      /**< UART������2400 */ 
    HAL_UART_BAUD_4800=4800,      /**< UART������4800 */
    HAL_UART_BAUD_9600=9600,      /**< UART������9600 */ 
    HAL_UART_BAUD_19200=19200,    /**< UART������19200 */
    HAL_UART_BAUD_38400=38400,    /**< UART������38400 */ 
    HAL_UART_BAUD_57600=57600,    /**< UART������38400 */ 
    HAL_UART_BAUD_115200=115200,  /**< UART������115200 */
    HAL_UART_BAUD_500000=500000,  /**< UART������500000 */
};

typedef struct hal_uart_cfg HAL_UART_CFG_t;  /**< UART���ýṹ��  */

/**
 * \struct hal_uart_cfg
 * UART���ýṹ��.
 */
struct hal_uart_cfg
{
    HAL_UART_ID_e       id;         /**< UART id    */
    HAL_UART_DATALEN_e  datalen;    /**< UART���ݳ���*/  
    HAL_UART_PARITY_e   parity;     /**< UARTУ��    */ 
    HAL_UART_STOPB_e    stopb;      /**< UARTֹͣλ  */ 
    HAL_UART_BAUD_e     baud;       /**< UART������  */   
}; 

typedef enum  hal_uart_err HAL_UART_ERR_e; /**< UART��������ö�ٶ���  */
   	      
/**
 * \enum hal_uart_err
 * UART��������ö�ٶ���.
 */
enum hal_uart_err
{
    HAL_UART_ERR_NONE=0,           /**< UART�޴��� */ 
    HAL_UART_ERR_PER=1,            /**< UARTУ����� */ 
    HAL_UART_ERR_FER=2,            /**< UART֡���� */
    HAL_UART_ERR_ORER=4,           /**< UART������� */ 
};

/**
 * \typedef HAL_UART_ISRCallBack
 * �жϷ���ص�����.
 * - rxerrfun�������ص�����ʱ,paramָ��������ֽ�����,����Ҫ����ֵ.
 * - rxfun���뻺�����ص�����ʱ,paramָ��������ֽ�����,����Ҫ����ֵ.
 * - txfun���ͻ���ջص�����,��Ҫ��paramд��Ҫ���͵��ֽ�,����ֵ0��ʾ����������Ҫ���� ��������ֵ��־����Ϊ���һ�ֽ�����.
 * - txcmpfun������ص�����,��Ҫ��paramд��Ҫ���͵��ֽ�,����ֵ0��ʾ����������Ҫ���� ��������ֵ��־����Ϊ���һ�ֽ�����.
 */
typedef int (*HAL_UART_ISRCallBack)(unsigned char* param) ; /**< �жϷ���ص�����        */   

typedef struct hal_uart_isrcallback HAL_UART_ISRCALLBACK_t;  /**< �жϷ���ص������ṹ��  */

/**
 * \struct hal_uart_isrcallback
 * UART�ص������ṹ��.
 */
struct hal_uart_isrcallback
{
    HAL_UART_ID_e        id;         /**< UART id             */
    HAL_UART_ISRCallBack rxerrfun;   /**< �������ص�����     */
    HAL_UART_ISRCallBack rxfun;      /**< ���뻺�����ص�����   */
    HAL_UART_ISRCallBack txfun;      /**< ���ͻ���ջص�����   */
    HAL_UART_ISRCallBack txcmpfun;   /**< ������ص�����       */
}; 

/**
  * \}
  */
/*****************************************************************************    
 *                                                                           
 *                             �ӿں�������                                   
 *                                                                            
 ****************************************************************************/

/** \defgroup HAL_UART_if Ӳ�������(HAL)����ģ��(UART)�ӿ�
  * \{
  */

/**
 *****************************************************************************
 * \fn          int HAL_UART_Init(HAL_UART_ID_e id)
 * \brief       ��ʼ��UARTģ��,���ģ����ر�Ҫ�ĳ�ʼ������.
 * \note        ʹ��UARTǰ�������,����UART��Ĭ������(BSPʵ��).
 * \param[in]   id UARTid,������BSPʵ��,\ref HAL_UART_ID_e.
 * \retval      0 ��ʼ���ɹ�.
 * \retval      -1 ��ʼ��ʧ��.
 *****************************************************************************
 */
int HAL_UART_Init(HAL_UART_ID_e id);

/**
 *****************************************************************************
 * \fn          int HAL_UART_CheckSend(HAL_UART_ID_e id,unsigned char byte)
 * \brief       ��ѯ�ķ�ʽ����һ���ֽ�.
 * \note        ��������,ֱ��������Ż᷵��.  
 * \param[in]   id UARTid,������BSPʵ��,\ref HAL_UART_ID_e.
 * \param[in]   byte ��Ҫ���͵��ֽ�.
 * \retval      0 ���ͳɹ�.
 * \retval      ����ֵ ����ʧ��(ʵ����û�з�������������Բ��᷵������ֵ).
 *****************************************************************************
 */
int HAL_UART_CheckSend(HAL_UART_ID_e id,unsigned char byte);

/**
 *****************************************************************************
 * \fn          int HAL_UART_CheckRead(HAL_UART_ID_e id,unsigned char* byte);
 * \brief       ��ѯ�ķ�ʽ��һ���ֽ�.
 * \note        ������,ֱ�������ݲŻ᷵��.  
 * \param[in]   id UARTid,������BSPʵ��,\ref HAL_UART_ID_e.
 * \param[in]   byte �洢�������ֽ�.
 * \retval      0 ���ɹ�.
 * \retval      ����ֵ ��ʧ��(ʵ���϶��������ݻ��������Բ��᷵������ֵ).
 *****************************************************************************
 */
int HAL_UART_CheckRead(HAL_UART_ID_e id,unsigned char* byte);

/**
 *****************************************************************************
 * \fn          int HAL_UART_Send(HAL_UART_ID_e id,unsigned char byte)
 * \brief       ֱ�ӷ���һ���ֽ�.
 * \note        ֱ�ӷ���,ֱ����Ӳ�����ͻ���д�ֽ���������.  
 * \param[in]   id UARTid,������BSPʵ��,\ref HAL_UART_ID_e.
 * \param[in]   byte ��Ҫ���͵��ֽ�.
 * \retval      0 ���ͳɹ�.
 * \retval      ����ֵ ����ʧ��(ʵ���ϲ��᷵������ֵ).
 *****************************************************************************
 */
int HAL_UART_Send(HAL_UART_ID_e id,unsigned char byte);

/**
 *****************************************************************************
 * \fn          int HAL_UART_Read(HAL_UART_ID_e id,unsigned char* byte);
 * \brief       ֱ�Ӷ�һ���ֽ�.
 * \note        ֱ�Ӷ�,ֱ�Ӷ�Ӳ��������������.  
 * \param[in]   id UARTid,������BSPʵ��,\ref HAL_UART_ID_e.
 * \param[in]   byte �洢�������ֽ�.
 * \retval      0 ���ɹ�.
 * \retval      ����ֵ ��ʧ��(ʵ���ϲ��᷵������ֵ).
 *****************************************************************************
 */
int HAL_UART_Read(HAL_UART_ID_e id,unsigned char* byte);

/**
 *****************************************************************************
 * \fn          int HAL_UART_SetTxISRCallBack(HAL_UART_ID_e id,HAL_UART_ISRCallBack callbackfun)
 * \brief       ���÷��ͻ��������жϷ���ص�����.
 * \note        .  
 * \param[in]   id UARTid,������BSPʵ��,\ref HAL_UART_ID_e.
 * \param[in]   callbackfun �ص�����.
 * \retval      0 ���óɹ�.
 * \retval      ����ֵ ����ʧ��.
 *****************************************************************************
 */
int HAL_UART_SetTxISRCallBack(HAL_UART_ID_e id,HAL_UART_ISRCallBack callbackfun);

/**
 *****************************************************************************
 * \fn          int HAL_UART_SetTxCmpISRCallBack(HAL_UART_ID_e id,HAL_UART_ISRCallBack callbackfun)
 * \brief       ���÷������жϷ���ص�����.
 * \note        .  
 * \param[in]   id UARTid,������BSPʵ��,\ref HAL_UART_ID_e.
 * \param[in]   callbackfun �ص�����.
 * \retval      0 ���óɹ�.
 * \retval      ����ֵ ����ʧ��.
 *****************************************************************************
 */
int HAL_UART_SetTxCmpISRCallBack(HAL_UART_ID_e id,HAL_UART_ISRCallBack callbackfun);

/**
 *****************************************************************************
 * \fn          int HAL_UART_SetRxCallBack(HAL_UART_ID_e id,HAL_UART_ISRCallBack callbackfun)
 * \brief       ���ý��ջ������жϷ���ص�����.
 * \note        .  
 * \param[in]   id UARTid,������BSPʵ��,\ref HAL_UART_ID_e.
 * \param[in]   callbackfun �ص�����.
 * \retval      0 ���óɹ�.
 * \retval      ����ֵ ����ʧ��.
 *****************************************************************************
 */
int HAL_UART_SetRxCallBack(HAL_UART_ID_e id,HAL_UART_ISRCallBack callbackfun);

/**
 *****************************************************************************
 * \fn          int HAL_UART_SetRxErrCallBack(HAL_UART_ID_e id,HAL_UART_ISRCallBack callbackfun)
 * \brief       ���ý��մ����жϷ���ص�����.
 * \note        .  
 * \param[in]   id UARTid,������BSPʵ��,\ref HAL_UART_ID_e.
 * \param[in]   callbackfun �ص�����.
 * \retval      0 ���óɹ�.
 * \retval      ����ֵ ����ʧ��.
 *****************************************************************************
 */
int HAL_UART_SetRxErrCallBack(HAL_UART_ID_e id,HAL_UART_ISRCallBack callbackfun);

/**
 *****************************************************************************
 * \fn          int HAL_UART_SetCfg(HAL_UART_ID_e id,HAL_UART_CFG_t* cfg)
 * \brief       ����UART����.
 * \note        .  
 * \param[in]   id UARTid,������BSPʵ��,\ref HAL_UART_ID_e.
 * \param[in]   cfg ָ�����ò����ṹ�� \ref HAL_UART_CFG_t.
 * \retval      0 ���óɹ�.
 * \retval      ����ֵ ����ʧ��.
 *****************************************************************************
 */
int HAL_UART_SetCfg(HAL_UART_ID_e id,HAL_UART_CFG_t* cfg);

/**
 *****************************************************************************
 * \fn          int HAL_UART_EnableTx(HAL_UART_ID_e id,HAL_UART_TYPE_e type,HAL_UART_ENABLE_e enable)
 * \brief       ����ʹ��.
 * \note        .  
 * \param[in]   id UARTid,������BSPʵ��,\ref HAL_UART_ID_e.
 * \param[in]   type ����ʽ \ref HAL_UART_TYPE_e.
 * \param[in]   enable ʹ�ܻ��߽��� \ref HAL_UART_ENABLE_e.
 * \retval      0 ���óɹ�.
 * \retval      ����ֵ ����ʧ��.
 *****************************************************************************
 */
int HAL_UART_EnableTx(HAL_UART_ID_e id,HAL_UART_TYPE_e type,HAL_UART_ENABLE_e enable);

/**
 *****************************************************************************
 * \fn          int HAL_UART_EnableRx(HAL_UART_ID_e id,HAL_UART_TYPE_e type,HAL_UART_ENABLE_e enable)
 * \brief       ����ʹ��.
 * \note        .  
 * \param[in]   id UARTid,������BSPʵ��,\ref HAL_UART_ID_e.
 * \param[in]   type ����ʽ \ref HAL_UART_TYPE_e.
 * \param[in]   enable ʹ�ܻ��߽��� \ref HAL_UART_ENABLE_e.
 * \retval      0 ���óɹ�.
 * \retval      ����ֵ ����ʧ��.
 *****************************************************************************
 */
int HAL_UART_EnableRx(HAL_UART_ID_e id,HAL_UART_TYPE_e type,HAL_UART_ENABLE_e enable);

/**
 *****************************************************************************
 * \fn          int HAL_UART_EnableRxErr(HAL_UART_ID_e id,HAL_UART_TYPE_e type,HAL_UART_ENABLE_e enable)
 * \brief       ���մ����ж�ʹ��.
 * \note        .  
 * \param[in]   id UARTid,������BSPʵ��,\ref HAL_UART_ID_e.
 * \param[in]   type ����ʽ \ref HAL_UART_TYPE_e.
 * \param[in]   enable ʹ�ܻ��߽��� \ref HAL_UART_ENABLE_e.
 * \retval      0 ���óɹ�.
 * \retval      ����ֵ ����ʧ��.
 *****************************************************************************
 */
int HAL_UART_EnableRxErr(HAL_UART_ID_e id,HAL_UART_TYPE_e type,HAL_UART_ENABLE_e enable);

/**
 *****************************************************************************
 * \fn          int HAL_UART_DeInit(HAL_UART_ID_e id)
 * \brief       ���UARTģ��,�ر�ģ��.
 * \note        .
 * \param[in]   id UARTid,������BSPʵ��,\ref HAL_UART_ID_e.
 * \retval      0 �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int HAL_UART_DeInit(HAL_UART_ID_e id);

/**
  * \}
  */

/** \defgroup HAL_UART_samp Ӳ�������(HAL)����ģ��(UART)ʹ������
  * \{
  */

/*****************************************************************************    
 *                                                                           
 *                             ʹ������                                   
 *                                                                            
 ****************************************************************************/

/**
 *****************************************************************************
 * \fn          void hal_uart_samp(void)
 * \brief       ʹ������.
 * \par   �μ�  ��ʾ������ǩ�µġ�hal_uart_samp.c��
 *****************************************************************************
 */
void hal_uart_samp(void);

/**
 *\example hal_uart_samp.c
 * Ӳ�������(HAL)����ģ��(UART)ʹ������.\n
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

