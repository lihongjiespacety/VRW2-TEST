/**
 *****************************************************************************        
 * \brief       ƽ̨���(PLATFORM)SHELLģ��(SHELL)��ؽӿ�ʵ��.
 * \details     Copyright (c) 2017,��ɳ��ʤ��Ϣ�������޹�˾.
 *              All rights reserved.    
 * \file        shell.c  
 * \author      ����W18900
 * \version     1.0 
 * \date        2017-08-26
 * \note        ʹ��ǰ��ο�ע��.\n
 * \since       ����      2017-8-26 11:11:46     1.0     �½� 
 * \par �޶���¼
 * - 2017-08-26 ��ʼ�汾
 * \par ��Դ˵��
 * - RAM:              
 * - ROM:
 *****************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include "shell.h"
#include "hal.h"
#include "osapi_freertos.h"
#include "driver.h"
#include "retarget.h"

/*****************************************************************************    
 *                                                                           
 *                             �ڲ�����                                   
 *                                                                            
 ****************************************************************************/
#define SHELL_CMD_LEN 64                          /**< shell�����������  */
extern const shell_cmd_cfg shell_cmd_list[ ];     /**< shell_fun�ж���  */
extern const shell_cmd_cfg shell_cmd_list_app[ ]; /**< app_shellfun�ж���  */
static int shellpassed = 1;  /**< shell�����Ƿ�У�� 0�Ѿ�У�� 1δУ��  */
static unsigned char cmd_buf[SHELL_CMD_LEN]="\r";
static int cmd_buf_index=0;  /**< ��ǰ���յ��������ַ���  */

/*****************************************************************************    
 *                                                                           
 *                             �ڲ��ӿں���ʵ��                                   
 *                                                                            
 ****************************************************************************/

/**
 *****************************************************************************
 * \fn          static int shell_getchar(unsigned char *data)
 * \brief       ��ȡһ���ַ�.
 * \note        . 
 * \param[out]  data �洢����������.
 * retval       0  �ɹ�.
 * retval       -1 ʧ��.
 *****************************************************************************
 */
static int shell_getchar(unsigned char *data)
{
    int erro=0;
    driver_uart_recv(getstdiouart(), data, 1,10,&erro);
    if(erro!=0)
    {
        return -1;
    }
    return 0;
}


/**
 *****************************************************************************
 * \fn          static unsigned int shell_cmd_len(char const *cmd)
 * \brief       ��ȡ�����ַ�����.
 * \note        �ո����\0����. 
 * \param[in]   cmd �ַ���
 * return       unsigned int �����ַ��ĳ���
 *****************************************************************************
 */
static unsigned int shell_cmd_len(unsigned char const *cmd)
{
    unsigned char const *p = cmd;
    unsigned int len = 0;
    while((*p != ' ') && (*p != 0)) 
    {
        p++;
        len++;
    }
    return len;
}

/**
 *****************************************************************************
 * \fn          static void shell_check_passwd(void)
 * \brief       shell����ȷ��.
 * \note        �ո����\0����. 
 *****************************************************************************
 */
static void shell_check_passwd(void)
{
	unsigned int i;
    unsigned char ch[6] = {0};
    unsigned char pw[7] = SHELL_PASSWORD_STR;

    while(1)
    {
        printf("\r\nPassword:");
        memset(ch, 0, sizeof(ch));
        for (i = 0; i < sizeof(ch); i++)
        {
            ch[i] = getchar();
            putchar('\b');
            putchar('*');
        }
        if (memcmp(pw, ch, sizeof(ch)) == 0)
        {
            printf("\r\n");
            break;
        }
        printf("\r\nAccess denied\r\n");
    }
}

/**
 *****************************************************************************
 * \fn          static int shell_cmd_check(unsigned char *cmd, char const *str)
 * \brief       ƥ�������ַ�.
 * \note        . 
 * \param[in]   cmd �ַ���
 * \param[in]   str ƥ���ַ�
 * retval       0 ƥ��ɹ�
 * retval       1 ƥ��ʧ��
 *****************************************************************************
 */
static int shell_cmd_check(unsigned char *cmd, unsigned char const *str)
{
    unsigned int len1 = shell_cmd_len((unsigned char const *)cmd);
    unsigned int len2 = shell_cmd_len(str);
    if(len1 != len2)
    {
        return 1;
    }
    return memcmp(cmd, str, len1);
}

/**
 *****************************************************************************
 * \fn          static unsigned int shell_read_line(int get(void), unsigned int len)
 * \brief       shell��������.
 * \note        �س�����,����ʹ��Backspace���������. 
 * \param[in]   get ����ӿں���
 * \param[in]   p   ���ջ�����
 * \param[in]   len ��Ҫ���յĳ���
 * return       unsigned int ʵ�ʽ��յĳ���
 *****************************************************************************
 */
static unsigned int shell_read_line(int get(unsigned char* tmp))
{
    unsigned char ch = '\r';
    unsigned int count;
    unsigned char tmp;
    /*��ʼ��ӡһ��"sh>"*/
    if(cmd_buf[0]=='\r')
    {
        printf("sh>\r\n");
        OsMemset(cmd_buf,0x00,sizeof(cmd_buf));
    }
    /*������յ��ַ�������,���򷵻�*/
    if(get(&tmp)==0)
    {
        ch = tmp; 
    }
    else
    {
        return 0;
    }
    /*������յ��Ǵ�ӡ�ַ��ҵ�ǰ���ջ�������Ϊ0����Ϊ���յ�һ֡�����ӡ"SH>"*/
    if((ch == '\r' || ch == '\n' || ch < ' ' || ch > '~') && (ch != '\b'))
    {
        if(cmd_buf_index==0)
        {
            printf("sh>\r\n");
        }
        else
        {
            count = cmd_buf_index;
            cmd_buf[cmd_buf_index]=0;
            cmd_buf_index =0;
            printf("\r\n");
            return count;
        }
    }
    else 
    {
        if(ch == '\b') 
        {
            if(cmd_buf_index != 0) 
            {
                cmd_buf_index--;
                putchar('\b');
                putchar(' ');
                putchar('\b');
                cmd_buf[cmd_buf_index]= '\0';
            }
        } 
        else 
        {
            /*������յ���ӡ�ַ��ҵ�ǰ���ջ�����������Ϊ������һ֡"*/
            putchar(ch);
            cmd_buf[cmd_buf_index++] = ch;
            if(cmd_buf_index>=(sizeof(cmd_buf)-1))
            {
                count = cmd_buf_index;
                cmd_buf[cmd_buf_index]=0;
                cmd_buf_index =0;
                printf("\r\n");
                return count;
            }
        } 
    } 
    return 0;
}

/**
 *****************************************************************************
 * \fn          int shell_exec_cmdlist(unsigned char* cmd)
 * \brief       ���������б�ִ��.
 * retval       0 �ɹ�
 * retval       -1 ʧ��
 * \note        . 
 *****************************************************************************
 */
int shell_exec_cmdlist(unsigned char* cmd)
{
    int i;
    /*ƽ̨�������*/
    for (i=0; shell_cmd_list[i].name != 0; i++)
    {
        if (shell_cmd_check(cmd, shell_cmd_list[i].name) == 0) 
        {
            shell_cmd_list[i].func(cmd);
            return 0;
        }            
    } 
//    /*Ӧ���������*/
//    for (i=0; shell_cmd_list_app[i].name != 0; i++)
//    {
//        if (shell_cmd_check(cmd, shell_cmd_list_app[i].name) == 0) 
//        {
//            shell_cmd_list_app[i].func(cmd);
//            return 0;
//        }            
//    }  
    if(shell_cmd_list[i].name == NULL)
    {
        printf("unkown command\r\n");
        return -1;
    }
    return 0;
}

/*****************************************************************************    
 *                                                                           
 *                             ����ӿں���ʵ��                                   
 *                                                                            
 ****************************************************************************/
/**
 *****************************************************************************
 * \fn          void shell_exec_shellcmd(void)
 * \brief       ִ��shell����.
 * \note        �������ڵ��øú���. 
 *****************************************************************************
 */
void shell_exec_shellcmd(void)
{
    if (!shellpassed)
    {
        shell_check_passwd();
        shellpassed = 1;
    } 
    if(shell_read_line(shell_getchar))
    {
        shell_exec_cmdlist(cmd_buf);
    }
}
