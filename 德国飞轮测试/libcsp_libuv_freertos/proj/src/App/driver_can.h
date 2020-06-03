/**
********************************************************************************        
* \brief       ������(DRIVER)CANģ��(CAN)��ؽӿ�����.
* \details     Copyright (c) 2019,����/��ɳ�н��Ƶ������޹�˾.
*              All rights reserved.    
* \file        driver_can.h 
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


#ifndef DRIVER_CAN_H
#define DRIVER_CAN_H

#ifdef __cplusplus
extern "C" {
#endif
  
  
#include <stdint.h>
  /** \addtogroup DRIVER ������(DRIVER)
  *  \{
  */
  
  /** \addtogroup DRIVER_CAN ������(DRIVER)CANģ��(CAN)
  *  \{
  */
  /*******************************************************************************    
  *                                                                             *
  *                             ���ݽṹ����                                    *
  *                                                                             *
  ******************************************************************************/
  
  /** \defgroup DRIVER_CAN_data ������(DRIVER)CANģ��(CAN)���ݽṹ 
  * \{
  */
  
#define DRIVER_CAN_DELAY_CHECK (uint32_t)1  /**< CAN���ݲ�ѯ���ʱ�� */
#define DRIVER_CAN_RXBUFFSIZE (uint16_t)(PBUF_MTU/2)  /**< CAN���ջ�������С */
#define DRIVER_CAN_TXBUFFSIZE (uint16_t)(PBUF_MTU/2)  /**< CAN���ͻ�������С */
    
#define CAN_DATA_IDE_BIT 1
#define CAN_DATA_RTR_BIT 0
    
 /**
 * \struct driver_can_t
 * CAN�������ýṹ��.
 */
typedef struct 
{
    uint8_t type;              /**< bit1 = ide  bit0 = rtr      */ 
    uint8_t len;               /**< ��Ч���ݳ���                */ 
    uint8_t buff[8];           /**< ����                        */ 
    uint32_t id;               /**< id */
}driver_can_data_t;
  


/**
 * \struct driver_can_t
 * CAN�������ýṹ��.
 */
typedef struct 
{
    uint16_t rx_in;              /**< ���ջ�����д��ָ��       */ 
    uint16_t rx_out;             /**< ���ջ���������ָ��       */ 
    uint16_t rx_len;             /**< ���ջ�������Ч���ݴ�С   */ 
    uint16_t tx_in;              /**< ���ͻ�����д��ָ��       */ 
    uint16_t tx_out;             /**< ���ͻ���������ָ��      */ 
    uint16_t tx_len;             /**< ���ͻ�������Ч���ݴ�С   */ 
    uint16_t rxbuf_len;          /**< ���ջ�������С           */   
    uint16_t txbuf_len;          /**< ���ͻ�������С          */ 
    driver_can_data_t *rx_buf;   /**< ���ջ�����              */ 
    driver_can_data_t *tx_buf;   /**< ���ͻ�����              */ 
}driver_can_t;

  
  /**
  * \}
  */
  /*******************************************************************************    
  *                                                                           
  *                             �ӿں�������                                   
  *                                                                            
  ******************************************************************************/
  
  /** \defgroup DRIVER_CAN_if ������(DRIVER)CANģ��(CAN)�ӿ�
  * \{
  */
 
/**
********************************************************************************
* \fn          void driver_can_init(uint32_t baudrate)
* \brief       ��ʼ��CAN.
* \param[in]   baudrate ������.
* \note        . 
********************************************************************************
*/
void driver_can_init(uint32_t baudrate);
    

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
void driver_can_filter(uint8_t ch,uint8_t mode,uint32_t id,uint32_t mask);

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
int8_t driver_can_send(driver_can_data_t data, uint32_t timeout);

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
int8_t driver_can_recv(driver_can_data_t* data, uint32_t timeout);

    
/**
 *****************************************************************************
 * \fn          int8_t driver_can_sendloop(void)
 * \brief       ���������Ϣ�����п�,���з�������,�򽫷��ͻ���������д�뷢����Ϣ����.
 * \note        Ӧ�����ڵ��øú������з��ʹ���. 
 * \retval      0 �з���
 * \retval      1 �޷���
 *****************************************************************************
 */
int8_t driver_can_sendloop(void);



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


