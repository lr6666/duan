#include "../../include/myhead.h"

/****************************************
Function:        int block_read(int fd,char *data,int len,char *fname)
Description:     接收文件
Calls:           no
Call By:         void *thread_read(void *arg)
Input:           int fd(连接描述符),  char *data(分配空间保存文件)
                 int len(文件长度),   char *fname(文件名)
Output:          no
return:          no
Others:          no
****************************************/

int block_read(int fd,char *data,int len,char *fname)   //接收文件
{
	int read_size = 0;                                  //已读大小
	int n = 0;                                          //每次读取大小
	while(read_size < len)                                   
	{ 
		n = read(fd,data + read_size,len - read_size);                
		
		if(n < 0)
		{
			perror("read fail!\n");                     //读取错误
			break;
		}
		
		else if(0 == n)
		{
			perror("read 0\n");                         //无内容
			break;
		}
		
		read_size += n;
		printf("文件 %s 已接收 %d%,请耐心等待!\n",fname,(100 * read_size)/len);	
	}
	
	return 0;
}
