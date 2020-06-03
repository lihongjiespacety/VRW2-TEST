/**
 *****************************************************************************        
 * \brief       ������(DRIVER)ʱ��ģ��(TIME)��ؽӿ�ʵ��.
 * \details     Copyright (c) 2017,��ɳ��ʤ��Ϣ�������޹�˾.
 *              All rights reserved.    
 * \file        driver_time.c 
 * \author      ����W18900
 * \version     1.0 
 * \date        2017-08-09
 * \note        ʹ��ǰ��ο�ע��.\n
 *              ����HALʵ��,����OSAPI,driver_iicrtc.\n
 * \since       ����      2017-8-9 18:18:12     1.0     �½� 
 * \par �޶���¼
 * - 2017-08-09 ��ʼ�汾
 * \par ��Դ˵��
 * - RAM:              
 * - ROM:
 *****************************************************************************
 */

#include "driver.h"
#include "osapi_freertos.h"

/*****************************************************************************    
 *                                                                           
 *                             �ڲ�����                                   
 *                                                                            
 ****************************************************************************/


static OSSemObj_p pSemTime=0;              /**<  ʱ�䴦���ź���ָ��    */
static time_t gSwTime=0;                   /**<  ȫ�����ʱ���¼      */   
static time_t gRunTime=0;                  /**<  ϵͳ����ʱ��          */   

/*****************************************************************************    
 *                                                                           
 *                             �ڲ��ӿں���ʵ��                                   
 *                                                                            
 ****************************************************************************/

/**
 *****************************************************************************
 * \fn          time_t gettime(void)
 * \brief       <time.h>�ײ�ӿ�ʵ��,����ʵ�ָú�����׼<time.h>����ʹ��.
 * \note        .
 *****************************************************************************
 */ 
time_t gettime(void)
{
    return gSwTime;
}


/*****************************************************************************    
 *                                                                           
 *                             ����ӿں���ʵ��                                   
 *                                                                            
 ****************************************************************************/

/**
 *****************************************************************************
 * \fn          time_t driver_time_mktime_r(struct tm * t)
 * \brief       ���ṹ��ʱ��struct tmת����time_tʱ���,�̰߳�ȫ�汾.
 * \param[in]   t  ָ����Ҫת����ʱ��ṹ��.
 * \return      time_t ����ת�����ʱ���,����0��ʾ����ʧ��. 
 * \note        .
 *****************************************************************************
 */ 
time_t driver_time_mktime_r(struct tm * t)
{
    if(OsSemPend(pSemTime, DRIVER_TIME_SEMTIMEOUT)==0) 
    {
        time_t ret = mktime(t);
        OsSemPost(pSemTime);
        return ret;
    } 
    else
    {    
        return 0;
    }
}

/**
 *****************************************************************************
 * \fn          int driver_time_sync(void)
 * \brief       ͬ��Ӳ��ʱ�䵽���ʱ��.
 * \retval      0 �����ɹ�
 * \retval      ����ֵ ����ʧ��
 * \note        .
 *****************************************************************************
 */ 
int driver_time_sync(void)
{
    struct tm t;
    int res;
    time_t temp;
    if(OsSemPend(pSemTime, DRIVER_TIME_SEMTIMEOUT)==0) 
    {
        res = -1;//driver_iicrtc_gettime(&t);  /*��ȡӲ��ʱ��*/
        OsSemPost(pSemTime);
    } 
    else
    {    
        return -1;
    }
    if(res == 0) 
    {
        temp =  driver_time_mktime_r(&t);  /*ת����ʱ���*/    
        if(temp != 0)
        {
            gSwTime = temp;
            return 0;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
}


/**
 *****************************************************************************
 * \fn          struct tm * driver_time_localtime_r(const time_t *pt, struct tm *ptm)
 * \brief       ��time_tʱ���ת���ɽṹ��ʱ��struct tm,�̰߳�ȫ�汾.
 * \param[in]   pt  ָ����Ҫת����ʱ���.
 * \param[out]  ptm ����ת�����,�������ṩ�洢�ռ�.
 * \return      struct tm* ����ָ��ת�������ָ��,�������0��ʾ����ʧ��.
 * \note        .
 *****************************************************************************
 */ 
struct tm * driver_time_localtime_r(const time_t *pt, struct tm *ptm)
{
    struct tm *ret;
    if(OsSemPend(pSemTime, DRIVER_TIME_SEMTIMEOUT)==0) 
    {
        ret = localtime(pt);
        if(ret != (void *)0)
        {
            *ptm = *ret;  /*ע������һ��Ҫ��ֵ����,��Ϊʵ����ptmָ�����һ��ȫ�ֱ���,�̲߳���ȫ,���Բ����ź�������*/
        }
        OsSemPost(pSemTime);
        return ptm;
    }
    else
    {
        return 0;
    }
}



/**
 *****************************************************************************
 * \fn          int driver_time_init(void)
 * \brief       ��ʼ��ʱ������.
 * \retval      0 �����ɹ�
 * \retval      ����ֵ ����ʧ��
 * \note        .
 *****************************************************************************
 */ 
int driver_time_init(void)
{
    //driver_iicrtc_init();   
    if(pSemTime == 0)
    {
        pSemTime = OsSemCreate("semtime", 1);
    }      
    return 0;//driver_time_sync();      /*��ʼ��ʱ����Ӳ��ʱ��*/	        
}


/**
 *****************************************************************************
 * \fn          void driver_time_sechandle(void)
 * \brief       ���ʱ����»ص�����,1���ӵ���һ��.
 * \note        .
 *****************************************************************************
 */ 
void driver_time_sechandle(void)
{
    gSwTime++;
    gRunTime++;
    if(gSwTime%DRIVER_TIME_SYNCPERIOD==0) 
    {  
        //driver_time_sync();   /*ͬ��ʱ��*/
    }
}

/**
 *****************************************************************************
 * \fn          int driver_time_settime(time_t t)
 * \brief       ������Ӳ��ʱ��.
 * \param[in]   t  Ҫ���õ�ʱ���.
 * \retval      0 �����ɹ�
 * \retval      ����ֵ ����ʧ��
 * \note        .
 *****************************************************************************
 */ 
int driver_time_settime(time_t t)
{
    int res=0;
    struct tm tmtm;
    struct tm *ptm = driver_time_localtime_r(&t, &tmtm);
    if(ptm != 0) 
    {
        if(OsSemPend(pSemTime, DRIVER_TIME_SEMTIMEOUT)==0) 
        {
            res = -1;//driver_iicrtc_settime(&tmtm);
            gSwTime = t;
            OsSemPost(pSemTime);
        } 
        else
        {    
            res = -1;
        }	
    } 
    else
    {
        res = -1;
    }
    return res;
}

/**
 *****************************************************************************
 * \fn          int driver_time_gettm(struct tm *p)
 * \brief       ��ȡ����ṹ��ʱ��.
 * \param[out]  p  �洢��ȡ��ʱ��ṹ��.
 * \retval      0 �����ɹ�
 * \retval      ����ֵ ����ʧ��
 * \note        .
 *****************************************************************************
 */ 
int driver_time_gettm(struct tm *p)
{
    time_t t = gettime();
    struct tm *ptm = driver_time_localtime_r(&t, p);
    if(ptm != 0)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

/**
 *****************************************************************************
 * \fn          int driver_time_settm(struct tm *p)
 * \brief       ������Ӳ���ṹ��ʱ��.
 * \param[out]  p  �洢��ȡ��ʱ��ṹ��.
 * \retval      0 �����ɹ�
 * \retval      ����ֵ ����ʧ��
 * \note        .
 *****************************************************************************
 */ 
int driver_time_settm(struct tm *p)
{
    time_t temp=0;
    int res=0;
    if(OsSemPend(pSemTime, DRIVER_TIME_SEMTIMEOUT)==0) 
    {
        res = -1;//driver_iicrtc_settime(p);
        OsSemPost(pSemTime);
    } 
    else
    {    
        res = -1;
    }	
    if(res == 0)
    {
        temp = driver_time_mktime_r(p);
        if(temp !=0)
        {
            gSwTime = temp;
            res =0;
        }
        else
        {
            res = -1;
        }
    }
    return res;
}

/**
 *****************************************************************************
 * \fn          time_t driver_time_gettime(void)
 * \brief       ��ȡ���ʱ���ʱ��.
 * \return      time_t �������ʱ���
 * \note        .
 *****************************************************************************
 */ 
time_t driver_time_gettime(void)
{
    return gettime(); 
}

/**
 *****************************************************************************
 * \fn          time_t driver_time_getruntime(void)
 * \brief       ��ȡϵͳ����ʱ��.
 * \return      time_t ����ϵͳ����ʱ��
 * \note        .
 *****************************************************************************
 */ 
time_t driver_time_getruntime(void)
{
    return gRunTime; 
}

/**
 *****************************************************************************
 * \fn          int driver_time_getrtctm(struct tm *p)
 * \brief       ��ȡӲ��RTC�ṹ��ʱ��.
 * \retval      0 �����ɹ�
 * \retval      ����ֵ ����ʧ��
 * \note        .
 *****************************************************************************
 */ 
int driver_time_getrtctm(struct tm *p)
{
    int res;
    if(OsSemPend(pSemTime, DRIVER_TIME_SEMTIMEOUT)==0) 
    {
        res = -1;//driver_iicrtc_gettime(p);
        OsSemPost(pSemTime);
        return res;
    } 
    else
    {    
        return -1;
    }	
}


