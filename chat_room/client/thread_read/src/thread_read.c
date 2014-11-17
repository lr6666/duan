#include "../../include/myhead.h"

/****************************************
Function:        void *thread_read(void *arg)
Description:     读取服务器消息,指令
Calls:           int block_read(int,char *,int,char *)
                 void log_or_register()
                 void after_log()
Call By:         int main() 
Input:           void *arg
Output:          no
return:          NULL
Others:          no
****************************************/

extern int is_log;
extern int is_slience;
extern char present_user[NAME_LEN];
extern int id;
extern int is_tran;
extern char tran_target[NAME_LEN];
extern char file_name[NAME_LEN];

void *thread_read(void *arg)         //接收服务器消息
{
	int sockfd = *((int *)arg);      //连接描述符
    msg_t msg;                       //结构体接收消息
	time_t ptime;                    //获取当前时间
	char pestime[50] = {0};          //当前时间

	sqlite3 *db = NULL;              //数据库
	char *err_msg = NULL;            //错误消息 

	int rc = sqlite3_open("chat_room.db",&db);           //打开数据库保存用户消息
	if(rc != SQLITE_OK)
	{
		printf("数据库异常,您的记录将无法保存!\n");	
	}

	while(1)
	{
		memset(&msg,0,sizeof(msg));                      //初始化
		
		int m = read(sockfd,&msg,sizeof(msg));           //读取服务器消息
		
        switch(msg.action)
		{	
			case LOG_SUCC:                               //登录成功
			{
				printf("\n登录成功!\n");
				is_log = YES;                            //登录状态: 已登录
                
                strcpy(present_user,msg.name);           //当前用户名
                id = msg.id;                             //当前id

                char sql_create_user_record[256] = {0};  //创建表保存消息
                sprintf(sql_create_user_record,"create table %s(id INTEGER,name TEXT,size TEXT,target TEXT,msg TEXT,time TEXT,primary key(size,time));",msg.name);
				int n = sqlite3_exec(db,sql_create_user_record,NULL,0,&err_msg);
                
                time(&ptime);
				strcpy(pestime,ctime(&ptime));          
 
  				char sql_log[256] = {0};                 //保存登录成功信息
				sprintf(sql_log,"insert into %s (id,name,size,time) values(%d,'%s','%s','%s');",present_user,msg.id,present_user,"登录成功",pestime);
                sqlite3_exec(db,sql_log,NULL,0,&err_msg);

				sleep(1);
				system("reset");
				after_log();                             //进入登录后页面
				break;
			}
			
			case LOG_FAIL:              
			{
				printf("\n登录失败!\n");
				break;
			}
			
			case PSWD_ERR:
			{
				printf("\n密码错误!\n");
				break;
			}
			
			case RE_LOG:
			{
				printf("\n该用户已登录,请勿重复登录!\n");
				break;
			}
			
			case REG_SUCC:
			{
				printf("\n注册成功!\n");
                if(msg.id == 666666)                            //管理员专用id
                {
                    printf("亲爱的管理员,该id为您专用id: 666666\n");    
                    break;
                }    
                    
                printf("您的id号为: \033[40;31m%d\033[0m,请您谨记!\n",msg.id);    //id号
				break;
			}
			
			case REG_FAIL:
			{
				printf("\n注册失败!\n");
				break;
			}
			
			case RE_REG:
			{
				printf("\n注册失败,用户已存在!\n");
				break;
			}
			
			case SLIENCE:
			{
				printf("\n你倒霉了,你惹到管理员了,现在你被禁言,不能发言了!\n");
				printf("你可以输入require请求管理员解禁!\n");

                time(&ptime);
				strcpy(pestime,ctime(&ptime));
 
				char sql_beslience[256] = {0};      //保存在数据库中
				sprintf(sql_beslience,"insert into %s(id,name,size,time) values(%d,'%s','%s','%s');",present_user,msg.id,present_user,"被管理员禁言",pestime);
                sqlite3_exec(db,sql_beslience,NULL,0,&err_msg);
				
				is_slience = YES;                   //禁言状态: 是
				break;
			}
			
			case SLIENCE_FAIL:                      //禁言用户失败    
			{
				printf("\n%s禁言失败!\n",msg.target);
                
				time(&ptime);
				strcpy(pestime,ctime(&ptime));

				char sql_slience[256] = {0};       
				sprintf(sql_slience,"insert into %s (id,name,size,target,time) values(%d,'%s','%s','%s','%s');",present_user,msg.id,present_user,"禁言失败",msg.target,pestime);
                sqlite3_exec(db,sql_slience,NULL,0,&err_msg);
				
				break;
			}
		    
			case SLIENCE_SUCC:                             //禁言用户成功
			{
			    printf("\n%s禁言成功!\n",msg.target);
				
				time(&ptime);
				strcpy(pestime,ctime(&ptime));               

				char sql_slience[256] = {0};
				sprintf(sql_slience,"insert into %s (id,name,size,target,time) values(%d,'%s','%s','%s','%s');",present_user,msg.id,present_user,"禁言成功",msg.target,pestime);
                sqlite3_exec(db,sql_slience,NULL,0,&err_msg);
				
				break;
			}
		    
			case SAY:                               //被解禁
			{
				if(is_slience == YES)               //未被禁言不显示 
				{
					printf("\n管理员大大原谅你了,你可以自由发言啦!\n");

					time(&ptime);
					strcpy(pestime,ctime(&ptime));

					char sql_say[256] = {0};
					sprintf(sql_say,"insert into %s (id,name,size,time) values(%d,'%s','%s','%s');",present_user,msg.id,present_user,"被解禁",pestime);
					sqlite3_exec(db,sql_say,NULL,0,&err_msg);

					is_slience = NO;
				}
				break;
			}
			
			case SAY_FAIL:                              //解禁用户失败
			{
				printf("\n%s解禁失败!\n",msg.target);
				
				time(&ptime);
				strcpy(pestime,ctime(&ptime));

				char sql_say[256] = {0};
				sprintf(sql_say,"insert into %s (id,name,size,target,time) values(%d,'%s','%s','%s','%s');",present_user,msg.id,present_user,"解禁失败",msg.target,pestime);
				sqlite3_exec(db,sql_say,NULL,0,&err_msg);
				break;
			}

			case SAY_SUCC:                              //解禁用户
			{
				printf("\n%s解禁成功!\n",msg.target);
				
				time(&ptime);
				strcpy(pestime,ctime(&ptime));

				char sql_say[256] = {0};
				sprintf(sql_say,"insert into %s (id,name,size,target,time) values(%d,'%s','%s','%s','%s');",present_user,msg.id,present_user,"解禁成功",msg.target,pestime);
				sqlite3_exec(db,sql_say,NULL,0,&err_msg);
				
				break;
			}
			
			case REQUIRE:                               //用户请求解禁
			{
				printf("\n%s请求解禁!\n",msg.name);
				
				time(&ptime);
				strcpy(pestime,ctime(&ptime));

				char sql_require[256] = {0};
				sprintf(sql_require,"insert into %s (id,name,size,time) values(%d,'%s','%s','%s');",present_user,msg.id,present_user,"请求解禁",pestime);
				sqlite3_exec(db,sql_require,NULL,0,&err_msg);
				
				break;
			}
			
			case REQUIRE_FAIL:
			{
				printf("\n请耐心等待!\n");
				break;
			}
			
			case GETOUT:                                        //被踢出聊天室
			{
				if(strcmp(msg.msg,"change") == 0)               //修改密码或用户名被强制下线
                {
                    printf("\n修改成功,请重新登录!\n");
                    sleep(1);
                    system("reset");
                    is_log = NO;
                    log_or_register();
                    break;
                }    

                printf("\n\n\n你已被管理员踢出聊天室!\n\n");
				
				time(&ptime);
				strcpy(pestime,ctime(&ptime));

				char sql_getout[256] = {0};
				sprintf(sql_getout,"insert into %s (id,name,size,time) values(%d,'%s','%s','%s');",present_user,msg.id,present_user,"被踢出聊天室",pestime);
				sqlite3_exec(db,sql_getout,NULL,0,&err_msg);
				
				char sql_logoff[256] = {0};
				sprintf(sql_logoff,"insert into %s (id,name,size,time) values(%d,'%s','%s','%s');",present_user,msg.id,present_user,"下线",pestime);
				sqlite3_exec(db,sql_logoff,NULL,0,&err_msg);
				
				sleep(1);
				system("reset");
				is_log = NO;                         //登录状态:未登录
				log_or_register();                   //登录页面
				break;
			}
			
			case GETOUT_FAIL:
			{
				printf("\n踢出失败,请重试!\n");
				
				time(&ptime);
				strcpy(pestime,ctime(&ptime));

				char sql_getout[256] = {0};
				sprintf(sql_getout,"insert into %s (id,name,size,time) values(%d,'%s','%s','%s');",present_user,msg.id,present_user,"踢出失败",pestime);
				sqlite3_exec(db,sql_getout,NULL,0,&err_msg);
				break;
			}
			
            case GETOUT_SUCC:                        //成功将用户踢出 
			{
				printf("\n您已将 %s 踢出聊天室!\n",msg.target);
				
				time(&ptime);
				strcpy(pestime,ctime(&ptime));

				char sql_getout[256] = {0};
				sprintf(sql_getout,"insert into %s (id,name,size,target,time) values(%d,'%s','%s','%s','%s');",present_user,msg.id,present_user,"踢出成功",msg.target,pestime);
				sqlite3_exec(db,sql_getout,NULL,0,&err_msg);
				break;
			}
			
            case WHISPER:                            //接收私聊消息
			{
				time(&ptime);
				printf("\n%s",ctime(&ptime));
				
				if(strcmp(msg.msg,"SaD") == 0)
				{
				    printf("%s对你做了一张哭脸\n",msg.name);	
				}	
				
				else if(strcmp(msg.msg,"SmIlE") == 0)
				{
				    printf("%s对你做了一张笑脸\n",msg.name);	
				}	
				
				else
				{	
					printf("%s悄悄对你说:%s\n",msg.name,msg.msg);
				}
				break;
			}
			
			case WHISPER_SUCC:                            //私聊消息发送成功
			{
				time(&ptime);
				printf("\n%s",ctime(&ptime));
				
				if(strcmp(msg.msg,"SaD") == 0)            //表情（苦脸）
				{
				    printf("你对%s做了一张哭脸\n",msg.target);	
					
					time(&ptime);
					strcpy(pestime,ctime(&ptime));

					char sql_getout[256] = {0};
					sprintf(sql_getout,"insert into %s (id,name,size,target,time) values(%d,'%s','%s','%s','%s');",present_user,msg.id,present_user,"做了一个哭脸",msg.target,pestime);
					sqlite3_exec(db,sql_getout,NULL,0,&err_msg);
				}	
				
				else if(strcmp(msg.msg,"SmIlE") == 0)     //表情（笑脸）
				{
				    printf("你对%s做了一张笑脸\n",msg.target);	
					
					time(&ptime);
					strcpy(pestime,ctime(&ptime));

					char sql_getout[256] = {0};
					sprintf(sql_getout,"insert into %s (id,name,size,target,time) values(%d,'%s','%s','%s','%s');",present_user,msg.id,present_user,"做了一个笑脸",msg.target,pestime);
					sqlite3_exec(db,sql_getout,NULL,0,&err_msg);
				}	
				
				else
				{	
					printf("你悄悄对%s说:%s\n",msg.target,msg.msg);     //普通消息
					
					time(&ptime);
					strcpy(pestime,ctime(&ptime));

					char sql_getout[256] = {0};
					sprintf(sql_getout,"insert into %s (id,name,size,target,msg,time) values(%d,'%s','%s','%s','%s');",present_user,msg.id,present_user,"私密",msg.target,msg.msg,pestime);
					sqlite3_exec(db,sql_getout,NULL,0,&err_msg);
				}
				
				break;
			}
			
			case SEND_FAIL:
			{
				printf("\n消息发送失败!\n");
				break;
			}	
			
			case USER_ERROR:                           
			{
				printf("\n该用户不存在!\n");
				break;
			}
			
			case OFFLINE:
			{
				printf("\n该用户离线!\n");
				break;
			}
			
			case CHAT:                                                 //接收群发消息
			{
				time(&ptime);
				printf("\n%s",ctime(&ptime));
				
				if(strcmp(msg.msg,"SaD") == 0)                         //表情（哭脸） 
				{
				    printf("%s对大家做了一张哭脸\n",msg.name);	
					
					time(&ptime);
					strcpy(pestime,ctime(&ptime));

					char sql_getout[256] = {0};
					sprintf(sql_getout,"insert into %s (id,name,size,target,time) values(%d,'%s','%s','%s','%s');",present_user,msg.id,present_user,"做了一个哭脸","所有在线用户",pestime);
					sqlite3_exec(db,sql_getout,NULL,0,&err_msg);
				}	
				
				else if(strcmp(msg.msg,"SmIlE") == 0)                  //表情（笑脸）
				{
				    printf("%s对大家做了一张笑脸\n",msg.name);	
					
					time(&ptime);
					strcpy(pestime,ctime(&ptime));

					char sql_getout[256] = {0};
					sprintf(sql_getout,"insert into %s (id,name,size,target,time) values(%d,'%s','%s','%s','%s');",present_user,msg.id,present_user,"做了一个笑脸","所有在线用户",pestime);
					sqlite3_exec(db,sql_getout,NULL,0,&err_msg);
				}	
				
				else
				{	
					printf("%s对大家说:%s\n",msg.name,msg.msg);	
					
					time(&ptime);
					strcpy(pestime,ctime(&ptime));

					char sql_getout[256] = {0};
					sprintf(sql_getout,"insert into %s (id,name,size,target,msg,time) values(%d,'%s','%s','%s','%s','%s');",present_user,msg.id,present_user,"群发","所有在线用户",msg.msg,pestime);
					sqlite3_exec(db,sql_getout,NULL,0,&err_msg);
				}
				
				break;
			}
			
			case CHAT_SUCC:                                    //群发消息发送成功
			{
				time(&ptime);
				printf("\n%s",ctime(&ptime));
				
				if(strcmp(msg.msg,"SaD") == 0)
				{
				    printf("你对大家做了一张哭脸\n");	       //表情（哭脸）
					
					time(&ptime);
					strcpy(pestime,ctime(&ptime));

					char sql_getout[256] = {0};
					sprintf(sql_getout,"insert into %s (id,name,size,target,time) values(%d,'%s','%s','%s','%s');",present_user,msg.id,present_user,"做了一个哭脸","所有在线用户",pestime);
					sqlite3_exec(db,sql_getout,NULL,0,&err_msg);
				}	
				
				else if(strcmp(msg.msg,"SmIlE") == 0)          //表情（笑脸）
				{
				    printf("你对大家做了一张笑脸\n");	
					
					time(&ptime);
					strcpy(pestime,ctime(&ptime));

					char sql_getout[256] = {0};
					sprintf(sql_getout,"insert into %s (id,name,size,target,time) values(%d,'%s','%s','%s','%s');",present_user,msg.id,present_user,"做了一个笑脸","所有在线用户",pestime);
					sqlite3_exec(db,sql_getout,NULL,0,&err_msg);
				}	
				
				else
				{
					printf("你对大家说:%s\n",msg.msg);
					
					time(&ptime);
					strcpy(pestime,ctime(&ptime));

					char sql_getout[256] = {0};
					sprintf(sql_getout,"insert into %s (id,name,size,target,msg,time) values(%d,'%s','%s','%s','%s','%s');",present_user,msg.id,present_user,"群发","所有在线用户",msg.msg,pestime);
					sqlite3_exec(db,sql_getout,NULL,0,&err_msg);
				}
				
				break;
			}
			
			case SHOW_ONLINE:                   //查询在线用户
			{
				printf("\n在线用户:");
				printf("  %s\n",msg.msg);
				break;
			}
			
			case SHOW_ERR:                      //查询失败
			{
				printf("\nshow 失败!\n");
				break;
			}
			
            case TRAN_ASK:                                                //接收文件 
            {
                printf("\n用户 %s 向您发送了文件 %s,是否接收(yes/no)?\n",msg.name,msg.fname);
                strcpy(tran_target,msg.name);
                is_tran = YES;
                break;
            }                
            
            case TRAN_REC:
            {    
                printf("\n文件 %s 开始接收!\n",msg.fname);
                
                char *data = NULL;                                    //开辟空间
                data = (char *)malloc(msg.flen + 1);
                memset(data,0,msg.flen + 1);

                block_read(sockfd,data,msg.flen,msg.fname);           //接收文件

                int fd = open("rec",O_RDWR|O_CREAT|O_TRUNC,0666);     //保存在rec文件中

                if(fd < 0)
                {
                    printf("\n文件接收失败!\n");
                    break;
                }

                write(fd,data,msg.flen);                              //发送接收成功信息 
                printf("\n文件 %s 接收成功!\n",msg.fname);

                time(&ptime);
                strcpy(pestime,ctime(&ptime));

                char sql_getout[256] = {0};                           //存入数据库  
                sprintf(sql_getout,"insert into %s (id,name,size,target,time) values(%d,'%s','%s','%s','%s');",present_user,msg.id,present_user,"接收文件",msg.target,pestime);
                sqlite3_exec(db,sql_getout,NULL,0,&err_msg);

                close(fd);
                free(data);
                break;
            }

            //else if(strcmp(msg.msg,"accept") == 0)
            case TRAN_ACCEPT:
            {
                printf("\n对方同意接收,文件即将开始发送!\n");

                int fd = open(file_name,O_RDONLY);
                msg.flen = lseek(fd,0,SEEK_END);
                lseek(fd,0,SEEK_SET);
                
                char *data = NULL;
                data = (char *)malloc(msg.flen + 1);
                memset(data,0,msg.flen + 1);
                int m = read(fd,data,msg.flen);
                //printf("file_name = %s\n",file_name); 
                
                if(m < msg.flen || fd < 0)
                {
                    printf("\n文件读取失败,请检查文件是否正常后重试!\n");
                    strcpy(msg.target,msg.name);
                    strcpy(msg.name,present_user);
                    strcpy(msg.msg,"cancle");
                    write(sockfd,&msg,sizeof(msg_t));
                    free(data);
                    close(fd);
                    break;
                }

                msg.action = TRAN;
                strcpy(msg.target,msg.name);
                strcpy(msg.name,present_user);
                strcpy(msg.msg,"tranfile");
                strcpy(msg.fname,file_name);
                write(sockfd,&msg,sizeof(msg_t));
                
                block_write(sockfd,data,msg.flen,msg.fname);
                
                is_tran = NO;
                //printf("\n文件 %s 发送成功!\n",file_name);
                free(data);
                break;
            }

            //else if(strcmp(msg.msg,"refuse") == 0)
            case TRAN_REF:
            {
                printf("\n对方拒绝接收!\n");
                is_tran = NO;
                break;
            }        

            //else if(strcmp(msg.msg,"cancle") == 0)
            case TRAN_CANCLE:
            {
                is_tran = NO;
                printf("\n对方取消发送!\n");
                break;
            }    

            case TRAN_FAIL:                   //文件发送失败 
            {
                printf("\n文件传送失败!\n");
                break;
            }

            case TRAN_SUCC:                   //文件发送成功
            {
                is_tran = NO;
                printf("\n文件传送成功!\n");

                time(&ptime);
                strcpy(pestime,ctime(&ptime));

                char sql_getout[256] = {0};
                sprintf(sql_getout,"insert into %s (id,name,size,target,time) values(%d,'%s','%s','%s','%s');",present_user,msg.id,present_user,"发送文件",msg.target,pestime);
                sqlite3_exec(db,sql_getout,NULL,0,&err_msg);
                break;
            }

            case CHANGE_FAIL:
            {
                printf("\n修改失败,请重试!\n");
                break;
            }
			
            case QUIT_SYSTEM:              //退出系统
			{
				close(sockfd);
				return NULL;
			}
			
			case QUIT_FAIL:                //退出系统失败
			{
				printf("退出失败!\n");
				break;
			}

			default:
			{
				printf("未识别操作!\n");
				break;
			}
		}
	}	
	
	return NULL;
}
