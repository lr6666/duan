#include "../../include/myhead.h"

/****************************************
Function:        static int call_back(void *,int,char **,char **)
Description:     查找表中数据并返回
Calls:           no
Call By:         int msg_handle(msg_t *,int)
Input:           void *not_used,int argc,char **argv,char **col_name
Output:          str_return
return:          no
Others:          no
****************************************/

extern char str_return[100];

static int call_back(void * not_used,int argc,char **argv,char **col_name)  //调用函数查找表内信息
{
    int i = 0;
    int count = 0;

    for(i = 0;i < argc;i++)
    {
        count = strlen(str_return);
        sprintf(str_return + count,"%s\t",argv[i]?argv[i]:"NULL");
    }	

    return 0;
}

/****************************************
Function:        int msg_handle(msg_t *,int)
Description:     处理客户端的指令,消息
Calls:           static int call_back(void *,int,char **,char **)
                 int block_read(int,char *,int)
                 int block_write(int,char *,int)
Call By:         int main() 
Input:           msg_t *msg,int connectfd
Output:          no
return:          no
Others:          no
****************************************/

int msg_handle(msg_t *msg,int connectfd)                     //处理客户端信息      
{
    time_t ptime;                                            //获取时间
    char pestime[30] = {0};                                  //当前时间 
    
    sqlite3 *db = NULL;                                      //数据库  
    char *err_msg = NULL;                                    //错误信息
    int tgt_confd = 0;                                       //接收方connectfd
    
    int id = 0;                                              //生成的id号 

    int rc = sqlite3_open("chat_room.db",&db);               //打开数据库
    if(rc != SQLITE_OK)
    {
        fprintf(stderr,"open database failed %s\n",sqlite3_errmsg(db));
    }

    if(LOG == msg->action)                                         //指令:登录
    {
        char sql_log[256] = {0};                                   //该用户是否存在  
        sprintf(sql_log, "select password from user_info where id = %d;", msg->id); 
        memset(str_return, 0, sizeof(str_return));
        int rc = sqlite3_exec(db, sql_log, call_back, 0, &err_msg);

        if(rc != SQLITE_OK)
        {
            msg->action = LOG_FAIL;
            write(connectfd,msg,sizeof(msg_t));                    //查找失败
            return ERROR;
        }

        if(0 == strlen(str_return))                                //用户不存在 
        {
            msg->action = USER_ERROR;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        if((0 != strncmp(str_return, msg->password, strlen(str_return) - 1)) || (strlen(msg->password) != (strlen(str_return) - 1)))
        {
            msg->action = PSWD_ERR;                                //密码错误  
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        sprintf(sql_log, "select * from log_info where id = %d;", msg->id);   //是否已登录
        memset(str_return, 0, sizeof(str_return));
        rc = sqlite3_exec(db, sql_log, call_back, 0, &err_msg);

        if(rc != SQLITE_OK)                                                   //查找失败
        {
            msg->action = LOG_FAIL;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        if(0 != strlen(str_return))
        {
            msg->action = RE_LOG;
            write(connectfd,msg,sizeof(msg_t));                               //用户已登录 
            return ERROR;
        }

        sprintf(sql_log, "select name from user_info where id = %d;", msg->id);   //该id的用户名 
        memset(str_return, 0, sizeof(str_return));
        rc = sqlite3_exec(db, sql_log, call_back, 0, &err_msg);
        
        memset(msg->name,0,sizeof(msg->name));
        strncpy(msg->name,str_return,strlen(str_return) - 1);                   
        
        sprintf(sql_log,"insert into log_info(id,name,connectfd) values(%d,'%s',%d);",msg->id,msg->name,connectfd); 
        rc = sqlite3_exec(db,sql_log,NULL,0,&err_msg);                            //插入已登录用户表

        if(rc != SQLITE_OK)
        {
            msg->action = LOG_FAIL;
            write(connectfd,msg,sizeof(msg_t));                                   //绑定connectfd失败
            return ERROR;
        }
        
        msg->action = LOG_SUCC;                                                   //返回登录成功信息
        write(connectfd,msg,sizeof(msg_t));

        time(&ptime);
        strcpy(pestime,ctime(&ptime));                                            //获取时间
        char sql_log_record[256];
        sprintf(sql_log_record, "insert into record (id,name,size,time) values(%d,'%s','%s','%s');",msg->id,msg->name,"登录",pestime); 

        rc = sqlite3_exec(db, sql_log_record, NULL, 0, &err_msg);                  //保存记录
        return SUCC;
    }

    else if(REGISTER == msg->action)                    //指令:注册
    {   
        strcpy(str_return,"hello");                     //随意赋值,使不为空 

        while(0 != strlen(str_return))                   
        {
            int i = 0;
            int num = 0;
            id = 0;                                     //id 
            srand((unsigned)time(NULL));                //随机生成数字
            for(i = 0;i < 8;i++)
            {
                num = (rand()%10);
                id = id * 10 + num;
            }

            char sql_id[256] = {0};                     //该id是否已存在
            sprintf(sql_id, "select name from user_info where id = %d;",id); 
            memset(str_return, 0, sizeof(str_return));
            rc = sqlite3_exec(db, sql_id, call_back, 0, &err_msg);
        }
        
        if(strcmp(msg->name,"admin") == 0)              //管理员专属id 
        {
            id = 666666;    
        }    
        
        char sql_register[256] = {0};                   //保存资料
        sprintf(sql_register, "insert into user_info(id,name,password) values(%d,'%s','%s');",id,msg->name,msg->password); 
        int rc = sqlite3_exec(db, sql_register, NULL, 0, &err_msg);
        
        if(rc == SQLITE_CONSTRAINT)                     //已被注册   
        {
            msg->action = RE_REG;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        else if(rc != SQLITE_OK)                        //查找失败
        {
            msg->action = REG_FAIL;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        msg->action = REG_SUCC;                         //返回注册成功与id号  
        msg->id = id;
        write(connectfd,msg,sizeof(msg_t));
        
        time(&ptime);
        strcpy(pestime,ctime(&ptime));
        
        char sql_register_record[256];                   //保存记录
        sprintf(sql_register_record, "insert into record (id,name,size,time) values(%d,'%s','%s','%s');",msg->id,msg->name,"注册",pestime); 
        rc = sqlite3_exec(db, sql_register_record, NULL, 0, &err_msg);
        return SUCC;
    }	

    else if(SHOW_ONLINE == msg->action)
    {
        char * sql_show = "select name from log_info;";                //查找所有在线用户  
        memset(str_return, 0, sizeof(str_return));
        int rc = sqlite3_exec(db, sql_show, call_back, 0, &err_msg);    

        if(rc != SQLITE_OK)                                            //查找失败
        {
            msg->action = SHOW_ERR;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        memset(msg->msg,0,sizeof(msg->msg));
        strcpy(msg->msg,str_return);
        msg->action = SHOW_ONLINE;
        write(connectfd,msg,sizeof(msg_t));                             //返回在线用户
        return SUCC;
    }	

    else if(TRAN == msg->action)                                        //指令:发送文件
    {
        char sql_tran[256];                                             //接收用户是否存在
        sprintf(sql_tran, "select name from user_info where name = '%s';", msg->target); 

        memset(str_return, 0, sizeof(str_return));
        int rc = sqlite3_exec(db, sql_tran, call_back, 0, &err_msg);

        if(rc != SQLITE_OK)                                             //查找失败
        {
            msg->action = TRAN_FAIL;
            write(connectfd,msg,sizeof(msg_t));
            //continue;
            return ERROR;
        }

        if(0 == strlen(str_return))                                     //用户不存在
        {
            msg->action = USER_ERROR;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        sprintf(sql_tran, "select connectfd from log_info where name = '%s';", msg->target); //查找id
        memset(str_return, 0, sizeof(str_return));
        rc = sqlite3_exec(db,sql_tran,call_back,0,&err_msg);

        if(rc != SQLITE_OK)                                                 //查找失败
        {
            msg->action = TRAN_FAIL;
            write(connectfd,msg,sizeof(msg_t));                    
            return ERROR;
        }

        if(0 == strlen(str_return))                                         //用户不在线 
        {
            msg->action = OFFLINE;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }
        
        if(strcmp(msg->msg,"wanttotran") == 0)
        {
            tgt_confd = atoi(str_return);                                       //征询接受方 
            msg->action = TRAN_ASK;
            write(tgt_confd,msg,sizeof(msg_t));
        }    
        
        if(strcmp(msg->msg,"accept") == 0)
        {
            tgt_confd = atoi(str_return);                                       //同意接收文件 
            msg->action = TRAN_ACCEPT;
            write(tgt_confd,msg,sizeof(msg_t));
        }    
            
        if(strcmp(msg->msg,"refuse") == 0)
        {
            tgt_confd = atoi(str_return);                                       //拒绝接收文件 
            msg->action = TRAN_REF;
            write(tgt_confd,msg,sizeof(msg_t));
        }    
        
        if(strcmp(msg->msg,"cancle") == 0)
        {
            tgt_confd = atoi(str_return);                                       //取消发送文件 
            msg->action = TRAN_CANCLE;
            write(tgt_confd,msg,sizeof(msg_t));
        }    
        
        if(strcmp(msg->msg,"tranfile") == 0)
        {
            char *data = NULL;                                                  //分配空间接收文件
            data = (char *)malloc(msg->flen + 1);     
            memset(data,0,msg->flen + 1);

            block_read(connectfd,data,msg->flen);                               //接收 
            tgt_confd = atoi(str_return);                                       //转发给接收方 
            msg->action = TRAN_REC;
            write(tgt_confd,msg,sizeof(msg_t));
            block_write(tgt_confd,data,msg->flen);
            free(data);

            msg->action = TRAN_SUCC;                                            //返回发送成功 
            write(connectfd,msg,sizeof(msg_t));                                 

            time(&ptime);                                                       //保存记录
            strcpy(pestime,ctime(&ptime));
            char sql_tran_record[256];
            sprintf(sql_tran_record, "insert into record (name,size,target,time) values('%s','%s','%s','%s');",msg->name,"发送文件",msg->target,pestime); 

            rc = sqlite3_exec(db, sql_tran_record, NULL, 0, &err_msg);
            return SUCC;
        }
    }	

    else if(WHISPER == msg->action)                                         //指令:私密
    {
        char sql_whisper[256];                                              //用户是否存在   
        sprintf(sql_whisper, "select name from user_info where name = '%s';", msg->target); 
        memset(str_return, 0, sizeof(str_return));
        int rc = sqlite3_exec(db, sql_whisper, call_back, 0, &err_msg);

        if(rc != SQLITE_OK)                                                 //查找失败
        {
            msg->action = SEND_FAIL;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        if(0 == strlen(str_return))                                         //用户不存在
        {
            msg->action = USER_ERROR;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        sprintf(sql_whisper, "select connectfd from log_info where name = '%s';", msg->target); //查找id 
        memset(str_return, 0, sizeof(str_return));
        rc = sqlite3_exec(db, sql_whisper, call_back, 0, &err_msg);

        if(rc != SQLITE_OK)                                //查找失败                     
        {
            msg->action = SEND_FAIL;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        
        }
        if(strcmp(msg->msg,"tranfile") == 0)
        {
            char *data = NULL;                                                  //分配空间接收文件
            data = (char *)malloc(msg->flen + 1);                          
            memset(data,0,msg->flen + 1);

            block_read(connectfd,data,msg->flen);                               //接收 

            tgt_confd = atoi(str_return);                                       //转发给接收方 
            //msg->action = TRAN;
            //write(tgt_confd,msg,sizeof(msg_t));
            block_write(tgt_confd,data,msg->flen);

            free(data);

            msg->action = TRAN_SUCC;                                            //返回发送成功 
            write(connectfd,msg,sizeof(msg_t));                                 

            time(&ptime);                                                       //保存记录
            strcpy(pestime,ctime(&ptime));
            char sql_tran_record[256];
            sprintf(sql_tran_record, "insert into record (name,size,target,time) values('%s','%s','%s','%s');",msg->name,"发送文件",msg->target,pestime); 

            rc = sqlite3_exec(db, sql_tran_record, NULL, 0, &err_msg);
            return SUCC;
        }
    }	

    else if(WHISPER == msg->action)                                         //指令:私密
    {
        char sql_whisper[256];                                              //用户是否存在   
        sprintf(sql_whisper, "select name from user_info where name = '%s';", msg->target); 
        memset(str_return, 0, sizeof(str_return));
        int rc = sqlite3_exec(db, sql_whisper, call_back, 0, &err_msg);

        if(rc != SQLITE_OK)                                                 //查找失败
        {
            msg->action = SEND_FAIL;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        if(0 == strlen(str_return))                                         //用户不存在
        {
            msg->action = USER_ERROR;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        sprintf(sql_whisper, "select connectfd from log_info where name = '%s';", msg->target); //查找id 
        memset(str_return, 0, sizeof(str_return));
        rc = sqlite3_exec(db, sql_whisper, call_back, 0, &err_msg);

        if(rc != SQLITE_OK)                                //查找失败                     
        {
            msg->action = SEND_FAIL;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        if(0 == strlen(str_return))                        //不在线 
        {
            msg->action = OFFLINE;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }
        
        tgt_confd = atoi(str_return);                      //发送给接收方
        write(tgt_confd,msg,sizeof(msg_t));

        msg->action = WHISPER_SUCC;                        //返回发送成功
        write(connectfd,msg,sizeof(msg_t));

        time(&ptime);                                      //保存记录
        strcpy(pestime,ctime(&ptime));                      
        char sql_whsp_record[256];
        sprintf(sql_whsp_record, "insert into record (id,name,size,target,msg,time) values(%d,'%s','%s','%s','%s','%s');",msg->id,msg->name,"私密",msg->target,msg->msg,pestime); 

        rc = sqlite3_exec(db, sql_whsp_record, NULL, 0, &err_msg);
        return SUCC;
    }	

    else if(CHAT == msg->action)                                      //指令:群发
    {
        int i = 0;
        int count = 0;
        char temp[4];

        char * sql_chat = "select connectfd from log_info;";         //查找所有在线用户

        memset(str_return, 0, sizeof(str_return));
        int rc = sqlite3_exec(db, sql_chat, call_back, 0, &err_msg);

        if(rc != SQLITE_OK)                                          //查找失败
        {
            msg->action = SEND_FAIL;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        msg->action = CHAT;

        while(0 != *(str_return + count))                           //读取每一个connectfd
        {
            if('\t' == *(str_return + count))
            {
                tgt_confd = atoi(temp);
                if(tgt_confd != connectfd)                          //不能给自己发送 
                {
                    write(tgt_confd,msg,sizeof(msg_t));
                }
                memset(temp, 0, sizeof(temp));                      //初始化   
                i = 0;
            }
            strncpy(temp + i, str_return + count, 1);
            i++;
            count++;
        }
        msg->action = CHAT_SUCC;                                    //返回发送成功    
        write(connectfd,msg,sizeof(msg_t));

        time(&ptime);                                               //保存记录 
        strcpy(pestime,ctime(&ptime)); 
        char sql_chat_record[256];
        sprintf(sql_chat_record, "insert into record (name,size,target,msg,time) values('%s','%s','%s','%s','%s');",msg->name,"群发","all user",msg->msg,pestime); 

        rc = sqlite3_exec(db, sql_chat_record, NULL, 0, &err_msg);
        return SUCC;
    }	

    else if(SLIENCE == msg->action)                                 //指令: 禁言
    {
        char sql_slience[256];                                      //用户是否存在      
        sprintf(sql_slience, "select name from user_info where name = '%s';", msg->target); 
        memset(str_return, 0, sizeof(str_return));
        int rc = sqlite3_exec(db, sql_slience, call_back, 0, &err_msg);

        if(rc != SQLITE_OK)                                         //查找失败
        {
            msg->action = SLIENCE_FAIL;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        if(0 == strlen(str_return))                                 //用户不存在
        {
            msg->action = USER_ERROR;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        sprintf(sql_slience, "select connectfd from log_info where name = '%s';", msg->target); //查找id
        memset(str_return, 0, sizeof(str_return));
        rc = sqlite3_exec(db, sql_slience, call_back, 0, &err_msg);

        if(rc != SQLITE_OK)                                         //查找失败
        {
            msg->action = SLIENCE_FAIL;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        if(0 == strlen(str_return))                                 //不在线
        {
            msg->action = OFFLINE;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        tgt_confd = atoi(str_return);                               //发送禁言指令
        msg->action = SLIENCE;
        write(tgt_confd,msg,sizeof(msg_t));
        
        msg->action = SLIENCE_SUCC;                                 //返回禁言成功
        write(connectfd,msg,sizeof(msg_t));

        time(&ptime);                                               //保存记录
        strcpy(pestime,ctime(&ptime));
        char sql_slience_record[256];
        sprintf(sql_slience_record, "insert into record (id,name,size,target,time) values(%d,'%s','%s','%s','%s');",msg->id,msg->name,"禁言",msg->target,pestime); 
        rc = sqlite3_exec(db, sql_slience_record, NULL, 0, &err_msg);
        return SUCC;
    }	

    else if(REQUIRE == msg->action)                                 //指令: 请求解禁
    {
        char sql_require[256];                                      //管理员是否存在
        sprintf(sql_require, "select name from user_info where name = '%s';", msg->target); 
        memset(str_return, 0, sizeof(str_return));
        int rc = sqlite3_exec(db, sql_require, call_back, 0, &err_msg);

        if(rc != SQLITE_OK)                                         //查找失败
        {
            msg->action = REQUIRE_FAIL;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        if(0 == strlen(str_return))                                 //管理员不存在
        {
            msg->action = USER_ERROR;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        sprintf(sql_require, "select connectfd from log_info where name = '%s';", msg->target); 
        memset(str_return, 0, sizeof(str_return));                  //查找connectfd 
        rc = sqlite3_exec(db, sql_require, call_back, 0, &err_msg);

        if(rc != SQLITE_OK)                                         //查找失败
        {
            msg->action = REQUIRE_FAIL;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        if(0 == strlen(str_return))                                 //不在线
        {
            msg->action = OFFLINE;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        tgt_confd = atoi(str_return);                               //向管理员发送提示
        msg->action = REQUIRE;
        write(tgt_confd,msg,sizeof(msg_t));                    

        time(&ptime);                                               //保存记录  
        strcpy(pestime,ctime(&ptime));
        char sql_require_record[256];
        sprintf(sql_require_record, "insert into record (id,name,size,time) values(%d,'%s','%s','%s');",msg->id,msg->name,"请求解禁",pestime); 
        rc = sqlite3_exec(db, sql_require_record, NULL, 0, &err_msg);
        return SUCC;
    }	

    else if(SAY== msg->action)                                      //指令: 解禁 
    {
        char sql_say[256];                                          //用户是否存在 
        sprintf(sql_say, "select name from user_info where name = '%s';", msg->target); 
        memset(str_return, 0, sizeof(str_return));
        int rc = sqlite3_exec(db, sql_say, call_back, 0, &err_msg);

        if(rc != SQLITE_OK)                                         //查找失败   
        {
            msg->action = SAY_FAIL;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        if(0 == strlen(str_return))                                 //不在线 
        {
            msg->action = USER_ERROR;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        sprintf(sql_say, "select connectfd from log_info where name = '%s';", msg->target); 
        memset(str_return, 0, sizeof(str_return));                  //查找connectfd 
        rc = sqlite3_exec(db, sql_say, call_back, 0, &err_msg);

        if(rc != SQLITE_OK)                                         //查找失败
        {
            msg->action = SAY_FAIL;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        if(0 == strlen(str_return))                                 //不在线
        {
            msg->action = OFFLINE;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        tgt_confd = atoi(str_return);                               //向接收发发送指令
        msg->action = SAY;
        write(tgt_confd,msg,sizeof(msg_t));

        msg->action = SAY_SUCC;                                     //返回解禁成功
        write(connectfd,msg,sizeof(msg_t));
        
        time(&ptime);
        strcpy(pestime,ctime(&ptime));

        char sql_say_record[256];                                   //保存记录
        sprintf(sql_say_record, "insert into record (id,name,size,target,time) values(%d,'%s','%s','%s','%s');",msg->id,msg->name,"解禁",msg->target,pestime); 
        rc = sqlite3_exec(db, sql_say_record, NULL, 0, &err_msg);
        return SUCC;
    }	

    else if(GETOUT == msg->action)                        //指令: 踢出聊天室
    {
        char sql_getout[256];                             //用户是否存在
        sprintf(sql_getout, "select name from user_info where name = '%s';", msg->target); 
        memset(str_return, 0, sizeof(str_return));
        int rc = sqlite3_exec(db, sql_getout, call_back, 0, &err_msg);

        if(rc != SQLITE_OK)                               //查找失败 
        {
            msg->action = GETOUT_FAIL;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        if(0 == strlen(str_return))                       //用户不存在
        {
            msg->action = USER_ERROR;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        sprintf(sql_getout, "select connectfd from log_info where name = '%s';", msg->target); 
        memset(str_return, 0, sizeof(str_return));                    //查找connectfd
        rc = sqlite3_exec(db, sql_getout, call_back, 0, &err_msg);

        if(rc != SQLITE_OK)                                           //查找失败
        {
            msg->action = GETOUT_FAIL;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        if(0 == strlen(str_return))                                   //不在线
        {
            msg->action = OFFLINE;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        char sql_quit[256] = {0};                                     //移处登录状态
        sprintf(sql_quit, "delete from log_info where connectfd = %d;", atoi(str_return));
        rc = sqlite3_exec(db,sql_quit,NULL,0,&err_msg);

        if(rc != SQLITE_OK)                                           //移除失败
        {
            msg->action = GETOUT_FAIL;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        msg->action = GETOUT_SUCC;                                    //返回踢出成功
        write(connectfd,msg,sizeof(msg_t));

        tgt_confd = atoi(str_return);                                 //向接受方发送指令
        msg->action = GETOUT;
        write(tgt_confd,msg,sizeof(msg_t));

        time(&ptime);                                                 //保存记录 
        strcpy(pestime,ctime(&ptime));
        char sql_getout_record[256];
        sprintf(sql_getout_record, "insert into record (id,name,size,target,time) values(%d,'%s','%s','%s','%s');",msg->name,"踢出聊天室",msg->target,pestime); 
        rc = sqlite3_exec(db, sql_getout_record, NULL, 0, &err_msg);

        time(&ptime);
        strcpy(pestime,ctime(&ptime));

        sprintf(sql_getout_record, "insert into record (id,name,size,time) values(%d,'%s','%s','%s');",msg->id,msg->target,"下线",pestime); 
        memset(str_return, 0, sizeof(str_return));                   //保存下线记录  
        rc = sqlite3_exec(db, sql_getout_record, NULL, 0, &err_msg);
        return SUCC;
    }	

    else if(QUIT_SYSTEM == msg->action)                              //指令:退出系统
    {
        char sql_quit[100] = {0};                                    //删除登录状态
        sprintf(sql_quit, "delete from log_info where id = %d;", msg->id);
        rc = sqlite3_exec(db,sql_quit,NULL,0,&err_msg);

        if(rc != SQLITE_OK)                                          //操作失败 
        {
            msg->action = QUIT_FAIL;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        else                                                         //返回退出成功
        {	
            msg->action = QUIT_SYSTEM;
            write(connectfd,msg,sizeof(msg_t));
            return SUCC;
        }
    }	

    else if(CH_PASSWD == msg->action)                     //指令:修改密码
    {   
        char sql_ch_passwd[256];                          //用户是否存在
        sprintf(sql_ch_passwd, "select name from user_info where id = %d;", msg->id); 
        memset(str_return, 0, sizeof(str_return));
        int rc = sqlite3_exec(db, sql_ch_passwd, call_back, 0, &err_msg);

        if(rc != SQLITE_OK)                               //查找失败 
        {
            msg->action = CHANGE_FAIL;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        if(0 == strlen(str_return))                       //用户不存在
        {
            msg->action = USER_ERROR;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }
        
        sprintf(sql_ch_passwd, "update user_info set password = '%s' where id = %d;",msg->password,msg->id); 
        rc = sqlite3_exec(db, sql_ch_passwd, NULL, 0, &err_msg);
        
        if(rc != SQLITE_OK)                                //修改失败
        {
            msg->action = CHANGE_FAIL;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        msg->action = GETOUT;                              //返回成功修改信息  
        strcpy(msg->msg,"change");
        write(connectfd,msg,sizeof(msg_t));
        
        time(&ptime);
        strcpy(pestime,ctime(&ptime));
        
        char sql_register_record[256];                     //保存记录
        sprintf(sql_register_record, "insert into record (id,name,size,time) values(%d,'%s','%s','%s');",msg->id,msg->name,"修改密码",pestime); 
        rc = sqlite3_exec(db, sql_register_record, NULL, 0, &err_msg);

        char sql_quit[256] = {0};                          //移处登录状态
        sprintf(sql_quit, "delete from log_info where id = %d;", msg->id);
        rc = sqlite3_exec(db,sql_quit,NULL,0,&err_msg);
        
        return SUCC;
    }	
    
    else if(CH_NAME == msg->action)                       //指令:修改用户名
    {   
        char sql_ch_name[256];                            //用户是否存在
        sprintf(sql_ch_name, "select name from user_info where id = %d;", msg->id); 
        memset(str_return, 0, sizeof(str_return));
        int rc = sqlite3_exec(db, sql_ch_name, call_back, 0, &err_msg);

        if(rc != SQLITE_OK)                               //查找失败 
        {
            msg->action = CHANGE_FAIL;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        if(0 == strlen(str_return))                        //用户不存在
        {
            msg->action = USER_ERROR;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }
        
        sprintf(sql_ch_name, "update user_info set name = '%s' where id = %d;",msg->name,msg->id); 
        rc = sqlite3_exec(db, sql_ch_name, NULL, 0, &err_msg);
        
        if(rc != SQLITE_OK)                                //修改失败
        {
            msg->action = CHANGE_FAIL;
            write(connectfd,msg,sizeof(msg_t));
            return ERROR;
        }

        msg->action = GETOUT;                              //返回成功修改信息  
        strcpy(msg->msg,"change");
        write(connectfd,msg,sizeof(msg_t));
        
        time(&ptime);
        strcpy(pestime,ctime(&ptime));
        
        char sql_register_record[256];                     //保存记录
        sprintf(sql_register_record, "insert into record (id,name,size,time) values(%d,'%s','%s','%s');",msg->id,msg->name,"修改用户名",pestime); 
        rc = sqlite3_exec(db, sql_register_record, NULL, 0, &err_msg);

        char sql_quit[256] = {0};                          //移处登录状态
        sprintf(sql_quit, "delete from log_info where id = %d;",msg->id);
        rc = sqlite3_exec(db,sql_quit,NULL,0,&err_msg);
        
        return SUCC;
    }
}
