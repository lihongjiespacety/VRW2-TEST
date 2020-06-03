/**
 *****************************************************************************        
 * \brief       Ӳ�������(HAL)HALģ��(HAL)��ؽӿ�ʵ��.
 * \details     Copyright (c) 2017,��ɳ��ʤ��Ϣ�������޹�˾.
 *              All rights reserved.    
 * \file        hal.c  
 * \author      ����W18900
 * \version     1.0 
 * \date        2017-10-17
 * \note        ʹ��ǰ��ο�ע��.\n
 *              HALֻ����������,������Դ��BSPʵ��.\n
 * \since       ����      2017-10-17 10:56:52    1.0     �½� 
 * \par �޶���¼
 * - 2017-10-17 ��ʼ�汾
 * \par ��Դ˵��
 * - RAM:              
 * - ROM:
 *****************************************************************************
 */

#include "hal.h"
#include "bsp.h"

/**
 *****************************************************************************
 * \fn          int HAL_Init(void)
 * \brief       ��ʼ��HALģ��,������Ҫ��BSP��ʼ��.
 * \note        ϵͳ���øú��������ײ�ĳ�ʼ��.
 * \retval      0  ��ʼ���ɹ�.
 * \retval      -1 ��ʼ��ʧ��.
 *****************************************************************************
 */
int HAL_Init(void)
{
    int res = 0;
    /*1.BSP��ʼ��*/
    res = BSP_Init();
    /*2.HAL��ʼ��*/
    return res;
}

/**
 *****************************************************************************
 * \fn          unsigned int HAL_GetBSPVersion(void)
 * \brief       ��ȡHAL������BSP�汾.
 * \note        .  
 * \return      unsigned int ���ذ汾��Ϣ \ref HAL_VERSION 
 *****************************************************************************
 */
unsigned int HAL_GetBSPVersion(void)
{
    return BSP_GetVersion();
}

/**
 *****************************************************************************
 * \fn          unsigned int HAL_GetHALVersion(void)
 * \brief       ��ȡHAL�汾.
 * \note        .  
 * \return      unsigned int ���ذ汾��Ϣ \ref HAL_VERSION 
 *****************************************************************************
 */
unsigned int HAL_GetHALVersion(void)
{
    return HAL_VERSION;
}

/**
 *****************************************************************************
 * \fn          int HAL_DeInit(void)
 * \brief       ���HALģ���ʼ��.
 * \note        .
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int HAL_DeInit(void)
{
    int res = 0;
    res = BSP_DeInit();
    return res;
}