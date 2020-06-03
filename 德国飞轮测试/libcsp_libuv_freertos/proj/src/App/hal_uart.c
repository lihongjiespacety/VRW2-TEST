/**
 *****************************************************************************        
 * \brief       Ӳ�������(HAL)����(UART)��ؽӿ�ʵ��.
 * \details     Copyright (c) 2017,��ɳ��ʤ��Ϣ�������޹�˾.
 *              All rights reserved.    
 * \file        hal_uart.c
 * \author      ����W18900
 * \version     1.0 
 * \date        2017-07-24
 * \note        ʹ��ǰ��ο�ע��.\n
 *              ����BSPʵ��.\n
 * \since       ����      2017-7-24 13:52:37     1.0     �½� 
 * \par �޶���¼
 * - 2017-07-24 ��ʼ�汾
 * \par ��Դ˵��
 * - RAM:              
 * - ROM:
 *****************************************************************************
 */
#include "hal.h"
#include "bsp.h"

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
int HAL_UART_Init(HAL_UART_ID_e id)
{
    return BSP_SCI_Init(id);
}

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
int HAL_UART_CheckSend(HAL_UART_ID_e id,unsigned char byte)
{
    return BSP_SCI_CheckSend(id,byte);
}

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
int HAL_UART_CheckRead(HAL_UART_ID_e id,unsigned char* byte)
{
    return BSP_SCI_CheckRead(id,byte);
}

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
int HAL_UART_Send(HAL_UART_ID_e id,unsigned char byte)
{
    return BSP_SCI_Send(id,byte);
}

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
int HAL_UART_Read(HAL_UART_ID_e id,unsigned char* byte)
{
    return BSP_SCI_Read(id,byte);
}

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
int HAL_UART_SetTxISRCallBack(HAL_UART_ID_e id,HAL_UART_ISRCallBack callbackfun)
{
    return BSP_SCI_SetTxCallBack(id,callbackfun);
}

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
int HAL_UART_SetTxCmpISRCallBack(HAL_UART_ID_e id,HAL_UART_ISRCallBack callbackfun)
{
    return BSP_SCI_SetTxCompleteCallBack(id,callbackfun);
}

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
int HAL_UART_SetRxCallBack(HAL_UART_ID_e id,HAL_UART_ISRCallBack callbackfun)
{
    return BSP_SCI_SetRxCallBack(id,callbackfun);
}

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
int HAL_UART_SetRxErrCallBack(HAL_UART_ID_e id,HAL_UART_ISRCallBack callbackfun)
{
    return BSP_SCI_SetRxErrCallBack(id,callbackfun);
}

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
int HAL_UART_SetCfg(HAL_UART_ID_e id,HAL_UART_CFG_t* cfg)
{
    return BSP_SCI_SetCfg(id,cfg);
}

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
int HAL_UART_EnableTx(HAL_UART_ID_e id,HAL_UART_TYPE_e type,HAL_UART_ENABLE_e enable)
{
    return BSP_SCI_EnableTx(id,type,enable);
}

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
int HAL_UART_EnableRx(HAL_UART_ID_e id,HAL_UART_TYPE_e type,HAL_UART_ENABLE_e enable)
{
    return BSP_SCI_EnableRx(id,type,enable);
}

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
int HAL_UART_EnableRxErr(HAL_UART_ID_e id,HAL_UART_TYPE_e type,HAL_UART_ENABLE_e enable)
{
    return BSP_SCI_EnableRxErr(id,type,enable);
}

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
int HAL_UART_DeInit(HAL_UART_ID_e id)
{
    return BSP_SCI_DeInit(id);
}