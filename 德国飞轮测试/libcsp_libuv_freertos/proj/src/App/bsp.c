#include "iorx113.h"
#include "hal.h"
#include "bsp.h"

#define BSP_VERSION 0x11



int BSP_GetVersion(void)
{
    return BSP_VERSION;
}

int BSP_Init(void)
{
    while(1)
    {
        if(BSP_CGC_Init()!=0)
        {
            /*ʹ���ⲿ�����ʼ��ʧ��*/
            if(BSP_CGC_SwitchToHOCO()!=0)
            {
                /*�л����ڲ�����ʱ��*/
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    BSP_LPC_Init();
    BSP_IOP_Init();
    BSP_SPI_Init(HAL_SPI_0);
    BSP_IWDT_Init();
    BSP_CMP_Init();
    return 0;
}

/**
 *****************************************************************************
 * \fn          int BSP_DeInit(void)
 * \brief       ���BSP���ʼ��.
 * \note        .
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int BSP_DeInit(void)
{
    return 0;
}