/**
 *****************************************************************************        
 * \brief       ƽ̨��(PLATFORM)SHELL����ģ��(SHELL_FUN)������ݽṹ�ͽӿ�����.
 * \details     Copyright (c) 2017,��ɳ��ʤ��Ϣ�������޹�˾.
 *              All rights reserved.    
 * \file        shell_fun.h 
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

#ifndef __SHELL_FUN_H
#define __SHELL_FUN_H

#ifdef __cplusplus
 extern "C" {
#endif

/*****************************************************************************    
 *                                                                           
 *                             �������                                  
 *                                                                            
 ****************************************************************************/   
void HelpFun(void* param);

/*****************************************************************************    
 *                                                                           
 *                             ����ϵͳ���                                  
 *                                                                            
 ****************************************************************************/
void ResetFun(void* cmdbuf);
void SetCanidFun(void* cmdbuf);
void ShowPsFun(void * cmdbuf);
void ShowSemFun(void * cmdbuf);
void ShowMsgFun(void * cmdbuf);
void ShowMutexFun(void * cmdbuf);
void ShowTimerFun(void * cmdbuf);
void ShowHeapInfoFun(void * cmdbuf);
void ShowMem(void * cmdbuf);
void SetdClrdFun(void * cmdbuf);
void SetCspDebugFun(void * cmdbuf);
void SetTimeFun(void * cmdbuf);
void ShowTimeFun(void * cmdbuf);
void RunTimeFun(void * cmdbuf);
#ifdef __cplusplus
}
#endif

#endif