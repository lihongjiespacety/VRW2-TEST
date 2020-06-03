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


#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include "driver.h"
#include "osapi_freertos.h"
#include "task.h"
#include "csp.h"

#define tskSTACK_FILL_BYTE	( 0xa5U )

extern PRIVILEGED_DATA  List_t pxReadyTasksLists[ configMAX_PRIORITIES ];
extern PRIVILEGED_DATA  List_t xDelayedTaskList1;						
extern PRIVILEGED_DATA  List_t xDelayedTaskList2;						
extern PRIVILEGED_DATA  List_t * volatile pxDelayedTaskList;			
extern PRIVILEGED_DATA  List_t * volatile pxOverflowDelayedTaskList;	
extern PRIVILEGED_DATA  List_t xPendingReadyList;					
#if ( INCLUDE_vTaskSuspend == 1 )
extern PRIVILEGED_DATA  List_t xSuspendedTaskList;					/*< Tasks that are currently suspended. */
#endif
#if( INCLUDE_vTaskDelete == 1 )
extern PRIVILEGED_DATA List_t xTasksWaitingTermination;				/*< Tasks that have been deleted - but their memory not yet freed. */
#endif
/*****************************************************************************    
 *                                                                           
 *                             �ڲ�����                                   
 *                                                                            
 ****************************************************************************/

static OSSemObj_p pSemStdio=0;                     /**< ������Ϣ����ź���    */
static unsigned int gTaskDelFlag   = 0;          /**< ����ɾ�������־ */
static unsigned int gTaskDebugFlag = 0x0;          /**< ������������־ */
static unsigned char gTaskDebugLevel[20] = {0};    /**< ������������־ */

/*****************************************************************************    
 *                                                                           
 *                             �ڲ��ӿں���ʵ��                                   
 *                                                                            
 ****************************************************************************/


/*****************************************************************************    
 *                                                                           
 *                             ����ӿں���ʵ��                                   
 *                                                                            
 ****************************************************************************/

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
int OsMemcmp(void *pDst, void *pSrc, unsigned int nNumBytes)
{
    return memcmp(pDst, pSrc, nNumBytes);    
}

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
void OsMemcpy(void *pDst, void *pSrc, unsigned int nNumBytes)
{
    memcpy(pDst, pSrc, nNumBytes);
}

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
void OsMemset(void *pMem, unsigned char iChar, unsigned int nNumBytes)
{
    memset(pMem, iChar, nNumBytes);
}

/**
 *****************************************************************************
 * \fn          void* OsMalloc(unsigned int nNumBytes)
 * \brief       ���䶯̬�ڴ�.
 * \param[in]   nNumBytes ��Ҫ��ȡ���ֽ���.
 * \note        .  
 * \return      ָ���ȡ���ڴ� ����0�����ȡʧ��
 *****************************************************************************
 */
void* OsMalloc(unsigned int nNumBytes)
{
    void* p = 0;
    int  err=0;
    OsLock();
    if(err==0)
    {
      p = malloc(nNumBytes);
      OsUnLock();
    }
    return p;
}

/**
 *****************************************************************************
 * \fn          void OsFree(void *pMem)
 * \brief       �ͷŶ�̬�ڴ�.
 * \note        .  
 * \param[in]   pMem ָ����Ҫ�ͷŵ��ڴ�.
 *****************************************************************************
 */
void OsFree(void *pMem)
{ 
    OsLock();
    free(pMem);
    OsUnLock();
}

/**
 *****************************************************************************
 * \fn          int OsInit()
 * \brief       ��ʼ��ϵͳ.
 * \note        .  
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int OsInit()
{
  return 0;
}

/**
 *****************************************************************************
 * \fn          int OsInit()
 * \brief       ����ϵͳ.
 * \note        .  
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int OsStart()
{
  vTaskStartScheduler();
  return 0;
}

/**
 *****************************************************************************
 * \fn          int OsInitInFirstTask(void);
 * \brief       ��һ������������Ӧ�ĳ�ʼ������.
 * \note        ��ʼ��ϵͳʱ��,ͳ�������.  
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int OsInitInFirstTask(void)
{
  vTaskSetTaskNumber(xTaskGetIdleTaskHandle(),0xFF);
  vTaskSetTaskNumber(xTimerGetTimerDaemonTaskHandle(),0xFE);
  return 0;
}

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
int OsTaskCreate(TSK_CFG_t* cfg)
{
  /*������������ע���
  1.���ȼ�:��ֵ�����ȼ�����UCOS�෴,����������Ϊ�Ľ��з�ת
  2.��ջ����Զ�ջ����(StackType_t)Ϊ��λ
  */
#if 0
  TaskHandle_t handle=NULL;  /* һ��void*ָ��,ʵ��ʹ��ʱǿ��ת��ΪTCB_t* */
  handle = xTaskCreateStatic((TaskFunction_t)cfg->taskp,
                             (char * )cfg->name,
                             cfg->tsksize,
                             (void * )cfg->arg,
                             (UBaseType_t)(configMAX_PRIORITIES-cfg->pri),
                             (StackType_t * )cfg->tskbase,
                             (StaticTask_t * )cfg->tcb );
  if(handle==NULL)
  {
    return -1;
  }
  else
  {
    vTaskSetTaskNumber(handle,(UBaseType_t )cfg->id);
    return 0;
  }
#endif
  return 0;
}

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
int OsTimeDelay( unsigned int milsec )
{
  TickType_t tick = milsec/(1000/configTICK_RATE_HZ);
  if(tick == 0)
  {
    tick = 1;
  }
  vTaskDelay(tick);
  return 0;
}

/**
 *****************************************************************************
 * \fn          unsigned int OsTimeGetTick(void)
 * \brief       ��ȡϵͳ�δ������.
 * \note        .  
 * \retval      unsigned int ���صδ������ֵ
 *****************************************************************************
 */
unsigned int OsTimeGetTick(void)
{
  return xTaskGetTickCount();
}

/**
 *****************************************************************************
 * \fn          unsigned int OsTaskGetId(void)
 * \brief       ��ȡ��ǰ����ID.
 * \note        . 
 * \return      unsigned int ���ص�ǰ�����ID.
 *****************************************************************************
 */
unsigned int OsTaskGetId(void)
{
  TaskHandle_t handle=NULL;
  handle = xTaskGetCurrentTaskHandle();
  return ((StaticTask_t*)handle)->uxDummy10[0];
}

/**
 *****************************************************************************
 * \fn          unsigned int OsTaskGetSwCount(unsigned char id)
 * \brief       ��ȡָ��ID����ĵ��ȴ���.
 * \param[in]   id ����ID.
 * \note        . 
 * \return      unsigned int ��������ĵ��ȴ���.
 *****************************************************************************
 */
unsigned int OsTaskGetSwCount(unsigned int id)
{
#if 0 
  List_t *pxList;
  UBaseType_t uxTask = 0, uxQueue = configMAX_PRIORITIES;
  StaticTask_t *pxNextTCB, *pxFirstTCB;
  //OSCriticalEnter();
  OsLock();
  /*������������*/
  do
  {
    uxQueue--;
    pxList = &(pxReadyTasksLists[uxQueue]); 
    /*����ָ�����в���prvListTasksWithinSingleList����*/
    if(listCURRENT_LIST_LENGTH(pxList) > (UBaseType_t)0)
    {
      listGET_OWNER_OF_NEXT_ENTRY( pxFirstTCB, pxList );
      do
      {
        listGET_OWNER_OF_NEXT_ENTRY( pxNextTCB, pxList );
        if(pxNextTCB->uxDummy10[1] == id)
        {
          OsUnLock();
          //return pxNextTCB->ulDummy16;  /*Freertosû���л������� ʹ������ʱ�����*/
          return pxNextTCB->ulDummy22;   /*���������л�������ʵ��*/
        }
        uxTask++;
      } while( pxNextTCB != pxFirstTCB );
    }
    
  }while(uxQueue > (UBaseType_t)tskIDLE_PRIORITY); /*�����������ȼ����� ����uxTaskGetSystemState����*/
  /*����pxDelayedTaskList*/             
  pxList = (List_t *)pxDelayedTaskList; 
  if(listCURRENT_LIST_LENGTH(pxList) > (UBaseType_t)0)
  {
    listGET_OWNER_OF_NEXT_ENTRY( pxFirstTCB, pxList );
    do
    {
      listGET_OWNER_OF_NEXT_ENTRY( pxNextTCB, pxList );
      if(pxNextTCB->uxDummy10[1] == id)
      {
        OsUnLock();
        //return pxNextTCB->ulDummy16;  /*Freertosû���л������� ʹ������ʱ�����*/
        return pxNextTCB->ulDummy22;   /*���������л�������ʵ��*/
      }
      uxTask++;
    } while( pxNextTCB != pxFirstTCB );
  }
  /*����pxOverflowDelayedTaskList*/             
  pxList = (List_t *)pxOverflowDelayedTaskList; 
  if(listCURRENT_LIST_LENGTH(pxList) > (UBaseType_t)0)
  {
    listGET_OWNER_OF_NEXT_ENTRY( pxFirstTCB, pxList );
    do
    {
      listGET_OWNER_OF_NEXT_ENTRY( pxNextTCB, pxList );
      if(pxNextTCB->uxDummy10[1] == id)
      {
        OsUnLock();
        //return pxNextTCB->ulDummy16;  /*Freertosû���л������� ʹ������ʱ�����*/
        return pxNextTCB->ulDummy22;   /*���������л�������ʵ��*/
      }
      uxTask++;
    } while( pxNextTCB != pxFirstTCB );
  }
  
#if( INCLUDE_vTaskDelete == 1 )  /*����xTasksWaitingTermination*/  
  pxList = (List_t *)&xTasksWaitingTermination; 
  if(listCURRENT_LIST_LENGTH(pxList) > (UBaseType_t)0)
  {
    listGET_OWNER_OF_NEXT_ENTRY( pxFirstTCB, pxList );
    do
    {
      listGET_OWNER_OF_NEXT_ENTRY( pxNextTCB, pxList );
      if(pxNextTCB->uxDummy10[1] == id)
      {
        OsUnLock();
        //return pxNextTCB->ulDummy16;  /*Freertosû���л������� ʹ������ʱ�����*/
        return pxNextTCB->ulDummy22;   /*���������л�������ʵ��*/
      }
      uxTask++;
    } while( pxNextTCB != pxFirstTCB );
  }
#endif
  
#if ( INCLUDE_vTaskSuspend == 1 ) /*����xSuspendedTaskList*/  
  pxList = (List_t *)&xSuspendedTaskList; 
  if(listCURRENT_LIST_LENGTH(pxList) > (UBaseType_t)0)
  {
    listGET_OWNER_OF_NEXT_ENTRY( pxFirstTCB, pxList );
    do
    {
      listGET_OWNER_OF_NEXT_ENTRY( pxNextTCB, pxList );
      if(pxNextTCB->uxDummy10[1] == id)
      {
        OsUnLock();
        //return pxNextTCB->ulDummy16;  /*Freertosû���л������� ʹ������ʱ�����*/
        return pxNextTCB->ulDummy22;   /*���������л�������ʵ��*/
      }
      uxTask++;
    } while( pxNextTCB != pxFirstTCB );
  }
#endif
  //OSCriticalExit();
  OsUnLock();
#endif
  return 0;
}

/**
 *****************************************************************************
 * \fn          char* OsGetTaskName(unsigned int id)
 * \brief       ��ȡָ��ID��������.
 * \param[in]   id ����id��.
 * \note        .  
 * \return      ����ָ���������ֵ�ָ��.
 *****************************************************************************
 */
char* OsGetTaskName(unsigned int id)
{
  TaskStatus_t *pxTaskStatusArray;
  volatile UBaseType_t uxArraySize, x;
  uxArraySize = uxTaskGetNumberOfTasks();
  pxTaskStatusArray = OsMalloc( uxArraySize * sizeof( TaskStatus_t ) );
  if( pxTaskStatusArray != NULL )
  {
    uxArraySize = uxTaskGetSystemState( pxTaskStatusArray, uxArraySize, NULL );
    for( x = 0; x < uxArraySize; x++ )
    {
      if(pxTaskStatusArray[x].xTaskNumber == id)
      {
          OsFree(pxTaskStatusArray);
          return (char*)pxTaskStatusArray[x].pcTaskName;
      }
    }
    OsFree(pxTaskStatusArray);
  }
  return 0;
}

/**
 *****************************************************************************
 * \fn          int OsGetTaskId(char* name,unsigned int* id)
 * \brief       ��ȡָ�����������ID.
 * \param[in]   name ��������.
 * \param[out]  id   ��ȡ����ID.
 * \note        .  
 * \retval      0 ��Ч.
 * \retval      -1 ��Ч.
 *****************************************************************************
 */
int OsGetTaskId(char* name,unsigned int* id)
{
  TaskStatus_t *pxTaskStatusArray;
  volatile UBaseType_t uxArraySize, x;
  uxArraySize = uxTaskGetNumberOfTasks();
  pxTaskStatusArray = OsMalloc( uxArraySize * sizeof( TaskStatus_t ) );
  if( pxTaskStatusArray != NULL )
  {
    uxArraySize = uxTaskGetSystemState( pxTaskStatusArray, uxArraySize, NULL );
    for( x = 0; x < uxArraySize; x++ )
    {
      if(memcmp(name, pxTaskStatusArray[x].pcTaskName, strlen(name)) == 0)
      {
          OsFree(pxTaskStatusArray);
          *id = pxTaskStatusArray[x].xTaskNumber;
          return 0;
      }
    }
    OsFree(pxTaskStatusArray);
  }
  return -1;
}

/*����ID��ȡ���*/
static TaskHandle_t OsGetTaskHandle(unsigned int id)
{
  List_t *pxList;
  UBaseType_t uxTask = 0, uxQueue = configMAX_PRIORITIES;
  StaticTask_t *pxNextTCB, *pxFirstTCB;
  OsLock();
  /*������������*/
  do
  {
    uxQueue--;
    pxList = (List_t *)&(pxReadyTasksLists[uxQueue]); 
    /*����ָ�����в���prvListTasksWithinSingleList����*/
    if(listCURRENT_LIST_LENGTH(pxList) > (UBaseType_t)0)
    {
      listGET_OWNER_OF_NEXT_ENTRY( pxFirstTCB, pxList );
      do
      {
        listGET_OWNER_OF_NEXT_ENTRY( pxNextTCB, pxList );
        if(pxNextTCB->uxDummy10[1] == id)
        {
          OsUnLock();
          return (TaskHandle_t)pxNextTCB;  
        }
        uxTask++;
      } while( pxNextTCB != pxFirstTCB );
    }
    
  }while(uxQueue > (UBaseType_t)tskIDLE_PRIORITY); /*�����������ȼ����� ����uxTaskGetSystemState����*/
  /*����pxDelayedTaskList*/             
  pxList = (List_t *)pxDelayedTaskList; 
  if(listCURRENT_LIST_LENGTH(pxList) > (UBaseType_t)0)
  {
    listGET_OWNER_OF_NEXT_ENTRY( pxFirstTCB, pxList );
    do
    {
      listGET_OWNER_OF_NEXT_ENTRY( pxNextTCB, pxList );
      if(pxNextTCB->uxDummy10[1] == id)
      {
        OsUnLock();
        return (TaskHandle_t)pxNextTCB; 
      }
      uxTask++;
    } while( pxNextTCB != pxFirstTCB );
  }
  /*����pxOverflowDelayedTaskList*/             
  pxList = (List_t *)pxOverflowDelayedTaskList; 
  if(listCURRENT_LIST_LENGTH(pxList) > (UBaseType_t)0)
  {
    listGET_OWNER_OF_NEXT_ENTRY( pxFirstTCB, pxList );
    do
    {
      listGET_OWNER_OF_NEXT_ENTRY( pxNextTCB, pxList );
      if(pxNextTCB->uxDummy10[1] == id)
      {
        OsUnLock();
        return (TaskHandle_t)pxNextTCB; 
      }
      uxTask++;
    } while( pxNextTCB != pxFirstTCB );
  }
  
#if( INCLUDE_vTaskDelete == 1 )  /*����xTasksWaitingTermination*/  
  pxList = (List_t *)&xTasksWaitingTermination; 
  if(listCURRENT_LIST_LENGTH(pxList) > (UBaseType_t)0)
  {
    listGET_OWNER_OF_NEXT_ENTRY( pxFirstTCB, pxList );
    do
    {
      listGET_OWNER_OF_NEXT_ENTRY( pxNextTCB, pxList );
      if(pxNextTCB->uxDummy10[1] == id)
      {
        OsUnLock();
        return (TaskHandle_t)pxNextTCB; 
      }
      uxTask++;
    } while( pxNextTCB != pxFirstTCB );
  }
#endif
  
#if ( INCLUDE_vTaskSuspend == 1 ) /*����xSuspendedTaskList*/  
  pxList = (List_t *)&xSuspendedTaskList; 
  if(listCURRENT_LIST_LENGTH(pxList) > (UBaseType_t)0)
  {
    listGET_OWNER_OF_NEXT_ENTRY( pxFirstTCB, pxList );
    do
    {
      listGET_OWNER_OF_NEXT_ENTRY( pxNextTCB, pxList );
      if(pxNextTCB->uxDummy10[1] == id)
      {
        OsUnLock();
        return (TaskHandle_t)pxNextTCB; 
      }
      uxTask++;
    } while( pxNextTCB != pxFirstTCB );
  }
#endif
  OsUnLock();
  return 0;
}

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
int OsCheckTaskId(unsigned int id)
{
  if(OsGetTaskHandle(id) ==0)
  {
    return -1;
  }
  else
  {
    return 0;
  }
}

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
int OsGetTaskFreeStack(unsigned int id)
{
  List_t *pxList;
  unsigned char* pstk;
  unsigned int ulCount=0;
  UBaseType_t uxTask = 0, uxQueue = configMAX_PRIORITIES;
  StaticTask_t *pxNextTCB, *pxFirstTCB;
  OsLock();
  /*������������*/
  do
  {
    uxQueue--;
    pxList = (List_t *)&(pxReadyTasksLists[uxQueue]); 
    /*����ָ�����в���prvListTasksWithinSingleList����*/
    if(listCURRENT_LIST_LENGTH(pxList) > (UBaseType_t)0)
    {
      listGET_OWNER_OF_NEXT_ENTRY( pxFirstTCB, pxList );
      do
      {
        listGET_OWNER_OF_NEXT_ENTRY( pxNextTCB, pxList );
        if(pxNextTCB->uxDummy10[1] == id)
        {
          pstk = (unsigned char*)pxNextTCB->pxDummy6;
          while( *pstk == ( unsigned char)tskSTACK_FILL_BYTE )
          {
            pstk -= portSTACK_GROWTH;
            ulCount++;
          }
          OsUnLock();
          return ulCount;  
        }
        uxTask++;
      } while( pxNextTCB != pxFirstTCB );
    }
    
  }while(uxQueue > (UBaseType_t)tskIDLE_PRIORITY); /*�����������ȼ����� ����uxTaskGetSystemState����*/
  /*����pxDelayedTaskList*/             
  pxList = (List_t *)pxDelayedTaskList; 
  if(listCURRENT_LIST_LENGTH(pxList) > (UBaseType_t)0)
  {
    listGET_OWNER_OF_NEXT_ENTRY( pxFirstTCB, pxList );
    do
    {
      listGET_OWNER_OF_NEXT_ENTRY( pxNextTCB, pxList );
      if(pxNextTCB->uxDummy10[1] == id)
      {
        pstk = (unsigned char*)pxNextTCB->pxDummy6;
        while( *pstk == ( unsigned char)tskSTACK_FILL_BYTE )
        {
          pstk -= portSTACK_GROWTH;
          ulCount++;
        }
        OsUnLock();
        return ulCount; 
      }
      uxTask++;
    } while( pxNextTCB != pxFirstTCB );
  }
  /*����pxOverflowDelayedTaskList*/             
  pxList = (List_t *)pxOverflowDelayedTaskList; 
  if(listCURRENT_LIST_LENGTH(pxList) > (UBaseType_t)0)
  {
    listGET_OWNER_OF_NEXT_ENTRY( pxFirstTCB, pxList );
    do
    {
      listGET_OWNER_OF_NEXT_ENTRY( pxNextTCB, pxList );
      if(pxNextTCB->uxDummy10[1] == id)
      {
        pstk = (unsigned char*)pxNextTCB->pxDummy6;
        while( *pstk == ( unsigned char)tskSTACK_FILL_BYTE )
        {
          pstk -= portSTACK_GROWTH;
          ulCount++;
        }
        OsUnLock();
        return ulCount; 
      }
      uxTask++;
    } while( pxNextTCB != pxFirstTCB );
  }
  
#if( INCLUDE_vTaskDelete == 1 )  /*����xTasksWaitingTermination*/  
  pxList = (List_t *)&xTasksWaitingTermination; 
  if(listCURRENT_LIST_LENGTH(pxList) > (UBaseType_t)0)
  {
    listGET_OWNER_OF_NEXT_ENTRY( pxFirstTCB, pxList );
    do
    {
      listGET_OWNER_OF_NEXT_ENTRY( pxNextTCB, pxList );
      if(pxNextTCB->uxDummy10[1] == id)
      {
        pstk = (unsigned char*)pxNextTCB->pxDummy6;
        while( *pstk == ( unsigned char)tskSTACK_FILL_BYTE )
        {
          pstk -= portSTACK_GROWTH;
          ulCount++;
        }
        OsUnLock();
        return ulCount; 
      }
      uxTask++;
    } while( pxNextTCB != pxFirstTCB );
  }
#endif
  
#if ( INCLUDE_vTaskSuspend == 1 ) /*����xSuspendedTaskList*/  
  pxList = (List_t *)&xSuspendedTaskList; 
  if(listCURRENT_LIST_LENGTH(pxList) > (UBaseType_t)0)
  {
    listGET_OWNER_OF_NEXT_ENTRY( pxFirstTCB, pxList );
    do
    {
      listGET_OWNER_OF_NEXT_ENTRY( pxNextTCB, pxList );
      if(pxNextTCB->uxDummy10[1] == id)
      {
        pstk = (unsigned char*)pxNextTCB->pxDummy6;
        while( *pstk == ( unsigned char)tskSTACK_FILL_BYTE )
        {
          pstk -= portSTACK_GROWTH;
          ulCount++;
        }
        OsUnLock();
        return ulCount; 
      }
      uxTask++;
    } while( pxNextTCB != pxFirstTCB );
  }
#endif
  OsUnLock();
  return -1;
}

/**
 *****************************************************************************
 * \fn          void OsSetTaskKillReq(unsigned char id)
 * \brief       ����ɾ����������.
 * \param[in]   id ����id��,���ܳ���31.
 * \note        .  
 *****************************************************************************
 */
void OsSetTaskKillReq(unsigned char id)
{
	gTaskDelFlag |= (1<<id);
}

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
int OsGetTaskKillReq(unsigned char id)
{
	return (gTaskDelFlag & (1<<id));
}

/**
 *****************************************************************************
 * \fn          int OsTaskKillSelf(void)
 * \brief       ����ɾ���Լ�.
 * \note        .  
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int OsTaskKillSelf(void)
{
    TaskHandle_t xTask;
	unsigned char id = OsTaskGetId();
	if((gTaskDelFlag & (1<<id)) != 0)
	{
		gTaskDelFlag &= ~(1<<id);
        xTask = xTaskGetCurrentTaskHandle(); 
		vTaskDelete(xTask);
	}
    return 0;
}

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
int OsSuspendTask(unsigned int id)
{
  TaskHandle_t handle;
  handle = OsGetTaskHandle(id);
  if(handle != 0)
  {
    vTaskSuspend(handle);
    return 0;
  }
  else
  {
    return -1;
  }
}

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
int OsResumeTask(unsigned int id)
{
#if 0
  TaskHandle_t handle;
  handle = OsGetTaskHandle(id);
  if(handle != 0)
  {
    if(IsIntContext())
    {
      xTaskResumeFromISR(handle);
    }
    else
    {
      vTaskResume(handle);
    }
    return 0;
  }
  else
  {
    return -1;
  }
#endif
  return 0;
}

/**
 *****************************************************************************
 * \fn          OSSemObj_p OsSemCreate(unsigned char *name, unsigned char count)
 * \brief       �����ź���.
 * \param[in]   name �ź������� ���봫��ȫ�����ݻ���const����.
 * \param[in]   count �ź�����ʼֵ.
 * \note        �ź���ʵ����malloc����.  
 * \return      OSSemObj*  ���ش������ź���ָ��,���Ϊ0��ʾ����ʧ��.
 *****************************************************************************
 */
OSSemObj_p OsSemCreate(unsigned char *name, unsigned int count)
{
  
  return xSemaphoreCreateCounting(0xFFFFFFFF,count);
}

/**
 *****************************************************************************
 * \fn          int OsSemPend(OSSemObj_p psem, unsigned int timeout )
 * \brief       �ȴ��ź���.
 * \param[in]   psem �ź���ָ�� �������Ѿ��������ź���.
 * \param[in]   timeout �ȴ���ʱʱ�䵥λmS.
 * \note        .  
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int OsSemPend(OSSemObj_p psem, unsigned int timeout)
{	
  BaseType_t xYieldRequired;
  if(( * ( ( volatile uint32_t * ) 0xe000ed04 ) ) & 0xFFUL)
  {
    if(xSemaphoreTakeFromISR(psem,&xYieldRequired) == pdTRUE)
    {
      return 0;
    }
    else
    {
      return -1;
    }
  }
  else
  {
    if(xSemaphoreTake(psem,timeout) == pdTRUE)
    {
      return 0;
    }
    else
    {
      return -1;
    }
  }
}

/**
 *****************************************************************************
 * \fn          int OsSemPost(OSSemObj_p psem )
 * \brief       �����ź���.
 * \param[in]   psem �ź���ָ�� �������Ѿ��������ź���.
 * \note        .  
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int OsSemPost(OSSemObj_p psem )
{
  BaseType_t xYieldRequired;
  if(( * ( ( volatile uint32_t * ) 0xe000ed04 ) ) & 0xFFUL)
  {
    if(xSemaphoreGiveFromISR(psem,&xYieldRequired) == pdTRUE)
    {
      portYIELD_FROM_ISR( xYieldRequired );
      return 0;
    }
    else
    {
      return -1;
    }
  }
  else
  {
    if(xSemaphoreGive(psem) == pdTRUE)
    {
      return 0;
    }
    else
    {
      return -1;
    }
  }
}

/**
 *****************************************************************************
 * \fn          int OsSemDel(OSSemObj_p psem )
 * \brief       ɾ���ź���.
 * \param[in]   psem �ź���ָ�� �������Ѿ��������ź���.
 * \note        .  
 * \retval      0  �ɹ�.
 * \retval      -1 ʧ��.
 *****************************************************************************
 */
int OsSemDel(OSSemObj_p psem )
{ 
  vSemaphoreDelete(psem);
  //OsFree(psem);
  return 0;
}

/**
 *****************************************************************************
 * \fn          OSMutexObj_p OsMutexCreate(char *name)
 * \brief       ����������.
 * \param[in]   name ���������� ���봫��ȫ�����ݻ���const����.
 * \note        ��������ʵ����malloc����.  
 * \return      OSMutexObj_p  ���ش����Ļ�����ָ��,���Ϊ0��ʾ����ʧ��.
 *****************************************************************************
 */
OSMutexObj_p OsMutexCreate(char *name)
{
#if 0
  OSMutexObj_p pMutex = (OSMutexObj_p)OsMalloc(sizeof(OSMutexObj));
  if(pMutex != 0)
  {
    if (xSemaphoreCreateMutexStatic(pMutex) != NULL )
    {
      return pMutex;
    }
    else
    {
      OsFree(pMutex);
    }
  }    
#endif
  return 0; 
}

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
int OsMutexPend(OSMutexObj_p pmutex, unsigned int timeout)
{	
#if 0 
  BaseType_t xYieldRequired;
  if(IsIntContext())
  {
    if(xSemaphoreTakeFromISR(pmutex,&xYieldRequired) == pdTRUE)
    {
      return 0;
    }
    else
    {
      return -1;
    }
  }
  else
  {
    if(xSemaphoreTake(pmutex,timeout) == pdTRUE)
    {
      return 0;
    }
    else
    {
      return -1;
    }
  }
#endif
  return 0;
}

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
int OsMutexPost(OSMutexObj_p pmutex)
{
#if 0
  BaseType_t xYieldRequired;
  if(IsIntContext())
  {
    if(xSemaphoreGiveFromISR(pmutex,&xYieldRequired) == pdTRUE)
    {
      return 0;
    }
    else
    {
      return -1;
    }
  }
  else
  {
    if(xSemaphoreGive(pmutex) == pdTRUE)
    {
      return 0;
    }
    else
    {
      return -1;
    }
  }
#endif
  return 0;
}

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
int OsMutexDel(OSMutexObj_p pmutex)
{
  vSemaphoreDelete(pmutex);
  //OsFree(pmutex);
  return 0;
}

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
OSMSGQObj_p OsMsgQCreate(char *name,unsigned int size)
{
#if 0
  OSMSGQObj_p pMsgQ=0; 
  void* pData=0; 
  pMsgQ = (OSMSGQObj_p)OsMalloc(sizeof(OSMSGQObj));
  if (pMsgQ != 0)
  {
    pData = (void*)OsMalloc(sizeof(void*)*size); /*����������*/
    if(pData ==0)
    {
      OsFree(pMsgQ);
    }
    else
    {
      if(xQueueCreateStatic( size,sizeof(void*),pData,&(pMsgQ->queue)) != NULL)
      {
        pMsgQ->data = pData;                       /*ָ��������*/
        return pMsgQ;
      }
      else
      {
        OsFree(pMsgQ);
        OsFree(pData);
      }
    }
  }    
  return 0; 
#endif
  return 0;
}

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
int OsMsgQRcv(OSMSGQObj_p pMsgQ, unsigned char *buffer, unsigned int bufsize,unsigned int* size,unsigned int timeout)
{
#if 0
    BaseType_t xYieldRequired;
    BaseType_t res;
    unsigned int pmsg=NULL;
	OSAPI_MSGQ_FORM msgform;
    if(IsIntContext())
    {
      res = xQueueReceiveFromISR(&(pMsgQ->queue),&pmsg,&xYieldRequired);
    }
    else
    {
      res = xQueueReceive(&(pMsgQ->queue),&pmsg,timeout);
    }
    if(pdTRUE  == res)
	{
      if(pmsg != NULL)
      {
        msgform.addr = ((OSAPI_MSGQ_FORM*)pmsg)->addr;
        msgform.len = ((OSAPI_MSGQ_FORM*)pmsg)->len;
        if(bufsize>=msgform.len)
        {
          *size = msgform.len;
        }
        else
        {
          *size = bufsize; 
        }
		OsMemcpy(buffer, (unsigned char *)msgform.addr, *size);
		OsFree((unsigned char *)(msgform.addr));
		OsFree((void*)pmsg);
        return 0;
      }
      else
      {
		*size = 0;
        return -1;
      }
	}
	else
	{
		*size = 0;
        return -1;
	}
#endif
    return 0;
}


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
int OsMsgQSnd(OSMSGQObj_p pMsgQ, unsigned char *buffer, unsigned size)
{
#if 0
	unsigned char *pmsg;
	OSAPI_MSGQ_FORM *msgform;
    BaseType_t xYieldRequired;
	pmsg = (unsigned char *)OsMalloc(size);
	if(pmsg == 0) 
	{
		return -1;
	}
	msgform = (OSAPI_MSGQ_FORM *)OsMalloc(sizeof(OSAPI_MSGQ_FORM));	
	if(msgform == NULL) 
	{
		OsFree(pmsg);
		return -1;	
	}
	OsMemcpy(pmsg, buffer, size );
	msgform->addr = (unsigned int)pmsg;
	msgform->len = size;
    if(IsIntContext())
    {
      if(pdTRUE != xQueueSendFromISR(&(pMsgQ->queue),(void*)&msgform,&xYieldRequired))
      {
	    OsFree(pmsg);
	    OsFree(msgform);
        return -1;
      }	 
    }
    else
    {
      if(pdTRUE != xQueueSend(&pMsgQ->queue,(void*)&msgform,0))
      {
	    OsFree(pmsg);
	    OsFree(msgform);
        return -1;
      }	
    }	
#endif
	return 0;
}

/**
 *****************************************************************************
 * \fn          OSTimerObj_p OsTimerCreate(unsigned int time,OSAPI_TIMER_TYPE_e mode,OSAPI_TIMER_CALLBACK callback,char *name)
 * \brief       ������ʱ��.
 * \param[in]   time ��ʱ������,����������OS_CFG_TMR_TASK_RATE_HZ����(Ŀǰ����Ϊ100mS)
 * \param[in]   mode ��ʱ��ģʽ \ref OSAPI_TIMER_TYPE_e.
 * \param[in]   callback ��ʱ���ص����� \ref OSAPI_TIMER_CALLBACK.
 * \param[in]   name ��ʱ������.
 * \note        ��ʱ��ʵ����malloc����.  
 * \return      OSMutexObj_p  ���ش����Ļ�����ָ��,���Ϊ0��ʾ����ʧ��.
 *****************************************************************************
 */
OSTimerObj_p OsTimerCreate(unsigned int time,OSAPI_TIMER_TYPE_e mode,OSAPI_TIMER_CALLBACK callback,char *name)
{
#if 0
  OSTimerObj_p ptimer = (OSTimerObj_p)OsMalloc(sizeof(OSTimerObj));
  if(ptimer != NULL)
  {
    time = time/(1000/TICKS_PER_SEC);
    if(time == 0)
    {
      time = 1;
    }
    if(mode == OSAPI_TIMIER_SINGLE)
    {
      xTimerCreateStatic(name,time,pdFALSE,0,callback,ptimer); 
    }
    else
    {
      xTimerCreateStatic(name,time,pdTRUE,0,callback,ptimer);
    }
    return ptimer;
  }
  else
  {
    return 0; 
  }
#endif
  return 0;
}

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
int OsTimerStart(OSTimerObj_p ptimer)
{
#if 0
  BaseType_t xYieldRequired;
  if(IsIntContext())
  {
    if(pdPASS == xTimerStartFromISR(ptimer,&xYieldRequired))
    {
      return 0;
    }
    else
    {
      return -1;
    }
  }
  else
  {
    if(pdPASS == xTimerStart(ptimer,1000))
    {
      return 0;
    }
    else
    {
      return -1;
    }
  }
#endif
  return 0;
}

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
int OsTimerStop(OSTimerObj_p ptimer)
{
#if 0
  BaseType_t xYieldRequired;
  if(IsIntContext())
  {
    if(pdPASS == xTimerStopFromISR(ptimer,&xYieldRequired))
    {
      return 0;
    }
    else
    {
      return -1;
    }
  }
  else
  {
    if(pdPASS == xTimerStop(ptimer,1000))
    {
      return 0;
    }
    else
    {
      return -1;
    }
  }
#endif
  return 0;
}

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
int OsTimerDelete(OSTimerObj_p ptimer)
{
#if 0
  if(pdPASS == xTimerDelete(ptimer,1000))
  {
      OsFree(ptimer);
      return 0;
  }
  else
  {
    return -1;
  }
#endif
  return 0;
}



/**
*****************************************************************************
* \fn          void OsSetTaskDebug(unsigned int flag,osapi_debug_e level)
* \brief       ����������������־.
* \param[in]   id �����,ĳһλΪ1��ʾָ��ID�����������.
* \param[in]   level \ref osapi_debug_e.
* \note        .  
*****************************************************************************
*/
void OsSetTaskDebug(unsigned int id,osapi_debug_e level)
{
  uint8_t i;
  if(id == (uint32_t)0xFFFFFFFF)
  {
      gTaskDebugFlag =  (uint32_t)0xFFFFFFF;
      for(i=0;i<sizeof(gTaskDebugLevel)/sizeof(gTaskDebugLevel[0]);i++)
      {
        gTaskDebugLevel[id] = level;  
      }
  }
  else
  {
    gTaskDebugFlag |= 1<<id;
    gTaskDebugLevel[id] = level;
    
  }
}

/**
*****************************************************************************
* \fn          void OsClrTaskDebug(unsigned int flag,osapi_debug_e level)
* \brief       ���������������־.
* \param[in]   id �����,ĳһλΪ1��ʾָ��ID�����������.
* \param[in]   level \ref osapi_debug_e.
* \note        .  
*****************************************************************************
*/
void OsClrTaskDebug(unsigned int id,osapi_debug_e level)
{
  if(id == (uint32_t)0xFFFFFFFF)
  {
      gTaskDebugFlag = (uint32_t)0x0;
    
  }
  else
  {
    gTaskDebugFlag &= ~(1<<id);
    gTaskDebugLevel[id] = level;
    
  }
}


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
int OsPrintf(osapi_debug_e level,const char *format, ...)
{
    unsigned int id = OsTaskGetId();
	struct tm tm_time; 
	int ret;
    if(pSemStdio == 0)
    {
        pSemStdio = OsSemCreate("semstdio", 1);
        if(pSemStdio==0)
        {
            return -1;
        }
    }    

    if((gTaskDebugFlag & (1 << id)) && (level>gTaskDebugLevel[id]))
    {
        if(OsSemPend(pSemStdio, 2000)==0) 
        {
            ret = driver_time_gettm(&tm_time);
            if (ret == 0)
            {
                printf("[%02d-%02d %02d:%02d:%02d %s]:", 
                       tm_time.tm_mon+1, tm_time.tm_mday,    
                       tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
                       OsGetTaskName(id));
            }
            else
            {
                printf("[%s]:", OsGetTaskName(id));
            }
            va_list arg;
            va_start(arg, format);      
            vprintf(format, arg);
            va_end(arg);  
            printf("\r\n");
            OsSemPost(pSemStdio);
        }
    }
    return 0;
}

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
int OsPrintf_va(csp_color_t color,osapi_debug_e level,const char *format, va_list arg)
{
    unsigned int id = OsTaskGetId();
	struct tm tm_time; 
	int ret;
    if(pSemStdio == 0)
    {
        pSemStdio = OsSemCreate("semstdio", 1);
    }    

    if((gTaskDebugFlag & (1 << id)) && (level>gTaskDebugLevel[id]))
    {
        if(OsSemPend(pSemStdio, 1000)==0) 
        {
            csp_sys_set_color(color);
            ret = driver_time_gettm(&tm_time);
            if (ret == 0)
            {
                printf("[%02d-%02d %02d:%02d:%02d %s]:", 
                       tm_time.tm_mon+1, tm_time.tm_mday,    
                       tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
                       OsGetTaskName(id));
            }
            else
            {
                printf("[%s]:", OsGetTaskName(id));
            }
            
            vprintf(format, arg);
            printf("\r\n");
            csp_sys_set_color(COLOR_RESET);
            OsSemPost(pSemStdio);
        }
    }
    return 0;
}

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
int OsPrintfBuf(char *name,osapi_debug_e level, unsigned char * buff, unsigned int length)
{  
    unsigned int i;
    unsigned int j;
    unsigned char* p;
    int dat;
    struct tm tm_time; 
    unsigned int lines = length / 16;          /* ��ӡ����          */
    unsigned int lastline = length % 16;       /* ���һ�����ݸ���  */
    unsigned int id = OsTaskGetId();
    if(pSemStdio == 0)
    {
        pSemStdio = OsSemCreate("semstdio", 1);
        if(pSemStdio==NULL)
        {
            return -1;
        }
    }   

    if((gTaskDebugFlag & (1 << id)) && (level>gTaskDebugLevel[id]))
    {
        if(OsSemPend(pSemStdio, 1000)==0) 
        {
            int ret = driver_time_gettm(&tm_time);
            if (ret == 0)
            {
                printf("[%02d-%02d %02d:%02d:%02d %s]:", 
                       tm_time.tm_mon+1, tm_time.tm_mday,    
                       tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
                       OsGetTaskName(id));
            }
            else
            {
                printf("[%s]:", OsGetTaskName(id));
            }
            printf("%s length = %d, data: \r\n", name, length);
            p = buff;
            for(j=0;j<lines;j++)
            {
                 printf("%#10x:",(uint32_t)p);
                for (i = 0; i< 16; i++)
                {
                    dat = p[i]; 
                    printf("%02x ", dat);
                }
                printf("| ");
                for (i = 0; i< 16; i++)
                {
                    dat = p[i]; 
                    if(isprint(dat))
                    {
                        printf("%c ", dat);
                    }
                    else
                    {
                        printf(".");    
                    }
                }
                p += 16;
                printf("\r\n");
            }
            printf("%#10x:",(uint32_t)p);
            for (i = 0; i< lastline; i++)
            {
                dat = p[i]; 
                printf("%02x ", dat);
            }
            printf("| ");
            for (i = 0; i< lastline; i++)
            {
                dat = p[i]; 
                if(isprint(dat))
                {
                    printf("%c ", dat);
                }
                else
                {
                    printf(".");    
                }
            }
            printf("\r\n");
            OsSemPost(pSemStdio);
        }
    }
    return 0;	
}

extern uint32_t port_get_runtime(void);
/**
 *****************************************************************************
 * \fn          int OsPrintfTaskInfo(void)
 * \brief       ��ӡ������Ϣ.
 * \note        .  
 *****************************************************************************
 */
void OsPrintfTaskInfo(void)
{
  unsigned int usage=0;
  TaskStatus_t *pxTaskStatusArray=NULL;
  volatile UBaseType_t uxArraySize, x;
  char cStatus;
  uxArraySize = uxTaskGetNumberOfTasks();
  pxTaskStatusArray = OsMalloc( uxArraySize * sizeof( TaskStatus_t ) );
  if( pxTaskStatusArray != NULL )
  {
    uxArraySize = uxTaskGetSystemState( pxTaskStatusArray, uxArraySize, NULL );
    printf("\r\nname\t\t\tID\tprio\ttskbase\t\ttmsize\tmfree\tusage\t\truntime\trunnum      \tDescp.\r\n");
    printf("------------------------------------------------------------------------------------\r\n");
    for( x = 0; x < uxArraySize; x++ )
    {
      switch( pxTaskStatusArray[ x ].eCurrentState )
      {
      case eRunning:    cStatus = '-';
      break;
      case eReady:		cStatus = 'R';
      break;
      case eBlocked:		cStatus = 'B';
      break;
      case eSuspended:	cStatus = 'S';
      break;
      case eDeleted:		cStatus = 'D';
      break;
      default:	
        cStatus = 0x00;
        break;
      }
      if(portGET_RUN_TIME_COUNTER_VALUE() !=0)
      {
        usage = (pxTaskStatusArray[x].ulRunTimeCounter*10*100)/portGET_RUN_TIME_COUNTER_VALUE();  
      }
      printf("%-024s%d\t%d\t%#x\t%d\t",pxTaskStatusArray[x].pcTaskName,pxTaskStatusArray[x].xTaskNumber,pxTaskStatusArray[x].uxCurrentPriority,pxTaskStatusArray[x].pxStackBase,(((uint32_t)((StaticTask_t*)(pxTaskStatusArray[x].xHandle))->pxDummy8)-(uint32_t)(((StaticTask_t*)(pxTaskStatusArray[x].xHandle))->pxDummy6))+8);
      printf("%d\t%03d/1000\t%#x\t",pxTaskStatusArray[x].usStackHighWaterMark*4,usage,pxTaskStatusArray[x].ulRunTimeCounter);
      printf("%c|",cStatus);
      printf("%s\r\n","");
    }
    OsFree(pxTaskStatusArray);
  }
  printf("%s\r\n","");
}

/**
 *****************************************************************************
 * \fn          void OsPrintfSemInfo(void)
 * \brief       ��ӡ�ź�����Ϣ.
 * \note        .  
 *****************************************************************************
 */
void OsPrintfSemInfo(void)
{
 
}

/**
 *****************************************************************************
 * \fn          void OsPrintfMsgInfo(void)
 * \brief       ��ӡ��Ϣ������Ϣ.
 * \note        .  
 *****************************************************************************
 */
void OsPrintfMsgInfo(void)
{

}

/**
 *****************************************************************************
 * \fn          void OsPrintfMutexInfo(void)
 * \brief       ��ӡ��������Ϣ.
 * \note        .  
 *****************************************************************************
 */
void OsPrintfMutexInfo(void)
{

}

/**
 *****************************************************************************
 * \fn          void OsPrintfTimerInfo(void)
 * \brief       ��ӡ��ʱ����Ϣ.
 * \note        .  
 *****************************************************************************
 */
void OsPrintfTimerInfo(void)
{

}

/**
 *****************************************************************************
 * \fn          unsigned int OsGetTs(void)
 * \brief       ��ȡʱ���.
 * \note        .  
 *****************************************************************************
 */
unsigned int OsGetTs(void)
{
  //return ulMainGetRunTimeCounterValue(); /*��λmS*/
  return 0;
}


/**
 *****************************************************************************
 * \fn          void OsLock(void)
 * \brief       �����������.
 * \note        .  
 *****************************************************************************
*/
void OsLock(void)
{
#if 0
  if(IsIntContext()==0)
  {
    vTaskSuspendAll(); /*�ص���(�ж�δ�ر�)*/
  }
#endif
}

/**
 *****************************************************************************
 * \fn          void OsUnLock(void)
 * \brief       ��������������.
 * \note        .  
 *****************************************************************************
 */
void OsUnLock(void)
{
#if 0
  if(IsIntContext()==0)
  {
    xTaskResumeAll(); /*������*/
  }
#endif
}
/**
 *****************************************************************************
 * \fn          void OsEnterInt(void)
 * \brief       �����ж�.
 * \note        .  
 *****************************************************************************
 */
void OsEnterInt(void)
{
}

/**
 *****************************************************************************
 * \fn          void OsExitInt(void)
 * \brief       �˳��ж�.
 * \note        .  
 *****************************************************************************
 */
void OsExitInt(void)
{
}