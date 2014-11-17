#include "../../include/myhead.h"

/****************************************
Function:        int block_read(int,char,int)
Description:     接收文件
Calls:           no
Call By:         int msg_handle(msg_t *,int)
Input:           int fd,char *data,int len
Output:          no
return:          no
Others:          no
****************************************/

int block_read(int fd,char *data,int len)            //接收文件  
{
    int read_size = 0;                               //已接收大小
    int n = 0;                                       //每次接收的大小

    while(read_size < len)
    {
        n = read(fd,data + read_size,len - read_size);

        if(n < 0)
        {
            perror("read fail!\n");                  //接收失败
            break;
        }

        else if(0 == n)
        {
            perror("read 0\n");                      //无数据
            break;
        }

        read_size += n;
    }
    return 0;
}
