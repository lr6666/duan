#include "../../include/myhead.h"

/****************************************
Function:        void after_log()
Description:     登录后的页面
Calls:           no
Call By:         void *thread_read(void *arg)
Input:           no
Output:          no
return:          NULL
Others:          no
****************************************/

extern int is_log;
extern int is_slience;
extern char present_user[NAME_LEN];
extern int id;

void after_log()             //登录后界面
{
	printf("\n\n\n\n\n");
	printf("     \t\t ******************************************************\n");
	printf("     \t\t                                                        \n");
	printf("     \t\t *                  欢迎使用聊天室系统                * \n");
	printf("     \t\t *        注册 :         reg                          * \n");
	printf("     \t\t *        显示在线用户 : show                         * \n");
	printf("     \t\t *        悄悄话 :       whsp                         * \n");
	printf("     \t\t *        群聊 :         chat                         * \n");
	printf("     \t\t *        传输文件 :     tran                         * \n");
	printf("     \t\t *        修改密码 :     chpswd                       * \n");
	printf("     \t\t *        修改用户名 :   chname                       * \n");
	printf("     \t\t *        退出 :         quit                         * \n");
	printf("     \t\t *        返回页面:      table                        * \n");
	printf("     \t\t *                                                    * \n");
	if(strcmp(present_user,"admin") == 0)          //是否是管理员
	{
		printf("     \t\t *  亲爱的管理员,你还可以对普通用户进行以下操作:      * \n");
		printf("     \t\t *         禁言:   slience                            * \n");
		printf("     \t\t *         解禁:   say                                * \n");
		printf("     \t\t *         踢人:   getout                             * \n");
	}	
	printf("     \t\t                                   当前用户: %s   \n",present_user);
	printf("     \t\t                                         id: %d  \n",id);
	printf("     \t\t **************************************************** \n");
}
