/**
 *****************************************************************************        
 * \brief       ��׼��������ײ�ӿ��ض���.
 * \details     Copyright (c) 2017,��ɳ��ʤ��Ϣ�������޹�˾.
 *              All rights reserved.    
 * \file        retarget.h 
 * \author      ����W18900
 * \version     1.0 
 * \date        2017-07-26
 * \note        ʹ��ǰ��ο�ע��.\n
 *              ʵ�ֹ�printf scanf���õĵײ�ӿ�.\n
 * \since       ����      2017-7-26 09:58:25     1.0     �½� 
 * \par �޶���¼
 * - 2017-07-26 ��ʼ�汾
 * \par ��Դ˵��
 * - RAM:              
 * - ROM:
 *****************************************************************************
 */

#ifndef __RETARGET_H
#define __RETARGET_H

#ifdef __cplusplus
 extern "C" {
#endif
     
#include "hal.h"
/** \addtogroup PLATFORM PLATFORM
 *  \{
 */
     
/** \addtogroup PLATFORM_RETARGET PLATFORM_RETARGET
 *  \{
 */ 
     
/*****************************************************************************    
 *                                                                           
 *                             ���ݽṹ����                                   
 *                                                                            
 ****************************************************************************/

/** \defgroup PLATFORM_RETARGET_data PLATFORM_RETARGET_data
  * \{
  */
    
 /**
  * \}
  */
   
   
/*****************************************************************************    
 *                                                                           
 *                             �ӿں�������                                   
 *                                                                            
 ****************************************************************************/

/** \defgroup PLATFORM_RETARGET_if PLATFORM_RETARGET_if
  * \{
  */
     
/**
 *****************************************************************************
 * \fn          void setstdiouart(HAL_UART_ID_e id)
 * \brief       ����stdioʹ�õĴ���.
 * \note        Ĭ�϶˿���Ч,��Ҫ���øú���ȥӳ��ʵ�ʶ�Ӧ�Ķ˿�. 
 * \param[in]   id stdioӳ�䵽�������ں�,�������Ҫʹ��stdio����,�˿�����ΪHAL_UART_MAX
 *****************************************************************************
 */
void setstdiouart(HAL_UART_ID_e id);

/**
 *****************************************************************************
 * \fn          HAL_UART_ID_e getstdiouart(void)
 * \brief       ��ȡstdioʹ�õĴ���.
 * \note        Ĭ�϶˿���Ч,��Ҫ����setstdiouartȥӳ��ʵ�ʶ�Ӧ�Ķ˿�. 
 * \return      ����ʹ�õĴ���ID \ref HAL_UART_ID_e
 *****************************************************************************
 */
HAL_UART_ID_e getstdiouart(void);

/**
  * \}
  */

/** \defgroup PLATFORM_RETARGET_samp PLATFORM_RETARGET_samp
  * \{
  */

/*****************************************************************************    
 *                                                                           
 *                             ʹ������                                   
 *                                                                            
 ****************************************************************************/

/**
 *****************************************************************************
 * \fn          void platform_retarget_samp(void)
 * \brief       ʹ������.
 * \par   �μ�  platform_retarget_samp.c��
 *****************************************************************************
 */
void platform_retarget_samp(void);

/**
 *\example platform_retarget_samp.c
 * ƽ̨��(PLATFORM)��׼��������ض���ģ��(RETARGET)ʹ������.\n
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