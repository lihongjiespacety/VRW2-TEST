/**
 *****************************************************************************        
 * \brief       ƽ̨���(PLATFORM)SHELLģ��(SHELL)������ݽṹ�ͽӿ�����.
 * \details     Copyright (c) 2017,��ɳ��ʤ��Ϣ�������޹�˾.
 *              All rights reserved.    
 * \file        shell.h  
 * \author      ����W18900
 * \version     1.0 
 * \date        2017-08-26
 * \note        ʹ��ǰ��ο�ע��.\n
 * \since       ����      2017-8-26 11:10:35     1.0     �½� 
 * \par �޶���¼
 * - 2017-08-26 ��ʼ�汾
 * \par ��Դ˵��
 * - RAM:              
 * - ROM:
 *****************************************************************************
 */

#ifndef _SHELL_H_
#define _SHELL_H_

#ifdef __cplusplus
extern "C" {
#endif
     
/** \addtogroup PLATFORM PLATFORM
 *  \{
 */
         
/** \addtogroup PLATFORM_SHELL PLATFORM_SHELL
 *  \{
 */
/*****************************************************************************    
 *                                                                           *
 *                             ���ݽṹ����                                  *
 *                                                                          *
 ****************************************************************************/
     
/** \defgroup PLATFORM_SHELL_data PLATFORM_SHELL_data
  * \{
  */

typedef void ( * CommandFunc )( void *);   /**< shell����ص����� */

/**
 * \struct shell_cmd_cfg
 * SHELL����ṹ��.
 */
typedef struct
{
    unsigned char const* name;           /**< shell������ */
    CommandFunc func;           /**< shell����ص����� */
    char const* helpstr;       /**< �����ַ��� */
}shell_cmd_cfg;

#define SHELL_PASSWORD_STR "DSWYBS"  /**< shell����̶�6���ַ�  */
#define SHELL_CMDBUF_SIZE  64         /**< shell���������С  */


/**
  * \}
  */
     
/*****************************************************************************    
 *                                                                           
 *                             �ӿں�������                                   
 *                                                                            
 ****************************************************************************/

/** \defgroup PLATFORM_SHELL_if PLATFORM_SHELL_if
  * \{
  */
    
/**
 *****************************************************************************
 * \fn          void shell_exec_shellcmd(void)
 * \brief       ִ��shell����.
 * \note        �������ڵ��øú���. 
 *****************************************************************************
 */
void shell_exec_shellcmd(void);
/**
 *****************************************************************************
 * \fn          int shell_exec_cmdlist(unsigned char* cmd)
 * \brief       ���������б�ִ��.
 * retval       0 �ɹ�
 * retval       -11 ʧ��
 * \note        . 
 *****************************************************************************
 */
int shell_exec_cmdlist(unsigned char* cmd);
/**
  * \}
  */

/** \defgroup PLATFORM_SHELL_samp PLATFORM_SHELL_samp
  * \{
  */

/*****************************************************************************    
 *                                                                           
 *                             ʹ������                                   
 *                                                                            
 ****************************************************************************/

/**
 *****************************************************************************
 * \fn          void platform_shell_samp(void)
 * \brief       ʹ������.
 * \par   �μ�  ��ʾ������ǩ�µġ�platform_shell_samp.c��
 *****************************************************************************
 */
void platform_shell_samp(void);

/**
 *\example platform_shell_samp.c
 * ƽ̨��(PLATFORM)SHELLģ��(SHELL)ʹ������.\n
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
