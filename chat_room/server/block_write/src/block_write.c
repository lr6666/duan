#include "../../include/myhead.h"

/****************************************
Function:        int block_write(int,char *,int)
Description:     发送文件
Calls:           no
Call By:         int msg_handle(msg_t *,int)
Input:           int fd,char *data,int len
Output:          no
return:          no
Others:          no
****************************************/

int block_write(int fd,char *data,int len)                    //发送文件
{
    int write_size = 0;                                       //已发送大小 
    int n = 0;                                                //当前发送的大小   

    while(write_size < len)
    {
        n = write(fd,data + write_size,len - write_size);

        if(n < 0)
        {
            //perror("write fail!\n");                        //发送失败
            break;
        }

        else if(0 == n)
        {
            //perror("write 0\n");                            //无数据
            break;
        }

        write_size += n;
    }
    return 0;
}
