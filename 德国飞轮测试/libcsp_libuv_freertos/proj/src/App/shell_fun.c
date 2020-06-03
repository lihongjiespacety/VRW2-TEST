/**
 *****************************************************************************        
 * \brief       ƽ̨��(PLATFORM)SHELL����ģ��(SHELL_FUN)��ؽӿ�ʵ��.
 * \details     Copyright (c) 2017,��ɳ��ʤ��Ϣ�������޹�˾.
 *              All rights reserved.    
 * \file        shell_fun.c 
 * \author      ����W18900
 * \version     1.0 
 * \date        2017-11-02
 * \note        ƽ̨�������.\n
 * \since       ����     2017��11��2��13:27:57   1.0     �½� 
 * \par �޶���¼
 * - 2017-11-02 ��ʼ�汾
 * \par ��Դ˵��
 * - RAM:              
 * - ROM:
 *****************************************************************************
 */

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "shell_fun.h"
#include "hal.h"
#include "driver.h"
#include "osapi_freertos.h"
#include "shell.h"
#include "csp.h"
#include "stm32f10x.h"
#include "core_cm3.h"

/*****************************************************************************    
 *                                                                           
 *                             �ڲ�����                                   
 *                                                                            
 ****************************************************************************/

const shell_cmd_cfg shell_cmd_list[ ] = 
{
  /*1.�������*/
  { "help",         HelpFun,           "help"},    /*��ӡ������Ϣ*/
  /*4.����ϵͳ���*/
  { "reset",        ResetFun,           "reset 0|1"},
  { "canid",        SetCanidFun,        "canid src dst port"},
  { "ps",ShowPsFun,""},	
  { "setd",        SetdClrdFun,     ""},
  { "clrd",        SetdClrdFun,     ""},
  { "cspdebug",    SetCspDebugFun, ""},
  { "sem",ShowSemFun,""},	
  { "msg",ShowMsgFun,""},
  { "mutex",ShowMutexFun,""},
  { "timer",ShowTimerFun,""},
  { "heap",ShowHeapInfoFun,""},
  { "settime",      SetTimeFun,          ""},
  { "t",	           ShowTimeFun,         ""},
  { "runtime",      RunTimeFun,      ""},
  { "mem",ShowMem,""},
  { 0,		0 },
};

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

/*****************************************************************************    
 *                                                                           
 *                             �������                                  
 *                                                                            
 ****************************************************************************/

void HelpFun(void* param)
{
    unsigned int i;
    printf("\r\n");
    printf("**************\r\n");
    printf("*   SHELL    *\r\n");
    printf("*   V1.0     *\r\n");
    printf("**************\r\n");
    printf("\r\n");
    for (i=0; shell_cmd_list[i].name != 0; i++)
    {
        printf("%02d.",i);
        printf("%-16s",shell_cmd_list[i].name);
        printf("%s\r\n",shell_cmd_list[i].helpstr);
    }
}

/*****************************************************************************    
 *                                                                           
 *                             ����ϵͳ���                                  
 *                                                                            
 ****************************************************************************/

/**
*****************************************************************************
* \fn          void ResetFun(void * cmdbuf);
* \brief       �������߻�ȡ��λԭ��.
* \note        .
* \param[in]   param �������ַ���, ie "reset / reset 0 ; reset 1"
*****************************************************************************
*/
void ResetFun(void* cmdbuf)
{
    uint32_t rebootmode;
    uint32_t len = sscanf((char const*)cmdbuf, "%*s %d", &rebootmode);

    if(len != 0)
    {

        NVIC_SystemReset();
    }
}

void SetCanidFun(void* cmdbuf)
{
    int src,dst,port;
    uint32_t len = sscanf((char const*)cmdbuf, "%*s %d %d %d", &src,&dst,&port);
    if(len==3)
    {
        kiss_set_clientaddr(src);
        kiss_set_serveraddr(dst);
        kiss_set_serverport(port);
    }
    else
    {
      printf("clientaddr:%d\r\n",kiss_get_clientaddr());
      printf("serveraddr:%d\r\n",kiss_get_serveraddr());
      printf("serverport:%d\r\n",kiss_get_serverport());
    }
}

void ShowPsFun(void * cmdbuf)
{
    OsPrintfTaskInfo();
}

void ShowSemFun(void * cmdbuf)
{
    OsPrintfSemInfo();
}

void ShowMsgFun(void * cmdbuf)
{
    OsPrintfMsgInfo();
}

void ShowMutexFun(void * cmdbuf)
{
    OsPrintfMutexInfo();
}

void ShowTimerFun(void * cmdbuf)
{
    OsPrintfTimerInfo();
}

void ShowHeapInfoFun(void * cmdbuf)
{
  //PrintHeapInfo();
}


void ShowMem(void * cmdbuf)
{
  unsigned int add;
  unsigned int size;
  unsigned int len = sscanf((char const *)cmdbuf, "%*s %x %d", &add,&size);
  if(len ==2)
  {
    OsPrintfBuf("mem:", OSAPI_DEBUG_INFO,(unsigned char*)add, size);
  }
  else
  {
    printf("mem add(hex) size\r\n ");	
  }
}

void SetdClrdFun(void * cmdbuf)
{
  char taskname[32] = {0};
  int level;
  unsigned int len = sscanf((char const *)cmdbuf, "%*s %31s %d", (char *)&taskname ,&level);
  unsigned int id=0;
  if (len == 2)
  {
    char cmdtype = 1;
    if(memcmp(cmdbuf, "clrd", strlen("clrd")) == 0)
    {
      cmdtype = 0;
    }
    if (strcmp(taskname, "all") == 0)
    {
      if (cmdtype == 1)
      {
        OsSetTaskDebug((uint32_t)0xFFFFFFFF,(osapi_debug_e)level);
      }
      else
      {
        OsClrTaskDebug((uint32_t)0xFFFFFFFF,(osapi_debug_e)level);
      }
    }
    else
    {
      if(OsGetTaskId(taskname,&id)==0)
      {
        
        if (cmdtype == 1)
        {
          OsSetTaskDebug(id,(osapi_debug_e)level);
        }
        else
        {
          OsClrTaskDebug(id,(osapi_debug_e)level);
        }
      }
    }
  }
  else
  {
    printf("usage: \r\n\t[setd or clrd] <taskname> or <all> <level>\r\n");
  }
}


void SetCspDebugFun(void * cmdbuf)
{
  int level;
  int onoff;
  unsigned int len = sscanf((char const *)cmdbuf, "%*s %d %d",&level ,&onoff);
  if (len == 2)
  {
     csp_debug_set_level((csp_debug_level_t)level,onoff);
  }
  else
  {
    printf("usage: \r\n\tset 0-6 0/1\r\n");
  }
}

void SetTimeFun(void * cmdbuf)
{
	unsigned int year, month, day, week, hour, minute, second;
	unsigned int len = sscanf((char const *)cmdbuf, "%*s %d %d %d %d %d %d %d", &year, &month, &day, &week, &hour, &minute, &second);
	if (len == 7)
	{
		struct tm tm_time;
		tm_time.tm_year = year -1900;
		tm_time.tm_mon = month - 1;
		tm_time.tm_mday = day;
		tm_time.tm_wday = week;
		tm_time.tm_hour = hour;
		tm_time.tm_min = minute;
		tm_time.tm_sec = second;
		tm_time.tm_isdst = -1;
        driver_time_settm(&tm_time);
	}
	else
	{
		printf("usage: \r\n\tsettime <year> <month> <day> <week> <hour> <minute> <second>\r\n");
	}
}

void ShowTimeFun(void * cmdbuf)
{
	struct tm tm_time;
    int res;
    res = driver_time_gettm(&tm_time);
    if(res==0)
    {
        printf("%04d-%02d-%02d %02d %02d:%02d:%02d \r\n",
               tm_time.tm_year + 1900,
               tm_time.tm_mon + 1,
               tm_time.tm_mday,
               tm_time.tm_wday,
               tm_time.tm_hour,
               tm_time.tm_min,
               tm_time.tm_sec);
    }
    else
    {
        printf("��ȡʱ��ʧ��\r\n");
    }
}

void RunTimeFun(void * cmdbuf)
{
    printf("%dS\r\n",driver_time_getruntime());
}