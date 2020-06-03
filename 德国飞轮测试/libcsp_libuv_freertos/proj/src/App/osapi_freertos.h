/**
 *****************************************************************************        
 * \brief       ����ϵͳӦ�ó���ӿ�(OSAPI)������ݽṹ�ͽӿ�����.
 * \details     Copyright (c) 2017,��ɳ��ʤ��Ϣ�������޹�˾.
 *              All rights reserved.    
 * \file        osapi.h  
 * \author      ����W18900
 * \version     1.0 
 * \date        2017-08-23
 * \note        ʹ��ǰ��ο�ע��.\n
 *              �Գ���OS�ӿں������з�װ.\n
 * \since       ����      2017-8-23 09:49:20     1.0     �½� 
 * \par �޶���¼
 * - 2017-08-23 ��ʼ�汾
 * \par ��Դ˵��
 * - RAM:              
 * - ROM:
 *****************************************************************************
 */

#ifndef __OSAPI_H
#define __OSAPI_H

#ifdef __cplusplus
 extern "C" {
#endif
     
#include "FreeRTOS.h"
#include "semphr.h"
#include "timers.h"
#include "task.h"
#include <stdarg.h>
#include "csp.h"
/** \addtogroup OSAPI ����ϵͳ��(OSAPI)
 *  \{
 */    
     
/*****************************************************************************    
 *                                                                           *
 *                             ���ݽṹ����                                  *
 *                                                                          *
 ****************************************************************************/
   

/** \defgroup OSAPI_data ����ϵͳ��(OSAPI)������ݽṹ
  * \{
  */
     
#define TSKNAMELEN 16                       /**< �������ֳ���           */
#define TICKS_PER_SEC configTICK_RATE_HZ    /**< ϵͳ�δ�Ƶ��           */
     
   typedef struct 
   {
     StaticQueue_t queue;
     void* data;
   }OSMSGQObj,*OSMSGQObj_p;  /*�̳���StaticQueue_t ����data����洢��������*/
   
typedef StaticTask_t   TSK_TCB;            /**< ostcb��������           */
typedef StackType_t    STK_TYPE;           /**< ��ջ��������           */
typedef void (*TASK_ENTRY_fp)(void *arg); /**< ָ����������ڵ�ָ��  */
typedef struct tsk_cfg TSK_CFG_t;         /**< ���񴴽������ṹ��      */
typedef struct QueueDefinition     OSSemObj;    /**< �ź�������              */
typedef QueueHandle_t    OSSemObj_p;  /**< �ź�������ָ������      */
typedef struct QueueDefinition     OSMutexObj;  /**< ����������             */
typedef QueueHandle_t *   OSMutexObj_p;/**< ����������ָ������      */
typedef StaticTimer_t         OSTimerObj;  /**< ��ʱ������             */
typedef StaticTimer_t *       OSTimerObj_p;/**< ��ʱ������ָ������      */
//typedef StaticQueue_t         OSMSGQObj;   /**< ��Ϣ���ж���             */
//typedef StaticQueue_t *       OSMSGQObj_p; /**< ��Ϣ���ж���ָ������      */


typedef void(*OSAPI_TIMER_CALLBACK)(void*,void*);   /**< ��ʱ���ص�����  */   
typedef enum  osapi_timer_type OSAPI_TIMER_TYPE_e;   /**< ��ʱ������ʽö�ٶ���  */      
/**
 * \enum osapi_timer_type
 * ��ʱ������ʽö�ٶ���.
 */
enum osapi_timer_type
{
    OSAPI_TIMIER_SINGLE=0,      /**< TIMER ���η�ʽ */ 
    OSAPI_TIMIER_CONTINU=1,     /**< TIMER ������ʽ */ 
};


/**
 * \enum osapi_debug_e
 * ��������ȼ�.
 */
typedef enum 
{
    OSAPI_DEBUG_OFF =0,      /**< */ 
    OSAPI_DEBUG_INFO=1,     /**<  */ 
    OSAPI_DEBUG_DEBUG=2,
    OSAPI_DEBUG_WARM=3,
    OSAPI_DEBUG_ERR=4,
}osapi_debug_e;


/**
 * \struct tsk_cfg
 * ���񴴽������ṹ�嶨��.
 * ���ڴ�������񴴽�����.
 */
struct tsk_cfg
{
    unsigned int   id;         /**< ����id         */
    unsigned int   pri;        /**< �������ȼ�     */
    STK_TYPE*       tskbase;    /**< �����ջ��ַ   */
    unsigned int   tsksize;    /**< �����ջ��С   */
    TASK_ENTRY_fp   taskp;      /**< �������       */
    unsigned int   robin;      /**< ʱ��Ƭ       */
    void*           arg;        /**< ������ڲ���   */
    unsigned int   opt;        /**< ��ѡ����     */
    char name[TSKNAMELEN];      /**< ��������       */
    StaticTask_t*   tcb;        /**< ָ��������ƿ�  */
}; 

typedef struct 
{
    unsigned int addr;
	unsigned int len;
}OSAPI_MSGQ_FORM;
/**
  * \}
  */
/*****************************************************************************    
 *                                                                           
 *                             �ӿں�������                                   
 *                                                                            
 ****************************************************************************/

/** \defgroup OSAPI_if ����ϵͳ��(OSAPI)��ؽӿ�
  * \{
  */

#define OSCriticalAlloc()         /**< ׼�������ٽ��       */
#define OSCriticalEnter() taskENTER_CRITICAL()    /**< �����ٽ��           */
#define OSCriticalExit()  taskEXIT_CRITICAL()	  /**< �˳��ٽ��           */

/**
 *****************************************************************************
 * \fn          int OsMemcmp(void *pDst, void *pSrc, unsigned int nNumBytes)
 * \brief       �ڴ�Ƚ�.
 * \param[in]   pDst Ŀ�ĵ�ַ.
 * \param[in]   pSrc Դ��ַ.
 * \param[in]   nNumBytes �Ƚ��ֽ���.
 * \note        .  
 * \retval      0 ���
 * \retval      >0 Ŀ�Ĵ���Դ
 * \retval      <0 Ŀ��С��Դ
 *****************************************************************************
 */
int OsMemcmp(void *pDst, void *pSrc, unsigned int nNumBytes);


/**
 *****************************************************************************
 * \fn          void OsMemcpy(void *pDst, void *pSrc, unsigned int nNumBytes)
 * \brief       �ڴ渴��.
 * \param[in]   pDst Ŀ�ĵ�ַ.
 * \param[in]   pSrc Դ��ַ.
 * \param[in]   nNumBytes ���Ƶ��ֽ���.
 * \note        .  
 *****************************************************************************
 */
void OsMemcpy(void *pDst, void *pSrc, unsigned int nNumBytes);

/**
 *****************************************************************************
 * \fn          void OsMemset(void *pMem, unsigned char iChar, unsigned int nNumBytes)
 * \brief       ����ڴ�.
 * \param[in]   pMem ��Ҫ���ĵ�ַ.
 * \param[in]   iChar ��Ҫ����ֵ.
 * \param[in]   nNumBytes ��Ҫ�����ֽ���.
 * \note        .  
 *****************************************************************************
 */
void OsMemset(void *pMem, unsigned char iChar, unsigned int nNumBytes);

/**
 *****************************************************************************
 * \fn          void* OsMalloc(unsigned int nNumBytes)
 * \brief       ���䶯̬�ڴ�.
 * \param[in]   nNumBytes ��Ҫ��ȡ���ֽ���.
 * \note        .  
 * \return      ָ���ȡ���ڴ� ����0�����ȡʧ��
 *****************************************************************************
 */
void* OsMalloc(unsigned int nNumBytes);

/**
 *****************************************************************************
 * \fn          void OsFree(void *pMem)
 * \brief       �ͷŶ�̬�ڴ�.
 * \note        .  
 * \param[in]   pMem ָ����Ҫ�ͷŵ��ڴ�.
 *****************************************************************************
 */
void OsFree(void *pMem);

/**
 *****************************************************************************
 * \fn          int OsInit()
 * \brief       ��ʼ��ϵͳ.
 * \note        .  
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int OsInit();

/**
 *****************************************************************************
 * \fn          int OsInit()
 * \brief       ����ϵͳ.
 * \note        .  
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int OsStart();
/**
 *****************************************************************************
 * \fn          int OsTaskCreate(TSK_CFG_t* cfg)
 * \brief       ��������.
 * \note        .  
 * \param[in]   cfg ָ����������ṹ���ָ�� \ref TSK_CFG_t.
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int OsTaskCreate(TSK_CFG_t* cfg);

/**
 *****************************************************************************
 * \fn          int OsTimeDelay(unsigned int milsec);
 * \brief       ������ʱ.
 * \note        .  
 * \param[in]   milsec ��ʱʱ�䵥λmS.
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int OsTimeDelay( unsigned int milsec );


/**
 *****************************************************************************
 * \fn          unsigned int OsTimeGetTick(void)
 * \brief       ��ȡϵͳ�δ������.
 * \note        .  
 * \retval      unsigned int ���صδ������ֵ
 *****************************************************************************
 */
unsigned int OsTimeGetTick(void);

/**
 *****************************************************************************
 * \fn          int OsInitInFirstTask(void);
 * \brief       ��һ������������Ӧ�ĳ�ʼ������.
 * \note        ��ʼ��ϵͳʱ��,ͳ�������.  
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int OsInitInFirstTask(void);

/**
 *****************************************************************************
 * \fn          unsigned int OsTaskGetId(void)
 * \brief       ��ȡ��ǰ����ID.
 * \note        . 
 * \return      unsigned int ���ص�ǰ�����ID.
 *****************************************************************************
 */
unsigned int OsTaskGetId(void);

/**
 *****************************************************************************
 * \fn          unsigned int OsTaskGetSwCount(unsigned char id)
 * \brief       ��ȡָ��ID����ĵ��ȴ���.
 * \param[in]   id ����ID.
 * \note        . 
 * \return      unsigned int ��������ĵ��ȴ���.
 *****************************************************************************
 */
unsigned int OsTaskGetSwCount(unsigned int id);

/**
 *****************************************************************************
 * \fn          void OsSetTaskKillReq(unsigned char id)
 * \brief       ����ɾ����������.
 * \param[in]   id ����id��,���ܳ���31.
 * \note        .  
 *****************************************************************************
 */
void OsSetTaskKillReq(unsigned char id);

/**
 *****************************************************************************
 * \fn          int OsGetTaskKillReq(unsigned char id)
 * \brief       ��ȡɾ����������.
 * \param[in]   id ����id��,���ܳ���31.
 * \note        . 
 * \retval      0  ������.
 * \retval      1  ������.
 *****************************************************************************
 */
int OsGetTaskKillReq(unsigned char id);

/**
 *****************************************************************************
 * \fn          int OsTaskKillSelf(void)
 * \brief       ����ɾ���Լ�.
 * \note        .  
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int OsTaskKillSelf(void);

/**
 *****************************************************************************
 * \fn          int OsSuspendTask(unsigned int id)
 * \brief       ����ָ��ID������.
 * \param[in]   id ����ID.
 * \note        .  
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int OsSuspendTask(unsigned int id);

/**
 *****************************************************************************
 * \fn          int OsResumeTask(unsigned int id)
 * \brief       �ָ�ָ��ID������.
 * \param[in]   id ����ID.
 * \note        .  
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int OsResumeTask(unsigned int id);

/**
 *****************************************************************************
 * \fn          OSSemObj* OsSemCreate(unsigned char *name, unsigned char count)
 * \brief       �����ź���.
 * \param[in]   name �ź������� ���봫��ȫ�����ݻ���const����.
 * \param[in]   count �ź�����ʼֵ.
 * \note        �ź���ʵ����malloc����.  
 * \return      OSSemObj*  ���ش������ź���ָ��,���Ϊ0��ʾ����ʧ��.
 *****************************************************************************
 */
OSSemObj_p OsSemCreate(unsigned char *name, unsigned int count);

/**
 *****************************************************************************
 * \fn          int OsSemPend(OSSemObj* psem, unsigned int timeout )
 * \brief       �ȴ��ź���.
 * \param[in]   psem �ź���ָ�� �������Ѿ��������ź���.
 * \param[in]   timeout �ȴ���ʱʱ�䵥λmS.
 * \note        .  
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int OsSemPend(OSSemObj_p psem, unsigned int timeout);

/**
 *****************************************************************************
 * \fn          int OsSemPost(OSSemObj* psem )
 * \brief       �����ź���.
 * \param[in]   psem �ź���ָ�� �������Ѿ��������ź���.
 * \note        .  
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int OsSemPost(OSSemObj_p psem );

/**
 *****************************************************************************
 * \fn          int OsSemDel(OSSemObj* psem )
 * \brief       ɾ���ź���.
 * \param[in]   psem �ź���ָ�� �������Ѿ��������ź���.
 * \note        .  
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int OsSemDel(OSSemObj_p psem );

/**
 *****************************************************************************
 * \fn          OSMutexObj_p OsMutexCreate(char *name)
 * \brief       ����������.
 * \param[in]   name ���������� ���봫��ȫ�����ݻ���const����.
 * \note        ��������ʵ����malloc����.  
 * \return      OSMutexObj_p  ���ش����Ļ�����ָ��,���Ϊ0��ʾ����ʧ��.
 *****************************************************************************
 */
OSMutexObj_p OsMutexCreate(char *name);

/**
 *****************************************************************************
 * \fn          int OsMutexPend(OSMutexObj_p pmutex, unsigned int timeout)
 * \brief       �ȴ�������.
 * \param[in]   pmutex ������ָ�� �������Ѿ������Ļ�����.
 * \param[in]   timeout �ȴ���ʱʱ�䵥λmS.
 * \note        .  
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int OsMutexPend(OSMutexObj_p pmutex, unsigned int timeout);

/**
 *****************************************************************************
 * \fn          int OsMutexPost(OSMutexObj_p pmutex)
 * \brief       ���ͻ�����.
 * \param[in]   pmutex ������ָ�� �������Ѿ������Ļ�����.
 * \note        .  
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int OsMutexPost(OSMutexObj_p pmutex);

/**
 *****************************************************************************
 * \fn          int OsMutexDel(OSMutexObj_p pmutex)
 * \brief       ɾ��������.
 * \param[in]   pmutex ��������ָ�� �������Ѿ������Ļ�����.
 * \note        .  
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int OsMutexDel(OSMutexObj_p pmutex);

/**
 *****************************************************************************
 * \fn          OSMSGQObj_p OsMsgQCreate(char *name,unsigned int size)
 * \brief       ������Ϣ����.
 * \param[in]   name ��Ϣ�������� ���봫��ȫ�����ݻ���const����.
 * \param[in]   size ��Ϣ���������Ϣ����.
 * \note        ��Ϣ����ʵ����malloc����.  
 * \return      OSMSGQObj_p  ���ش�������Ϣ����ָ��,���Ϊ0��ʾ����ʧ��.
 *****************************************************************************
 */
OSMSGQObj_p OsMsgQCreate(char *name,unsigned int size);

/**
 *****************************************************************************
 * \fn          int OsMsgQRcv(OSMSGQObj_p pMsgQ, unsigned char *buffer, unsigned int bufsize,unsigned int* size,unsigned int timeout)
 * \brief       ����Ϣ���н�����Ϣ.
 * \param[in]   pMsgQ ��Ϣ����ָ�� ����ָ���Ѿ���������Ϣ����.
 * \param[in]   buffer ���ջ�����.
 * \param[in]   bufsize ���ջ�������С.
 * \param[in]   timeout ��ʱʱ�䵥λmS.
 * \param[out]  size ʵ�ʽ��յ���Ϣ���ݴ�С.
 * \note        ��Ϣ���д洢��malloc����.  
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */ 
int OsMsgQRcv(OSMSGQObj_p pMsgQ, unsigned char *buffer, unsigned int bufsize,unsigned int* size,unsigned int timeout);


/**
 *****************************************************************************
 * \fn          int OsMsgQSnd(OSMSGQObj_p pMsgQ, unsigned char *buffer, unsigned size)
 * \brief       ����Ϣ���з�����Ϣ.
 * \param[in]   pMsgQ ��Ϣ����ָ�� ����ָ���Ѿ���������Ϣ����.
 * \param[in]   buffer ��Ϣ����.
 * \param[in]   size   ��Ϣ���ݵĳ���.
 * \note        ��Ϣ���д洢��malloc����.  
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */ 
int OsMsgQSnd(OSMSGQObj_p pMsgQ, unsigned char *buffer, unsigned size);

   /**
 *****************************************************************************
 * \fn          int OsMsgQDel(OSMSGQObj_p pMsgQ)
 * \brief       ɾ����Ϣ����.
 * \param[in]   pMsgQ ��Ϣ����ָ�� ����ָ���Ѿ���������Ϣ����.
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */ 
int OsMsgQDel(OSMSGQObj_p pMsgQ);
/**
 *****************************************************************************
 * \fn          OSTimerObj_p OsTimerCreate(unsigned int time,OSAPI_TIMER_TYPE_e mode,OSAPI_TIMER_CALLBACK callback,char *name)
 * \brief       ������ʱ��.
 * \param[in]   time ��ʱ������
 * \param[in]   mode ��ʱ��ģʽ \ref OSAPI_TIMER_TYPE_e.
 * \param[in]   callback ��ʱ���ص����� \ref OSAPI_TIMER_CALLBACK.
 * \param[in]   name ��ʱ������.
 * \note        ��ʱ��ʵ����malloc����.  
 * \return      OSMutexObj_p  ���ش����Ļ�����ָ��,���Ϊ0��ʾ����ʧ��.
 *****************************************************************************
 */
OSTimerObj_p OsTimerCreate(unsigned int time,OSAPI_TIMER_TYPE_e mode,OSAPI_TIMER_CALLBACK callback,char *name);

/**
 *****************************************************************************
 * \fn          int OsTimerStart(OSTimerObj_p ptimer)
 * \brief       ������ʱ��.
 * \param[in]   ptimer ��ʱ��ָ�� �������Ѿ������Ķ�ʱ��.
 * \note        .  
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int OsTimerStart(OSTimerObj_p ptimer);

/**
 *****************************************************************************
 * \fn          int OsTimerStop(OSTimerObj_p ptimer)
 * \brief       ֹͣ��ʱ��.
 * \param[in]   ptimer ��ʱ��ָ�� �������Ѿ������Ķ�ʱ��.
 * \note        .  
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int OsTimerStop(OSTimerObj_p ptimer);

/**
 *****************************************************************************
 * \fn          int OsTimerDelete(OSTimerObj_p ptimer)
 * \brief       ɾ����ʱ��.
 * \param[in]   ptimer ��ʱ��ָ�� �������Ѿ������Ķ�ʱ��.
 * \note        .  
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int OsTimerDelete(OSTimerObj_p ptimer);

/**
 *****************************************************************************
 * \fn          char* OsGetTaskName(unsigned int id)
 * \brief       ��ȡָ��ID��������.
 * \param[in]   id ����id��.
 * \note        .  
 * \return      ����ָ���������ֵ�ָ��.
 *****************************************************************************
 */
char* OsGetTaskName(unsigned int id);

/**
 *****************************************************************************
 * \fn          int OsGetTaskId(char* name,unsigned int* id)
 * \brief       ��ȡָ�����������ID.
 * \param[in]   name ��������.
 * \param[out]  id   ��ȡ����ID.
 * \note        .  
 * \return      unsigned int ���������ID.
 *****************************************************************************
 */
int OsGetTaskId(char* name,unsigned int* id);

/**
 *****************************************************************************
 * \fn          int OsCheckTaskId(unsigned int id);
 * \brief       ��������ID�Ƿ���Ч.
 * \param[out]  id   �����ID.
 * \note        .  
 * \retval      0 ��Ч.
 * \retval      -1 ��Ч.
 *****************************************************************************
 */
int OsCheckTaskId(unsigned int id);

/**
 *****************************************************************************
 * \fn          int OsGetTaskFreeStack(unsigned int id)
 * \brief       ��ȡָ��ID����Ķ�ջʣ����.
 * \param[in]   id ����ID.
 * \retval      >=0 ��������ʣ���ջ.
 * \retval      -1 ָ��ID���񲻴���.
 * \note        .  
 *****************************************************************************
 */
int OsGetTaskFreeStack(unsigned int id);

/**
*****************************************************************************
* \fn          void OsSetTaskDebug(unsigned int flag,osapi_debug_e level)
* \brief       ����������������־.
* \param[in]   id �����,ĳһλΪ1��ʾָ��ID�����������.
* \param[in]   level \ref osapi_debug_e.
* \note        .  
*****************************************************************************
*/
void OsSetTaskDebug(unsigned int id,osapi_debug_e level);
/**
*****************************************************************************
* \fn          void OsClrTaskDebug(unsigned int flag,osapi_debug_e level)
* \brief       ���������������־.
* \param[in]   id �����,ĳһλΪ1��ʾָ��ID�����������.
* \param[in]   level \ref osapi_debug_e.
* \note        .  
*****************************************************************************
*/
void OsClrTaskDebug(unsigned int id,osapi_debug_e level);



/**
 *****************************************************************************
 * \fn          int OsPrintf(osapi_debug_e level,const char *format, ...)
 * \brief       ��ӡ���������Ϣ:��ӡ��ʽ�������Ϣ.
 * \param[in]   format ��ʽ���ַ���, ��printf����һ��.
 * \note        .  
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int OsPrintf(osapi_debug_e level,const char *format, ...);

/**
 *****************************************************************************
 * \fn          int OsPrintf_va(csp_color_t color,osapi_debug_e level,const char *format, va_list arg)
 * \brief       ��ӡ���������Ϣ:��ӡ��ʽ�������Ϣ.
 * \param[in]   format ��ʽ���ַ���, ��printf����һ��.
 * \note        .  
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int OsPrintf_va(csp_color_t color,osapi_debug_e level,const char *format, va_list arg);

/**
 *****************************************************************************
 * \fn          int OsPrintfBuf(char *name,osapi_debug_e level, unsigned char * buff, unsigned int length)
 * \brief       ��ӡ���������Ϣ:��ӡ�ڴ���Ϣ.
 * \param[in]   name ��ӡ��ǩ.
 * \param[in]   buff �ڴ�����.
 * \param[in]   length ����������.
 * \note        .  
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int OsPrintfBuf(char *name,osapi_debug_e level, unsigned char * buff, unsigned int length);


/**
 *****************************************************************************
 * \fn          int OsPrintfTaskInfo(void)
 * \brief       ��ӡ������Ϣ.
 * \note        .  
 *****************************************************************************
 */
void OsPrintfTaskInfo(void);

/**
 *****************************************************************************
 * \fn          void OsPrintfSemInfo(void)
 * \brief       ��ӡ�ź�����Ϣ.
 * \note        .  
 *****************************************************************************
 */
void OsPrintfSemInfo(void);

/**
 *****************************************************************************
 * \fn          void OsPrintfMsgInfo(void)
 * \brief       ��ӡ��Ϣ������Ϣ.
 * \note        .  
 *****************************************************************************
 */
void OsPrintfMsgInfo(void);

/**
 *****************************************************************************
 * \fn          void OsPrintfMutexInfo(void)
 * \brief       ��ӡ��������Ϣ.
 * \note        .  
 *****************************************************************************
 */
void OsPrintfMutexInfo(void);

/**
 *****************************************************************************
 * \fn          void OsPrintfTimerInfo(void)
 * \brief       ��ӡ��ʱ����Ϣ.
 * \note        .  
 *****************************************************************************
 */
void OsPrintfTimerInfo(void);

/**
  * \}
  */

/**
 *****************************************************************************
 * \fn          unsigned int OsGetTs(void)
 * \brief       ��ȡʱ���.
 * \note        .  
 *****************************************************************************
 */
unsigned int OsGetTs(void);

/**
 *****************************************************************************
 * \fn          void OsLock(void)
 * \brief       �����������.
 * \note        .  
 *****************************************************************************
 */
void OsLock(void);

/**
 *****************************************************************************
 * \fn          void OsUnLock(void)
 * \brief       ��������������.
 * \note        .  
 *****************************************************************************
 */
void OsUnLock(void);

/**
 *****************************************************************************
 * \fn          void OsEnterInt(void)
 * \brief       �����ж�.
 * \note        .  
 *****************************************************************************
 */
void OsEnterInt(void);

/**
 *****************************************************************************
 * \fn          void OsExitInt(void)
 * \brief       �˳��ж�.
 * \note        .  
 *****************************************************************************
 */
void OsExitInt(void);

/** \defgroup OSAPI_samp ����ϵͳ��(OSAPI)ʹ������
  * \{
  */

/*****************************************************************************    
 *                                                                           
 *                             ʹ������                                   
 *                                                                            
 ****************************************************************************/

/**
 *****************************************************************************
 * \fn          void osapi_samp(void)
 * \brief       ʹ������.
 * \par   �μ�  ��ʾ������ǩ�µġ�osapi_samp.c��
 *****************************************************************************
 */
void osapi_samp(void);

/**
 *\example osapi_samp.c
 * ����ϵͳ��(OSAPI)ʹ������.\n
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