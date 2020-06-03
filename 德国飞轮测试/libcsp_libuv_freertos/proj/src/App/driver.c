/**
 *****************************************************************************        
 * \brief       ������(DRIVER)DRIVERģ��(DRIVER)��ؽӿ�ʵ��.
 * \details     Copyright (c) 2017,��ɳ��ʤ��Ϣ�������޹�˾.
 *              All rights reserved.    
 * \file        driver.c  
 * \author      ����W18900
 * \version     1.0 
 * \date        2017-10-20
 * \note        ʹ��ǰ��ο�ע��.\n
 *              ������HAL��OSʵ��.\n
 * \since       ����      2017-10-20 16:35:35    1.0     �½� 
 * \par �޶���¼
 * - 2017-10-20 ��ʼ�汾
 * \par ��Դ˵��
 * - RAM:              
 * - ROM:
 *****************************************************************************
 */
#include "driver.h"


#define DRIVER_VERSION 0x10    /**< DRIVER��汾���ݸ���λ�����汾��,����λ����С�汾�š�����0x10����汾1.0 */

/**
 *****************************************************************************
 * \fn          int driver_init(void)
 * \brief       ��ʼ��DRIVER��.
 * \note        ϵͳ���øú�������������ĳ�ʼ��.
 * \retval      0  ��ʼ���ɹ�.
 * \retval      -1 ��ʼ��ʧ��.
 *****************************************************************************
 */
int driver_init(void)
{
  /*driver_uart��driver_iruart���ڴ˳�ʼ��,ʹ��ʱ�س�ʼ��*/
  //driver_flashif_init();
  //driver_iap_init(); 
  //driver_adc_init(); 
  //driver_gpio_init();
  //driver_led_init();
  driver_time_init();
  //driver_esam_init();  esamһ�㲻�����в�Ʒ����,����ģ��϶���,��������Ҫʱ�ٵ��ó�ʼ��,���ڴ˵���
  return 0;
}

/**
 *****************************************************************************
 * \fn          unsigned int driver_getversion(void)
 * \brief       ��ȡHAL������BSP�汾.
 * \note        .  
 * \return      unsigned int ���ذ汾��Ϣ \ref unsigned 
 *****************************************************************************
 */
unsigned int driver_getversion(void)
{
    return DRIVER_VERSION;
}