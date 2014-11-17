/*****************************************
Copyright (c), 1988-1999, Huawei Tech. Co., Ltd.
File name: 1.c
Author: yinchenyang  Version: 1.0   Date:  2014.07.27
Description: 聊天室系统,服务器
Others:    no
Function List:  1. int main()                                                //监听客户端连接与消息指令
                2. int block_read(int,char *,int)                            //接收文件
                3. int block_write(int,char *,int)                           //发送文件
                4. int msg_handle(msg_t *,int)                               //处理客户端的指令与消息
                5.static int call_back(void *,int,char **,char **)           //查找表中信息并返回
History:   no
*****************************************/


#include "../../include/myhead.h"

/****************************************
Function:        int main()
Description:     监听客户端连接与消息指令
Calls:           int msg_handle(msg_t *,int)
Call By:         no 
Input:           no
Output:          no
return:          no
Others:          no
****************************************/

char str_return[100] = {0};

int main(void)
{
    int  lfd;
    int cfd;
    int sfd;
    int rdy;
    
    struct sockaddr_in sin;
    struct sockaddr_in cin;

    int client[FD_SETSIZE];  /* 客户端连接的套接字描述符数组 */

    int maxi;
    int maxfd;                        /* 最大连接数 */

    fd_set rset;
    fd_set allset;

    socklen_t addr_len;         /* 地址结构长度 */

    int i;
    int n;
    int len;
    int opt = 1;   /* 套接字选项 */

    char addr_p[20];

    sqlite3 *db = NULL;
    char *err_msg = NULL;
    msg_t msg;
    time_t ptime;
    char pestime[100] = {0};

                                                   /* 对server_addr_in  结构进行赋值  */
    bzero(&sin,sizeof(struct sockaddr_in));        /* 先清零 */
    sin.sin_family=AF_INET;                 
    sin.sin_addr.s_addr=htonl(INADDR_ANY);         //表示接受任何ip地址   将ip地址转换成网络字节序
    sin.sin_port=htons(PORT);                      //将端口号转换成网络字节序

                                                   /* 调用socket函数创建一个TCP协议套接口 */
    if((lfd=socket(AF_INET,SOCK_STREAM,0))==-1)    // AF_INET:IPV4;SOCK_STREAM:TCP
    {
        fprintf(stderr,"Socket error:%s\n\a",strerror(errno));
        exit(1);
    }


    /*设置套接字选项 使用默认选项*/
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    /* 调用bind函数 将serer_addr结构绑定到sockfd上  */
    if(bind(lfd,(struct sockaddr *)(&sin),sizeof(struct sockaddr))==-1)
    {
        fprintf(stderr,"Bind error:%s\n\a",strerror(errno));
        exit(1);
    }


    /* 开始监听端口   等待客户的请求 */
    if(listen(lfd,20)==-1)
    {
        fprintf(stderr,"Listen error:%s\n\a",strerror(errno));
        exit(1);
    }

    printf("Accepting connections .......\n");

    maxfd = lfd;                                /*对最大文件描述符进行初始化*/
    maxi = -1;

    /*初始化客户端连接描述符集合*/
    for(i = 0;i < FD_SETSIZE;i++)
    {
        client[i] = -1;
    }

    FD_ZERO(&allset);                     /* 清空文件描述符集合 */
    FD_SET(lfd,&allset);                    /* 将监听字设置在集合内 */


    int rc = sqlite3_open("chat_room.db",&db);
    if(rc != SQLITE_OK)
    {
        fprintf(stderr,"open database failed %s\n",sqlite3_errmsg(db));
    }

    char sql_create_user_info[256] = {0};            //保存用户信息
    sprintf(sql_create_user_info,"create table user_info(id INTEGER,name TEXT,password TEXT,primary key(id));");
    sqlite3_exec(db,sql_create_user_info,NULL,0,&err_msg);

    char sql_create_log_info[256] = {0};             //保存已登录用户
    sprintf(sql_create_log_info,"create table log_info(id INTEGER,name TEXT,connectfd INTEGER,primary key(id));");
    sqlite3_exec(db,sql_create_log_info,NULL,0,&err_msg);

    char sql_create_record[256] = {0};               //保存服务器运行记录
    sprintf(sql_create_record,"create table record(id INTEGER,name TEXT,size TEXT,target TEXT,msg TEXT,time TEXT,primary key(id));");
    sqlite3_exec(db,sql_create_record,NULL,0,&err_msg);

    /* 开始服务程序的死循环 */
    while(1)
    {
        rset = allset;
        /*得到当前可以读的文件描述符数*/
        rdy = select(maxfd + 1, &rset, NULL, NULL, NULL);

            if(FD_ISSET(lfd, &rset))
            {
                addr_len = sizeof(sin);

                /* 接受客户端的请求 */
                if((cfd=accept(lfd,(struct sockaddr *)(&cin),&addr_len))==-1)
                {
                    fprintf(stderr,"Accept error:%s\n\a",strerror(errno));
                    exit(1);
                }

                /*查找一个空闲位置*/
                for(i = 0; i<FD_SETSIZE; i++)
                {       //printf("%d\t",client[i]);
                    if(client[i] <= 0)
                    {
                        client[i] = cfd;   /* 将处理该客户端的连接套接字设置到该位置 */
                        break;
                    }
                }

                /* 太多的客户端连接   服务器拒绝俄请求  跳出循环 */
                if(i == FD_SETSIZE)
                {
                    printf("too many clients");
                    exit(1);
                }

                FD_SET(cfd, &allset);     /* 设置连接集合 */

                if(cfd > maxfd)                  /* 新的连接描述符 */
                {
                    maxfd = cfd;
                }

                if(i > maxi)
                {
                    maxi = i;
                }

                if(--rdy <= 0)                /* 减少一个连接描述符 */
                {
                    continue;
                }

            }
        /* 对每一个连接描述符做处理 */
        for(i = 0;i< FD_SETSIZE;i++)
        {   
            if((sfd = client[i]) < 0)
            {
                continue;
            }

            if(FD_ISSET(sfd, &rset))
            {
                n = read(sfd,&msg,sizeof(msg_t));

                if(n == 0)                                //客户端异常退出
                {                                     
                    printf("the other side has been closed. \n");              
                    char sql_quit[256];                   //删除登录状态
                    sprintf(sql_quit, "delete from log_info where connectfd = %d;", sfd);
                    rc = sqlite3_exec(db,sql_quit,NULL,0,&err_msg);
                    if(rc != SQLITE_OK)
                    {    
                        fprintf(stderr,"%s",err_msg);
                    }

                    time(&ptime);                         //保存记录
                    strcpy(pestime,ctime(&ptime));
                    rc = sqlite3_exec(db, sql_quit, NULL, 0, &err_msg);
                    sprintf(sql_quit, "insert into record (id,name,size,time) values(%d,'%s','%s','%s');",msg.id,msg.name,"下线",pestime); 

                    rc = sqlite3_exec(db, sql_quit, NULL, 0, &err_msg);
                    
                    memset(&msg,0,sizeof(msg_t));
                    
                    fflush(stdout);                                    /* 刷新 输出终端 */
                    close(sfd);

                    FD_CLR(sfd, &allset);                        /*清空连接描述符数组*/
                    client[i] = -1;
                }

                else
                {
                    msg_handle(&msg,sfd);
                    /* 谐函数出错 */
                    if(n == 1)
                    {
                        exit(1);
                    }
                }

                /*如果没有可以读的套接字   退出循环*/
                if(--rdy <= 0)
                {
                    break;
                }


            }
        }

    }

    close(lfd);       /* 关闭链接套接字 */
    return 0;
}

