#include "../../include/myhead.h"

/****************************************
Function:        void log_or_register()
Description:     注册与登录界面
Calls:           no
Call By:         1.void *thread_write(void *arg) 
                 2.void *thread_read(void *arg) 
Input:           no
Output:          no
return:          NULL
Others:          no
****************************************/

void log_or_register()                    //登录界面
{
	printf("\n\n\n\n\n");
	printf("     \t\t   ********************************************* \n");
	printf("     \t\t                                                 \n");
	printf("     \t\t   *             欢迎使用聊天室系统            * \n");
	printf("     \t\t   *                                           * \n");
	printf("     \t\t   *   --------                     --------   * \n");
	printf("     \t\t   *   | 登录 |                     | 注册 |   * \n");
	printf("     \t\t   *   --------                     --------   * \n");
	printf("     \t\t                                                 \n");
	printf("     \t\t   ********************************************* \n");
	printf("\n\n\n\t\t      登录:log,注册:reg,退出:quit         \n\n");
}
