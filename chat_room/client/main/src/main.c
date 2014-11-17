/****************************************
Copyright (c), 1988-1999, Huawei Tech. Co., Ltd.
File name: chat_room_client
Author: yinchenyang  Version: 1.0   Date:  2014.07.27
Description: 聊天室系统,客户端
Others:    no
Function List:  1. int main()                       //连接服务器,创建读写线程
                2. void log_or_register()           //注册与登录页面
                3. void after_log()                 //登录后页面
                4. int getpasswd(char *,int)        //输入密码
                5. int set_disp_mode(int,int)       //取消回显
                6. void *thread_read(void *)        //读取服务器指令与消息
                7. void *thread_write(void *)       //向服务器发送消息与指令
History:   no
*****************************************/


#include "../../include/myhead.h"

/****************************************
Function:        int main()
Description:     连接服务器,创建读写线程
Calls:           void *thread_write(void *arg)
                 void *thread_read(void *arg)   
Call By:         no
Input:           no
Output:          no
return:          no
Others:          no
****************************************/

int is_log = NO;
int is_slience = NO;
char present_user[NAME_LEN] = {0};
int id = 0;
int is_tran = NO;
char tran_target[NAME_LEN] = {0};
char file_name[NAME_LEN] = {0};

void *thread_read(void *arg);
void *thread_write(void *arg);

int main(int argc,char *argv[])
{
	system("reset");

	struct sockaddr_in server;                                        //设定ip与端口
	memset(&server,0,sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");

	char cmd[CMD_SIZE] = {0};
	int log_return = -1;
	int sockfd = socket(PF_INET,SOCK_STREAM,0);
	int n = connect(sockfd,(struct sockaddr *)&server,sizeof(server));     //连接服务器
    
	if(n < 0)                                                              //连接失败  无法运行
	{
	    printf("未连接至服务器!\n");
		close(sockfd);
		return 0;
	}

	pthread_t pid_write = 0;                  //发送指令线程id         
	pthread_t pid_read = 1;                   //接收消息线程id 

	int ret = pthread_create(&pid_write,NULL,(void *)thread_write,&(sockfd));   //创建发送指令线程
	if(0 != ret)
	{
		perror(strerror(ret));
		return CREATE_FAIL;
	}

	ret = pthread_create(&pid_read,NULL,(void *)thread_read,&(sockfd));         //创建接收消息线程  
	if(0 != ret)
	{
		perror(strerror(ret));
		return CREATE_FAIL;
	}

	pthread_join(pid_write,NULL);                                               //等待线程结束
	pthread_join(pid_read,NULL);                                                 

	system("reset");
	printf("感谢您的使用,再见!\n");
	close(sockfd);
	return 0;
}
