#include "../../include/myhead.h"

/****************************************
Function:        int block_write(int sockfd,char *data,int file_len,char *fname)
Description:     用于保证完整的发送文件
Calls:           no
Call By:         void *thread_write(void *arg)
Input:           int sockfd(连接描述符) ,char *data(已读取的文件内容) 
                 int file_len(文件大小) ,char *fname(文件名)
Output:          no
return:          no
Others:          no
****************************************/

int block_write(int sockfd,char *data,int file_len,char *fname)   //发送文件
{
	int write_size = 0;                                           //已发送大小
	int n = 0;                                                    //每次发送的大小
	
	while(write_size < file_len)
	{     
		n = write(sockfd,data + write_size,file_len - write_size);	
		write_size += n;
		printf("文件 %s 已传输 %d%,请耐心等待!\n",fname,(100 * write_size)/file_len);	
	}
	
	return 0;
}
