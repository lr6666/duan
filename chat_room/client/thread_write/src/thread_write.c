#include "../../include/myhead.h"

/****************************************
Function:        void *thread_write(void *arg)
Description:     向服务器发送消息,指令
Calls:           void log_or_register()
                 void after_log()
                 block_write(int,char *,int,char *)
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

/****************************************
Function:        int getpasswd(char *,int)
Description:     输入密码
Calls:           no
Call By:         int set_disp_mode(int,int)
Input:           char *passwd(密码),   int size(密码长度)
Output:          no
return:          n(每一位密码) 
Others:          no
****************************************/

int getpasswd(char *passwd,int size)     //获取密码
{
	int c;
	int n = 0;
	
	do
	{
		c = getchar();
		if(c != '\n' | c != '\r')
		{
			passwd[n++] = c;
		}	
	}
	while(c != '\n' && c != '\r' && n < (size - 1));
	
	passwd[n] = '\0';
	
	return n;
}

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




void *thread_write(void *arg)                  //像服务器发送命令 
{
	char *p;                               
	int sockfd = *((int *)arg);                //连接描述符
	char cmd[CMD_SIZE] = {0};                  // 操作命令
	char target[NAME_LEN] = {0};               //消息发送对象
	char message[MSG_MAX_LEN] = {0};           //消息内容
	char password[PSWOD_LEN] = {0};            //密码
	char passwd[PSWOD_LEN] = {0};              //确认密码
	msg_t msg;                                 //结构体
	time_t ptime;                              //获取时间 
	
	system("reset");
	log_or_register();                         //登录页面
	
	while(1)
	{
		sleep(1);
		
        gets(cmd);

		if(strcmp(cmd,"log") == 0)             //登录
		{
			if(is_log == NO)                   //未登录
			{	
                int id = 0;
				printf("请输入id:");
                scanf("%d",&(msg.id));
				getchar();
                printf("请输入密码:");
				
				set_disp_mode(STDIN_FILENO,0);                 //取消密码回显
				getpasswd(password,sizeof(password));
				p = password;
				
				while(*p != '\n')
				{
				    p++;	
				}	
				*p = '\0';
				
				set_disp_mode(STDIN_FILENO,1);
				
				msg.action = LOG;
				strcpy(msg.password,password);
				write(sockfd,&msg,sizeof(msg));
		    }
			
			else
			{
			    printf("\n您已登录!\n");	
			}	
			
			continue;
		}

		else if(strcmp(cmd,"reg") == 0)                 //注册
		{
			printf("请输入用户名:");
			//scanf("%s",msg.name);
			gets(msg.name);
            
            int i = 0;                                   
            while(*(msg.name + i) != 0)          //用户名是否有空格
            {
                if(*(msg.name + i) == ' ')
                {
                    break;
                }    
                i++;
            }
            
            if(*(msg.name + i) == ' ')           //有空格不能注册
            {
                printf("\n亲,用户名不能有空格哦\n");    
                continue;
            }

			printf("请输入密码:");
			set_disp_mode(STDIN_FILENO,0);                   //取消密码回显
			getpasswd(password,sizeof(password));
			p = password;
			
			while(*p != '\n')
			{
				p++;	
			}	
			*p = '\0';
			
			printf("\n请重新输入密码:");
			getpasswd(passwd,sizeof(passwd));
			p = passwd;
			while(*p != '\n')
			{
			    p++;	
			}	
			*p = '\0';
			
			set_disp_mode(STDIN_FILENO,1);
			
			if(strcmp(password,passwd) != 0)                   //密码不一致        
			{
				printf("\n两次输入的密码不一致!\n");
				continue;
			}	
			
			msg.action = REGISTER;
			strcpy(msg.password,passwd);
			write(sockfd,&msg,sizeof(msg));
			continue;
		}

        else if(strcmp(cmd,"chpswd") == 0)             //修改密码
        {
            printf("\n请输入新密码:");
            gets(password);
            printf("请再次输入新密码:");
            gets(msg.password);
            if(strcmp(password,msg.password) != 0)     //两次输入确认
            {
                printf("\n亲,两次输入的密码不一致哦!\n");
                continue;
            }

			msg.action = CH_PASSWD;
			strcpy(msg.name,present_user);               
            msg.id = id;
            write(sockfd,&msg,sizeof(msg));            //发送修改密码指令
            
            continue;
        }    
		
        else if(strcmp(cmd,"chname") == 0)       //修改用户名
        {
            char name_new[NAME_LEN] = {0};      
            printf("\n请输入新用户名:");
            gets(name_new);
            printf("请再次输入新用户名:");
            gets(msg.name);
 
            if(strcmp(name_new,msg.name) != 0)   //两次输入确认
            {
                printf("\n亲,两次输入的用户名不一致哦!\n");
                continue;
            }

            int i = 0;                                   
            while(*(msg.name + i) != 0)          //用户名是否有空格
            {
                if(*(msg.name + i) == ' ')
                {
                    break;
                }    
                i++;
            }
            
            if(*(msg.name + i) == ' ')           //有空格不能注册
            {
                printf("\n亲,用户名不能有空格哦\n");    
                continue;
            }
			
            msg.action = CH_NAME;
            msg.id = id;
            write(sockfd,&msg,sizeof(msg));      //发送修改用户名指令
            
            continue;
        }    

        else if(strcmp(cmd,"whsp") == 0)                       //私密
		{
			if(is_log == YES && is_slience == NO)
			{	
				printf("请输入接收的用户:");
				//scanf("%s",msg.target);
				gets(msg.target);
                printf("\n请输入消息内容:");
				//getchar();
				gets(msg.msg);
				
				if(strcmp(msg.msg,":)") == 0)                  //表情符转换
				{
					strcpy(msg.msg,"SmIlE");	
				}	
				
				else if(strcmp(msg.msg,":(") == 0)
				{
					strcpy(msg.msg,"SaD");	
                }
				
				msg.id = id;
                msg.action = WHISPER;
				strcpy(msg.name,present_user);
				write(sockfd,&msg,sizeof(msg));
			}
			
			else if(is_log == NO)
			{
			    printf("\n请先登录!\n");	
			}
			
			else if(is_log == YES && is_slience == YES)              //被禁言无法发送
			{
			    printf("\n你已被禁言!\n");	
				printf("你可以输入require请求管理员解禁!\n");
			}	
			
			continue;
		}

		else if(strcmp(cmd,"chat") == 0)
		{
			if(is_log == YES && is_slience == NO)             //已登录且未被禁言
			{	
				strcpy(msg.name,present_user);
				printf("请输入消息内容:");
				//getchar();
				gets(msg.msg);
				
				if(strcmp(msg.msg,":)") == 0)
				{
					strcpy(msg.msg,"SmIlE");	
				}	
				
				else if(strcmp(msg.msg,":(") == 0)
				{
					strcpy(msg.msg,"SaD");	
				}	
				
                msg.id =id;
				msg.action = CHAT;
				write(sockfd,&msg,sizeof(msg));
			}
			
			else if(is_log == NO)                          //未登录
			{
				printf("\n请先登录!\n");	
			}
			
			else if(is_log == YES && is_slience == YES)    //被禁言
			{
			    printf("\n你已被禁言!\n");	
				printf("你可以输入require请求管理员解禁!\n");
			}	
			continue;
		}

		else if(strcmp(cmd,"show") == 0)                   //查询在线用户
		{
			if(is_log == YES)
			{	
				msg.id = id;
                msg.action = SHOW_ONLINE;
				write(sockfd,&msg,sizeof(msg));
	    	}
			
			else
			{
				printf("\n请先登录!\n");                   //未登录
			}
		}

		else if(strcmp(cmd,"quit") == 0)                   //请求退出系统
		{
			msg.id = id;
            msg.action = QUIT_SYSTEM;
            strcpy(msg.name,present_user);
			write(sockfd,&msg,sizeof(msg));
			close(sockfd);
			return NULL;
		}
		
		else if(strcmp(cmd,"table") == 0)                  //返回页面 
		{
			system("reset");
			after_log();
			continue;
		}
		
		else if(strcmp(cmd,"slience") == 0)                //禁言
		{
			if(strcmp(present_user,"admin") == 0)
			{	
				msg.id = id;
                msg.action = SLIENCE;
				printf("请输入想要禁言的用户:");
				//scanf("%s",msg.target);
				gets(msg.target);

				if(strcmp(msg.target,"admin") == 0)        //不能禁言自己
				{
				    printf("\n亲爱的,要对自己好一点啊!\n");	
				}	
				
				else 
				{	
					write(sockfd,&msg,sizeof(msg));
			    }
			}
			
			else                                           //非管理员 
			{
				printf("\n你不是管理员,没有此项操作权利!\n");
		    }
			continue;
		}

		else if(strcmp(cmd,"require") == 0)                //请求解禁
		{
			if(is_slience == YES && is_log == YES)
			{	
				msg.id = id;
                msg.action = REQUIRE;
				strcpy(msg.target,"admin");
				write(sockfd,&msg,sizeof(msg));
				printf("\n管理员大大我错了,放我出去...\n");
			}
			
			else if(is_slience == NO)                      //没有被禁言
			{
				printf("\n莫非你干了坏事管理员大大没有发现,你还没被禁言纳!\n");
			}
			
			else if(is_log == NO)                          //未登录
			{
			    printf("\n亲,你还没有登录哦!\n");	
			}	
			continue;
		}
		
		else if(strcmp(cmd,"say") == 0)                     //解禁
		{
			if(strcmp(present_user,"admin") == 0)
			{	
				msg.id = id;
                msg.action = SAY;
				printf("请输入要解禁的用户:");
				//scanf("%s",msg.target);
				gets(msg.target);
                write(sockfd,&msg,sizeof(msg));
			}
			
			else                                            //非管理员
			{
				printf("\n你不是管理员,没有此项操作权利!\n");
		    }
			continue;
		}
		
		else if(strcmp(cmd,"getout") == 0)                  //踢出聊天室
		{
			if(strcmp(present_user,"admin") == 0)
			{	
				printf("请输入将要被踢出的用户:");
				//scanf("%s",msg.target);
				gets(msg.target);
                msg.id = id;
                msg.action = GETOUT;
				write(sockfd,&msg,sizeof(msg));
			}
			
			else                                            //非管理员  
			{
				printf("\n你不是管理员,没有此项操作权利!\n");
		    }
			continue;
		}
		
		else if(strcmp(cmd,"tran") == 0)                    //发送文件
		{
			if(is_log == YES)
			{	
				printf("请输入文件传输的对象:");                      //接受方  
				//scanf("%s",msg.target);
				gets(msg.target);
                printf("请输入要传输的文件:");                        //文件名
				//scanf("%s",msg.fname);
				gets(msg.fname);
                int fd = open(msg.fname,O_RDONLY);
				
				if(fd < 0)
				{
					printf("\n文件打开失败,请检查文件是否正常!\n");	  //打开文件失败
				}	
                
                else
                {
                    msg.flen = lseek(fd,0,SEEK_END);                  //文件大小 
                    lseek(fd,0,SEEK_SET);

                    strcpy(file_name,msg.fname);                      //文件名,方便写线程读取
                    msg.id = id;
                    msg.action = TRAN;
                    memset(&msg.name,0,sizeof(msg.name));             
                    memset(&msg.msg,0,sizeof(msg.msg));
                    strcpy(msg.name,present_user);
                    strcpy(msg.msg,"wanttotran");
				    write(sockfd,&msg,sizeof(msg));                   //等待对方是否接收
                    
                    printf("\n请等待对方确认...\n");
                    is_tran == YES;                                   //有文件传输指令
                    close(fd);
                }
			}
			
			else if(is_log == NO)
			{
				printf("\n你还没登录哦亲!\n");                       //未登录
		    }
			
			continue;
		}
        
        else if(is_tran == YES && (strcmp(cmd,"yes") == 0))          //有文件传输,指令有效
        {
            if(is_log == NO)                                         //未登录
            {
                printf("\n亲,你还没有登录哦!\n");
                continue;
            }    

            msg.action = TRAN;
            strcpy(msg.msg,"accept");
            strcpy(msg.name,present_user);
            strcpy(msg.target,tran_target);
            write(sockfd,&msg,sizeof(msg_t));                        //同意接收文件  
            
            continue;
        }    
        
        else if(is_tran == YES && (strcmp(cmd,"no") == 0))           //指令no是否可用
        {
            if(is_log == NO)                                         //未登录
            {
                printf("\n亲,你还没有登录哦!\n");
                continue;
            }   

            msg.action = TRAN;
            strcpy(msg.msg,"refuse");
            strcpy(msg.name,present_user);
            strcpy(msg.target,tran_target);
            write(sockfd,&msg,sizeof(msg_t));
            printf("\n你已拒绝接收该文件!\n");                       //拒绝接收文件
            is_tran = NO;            
            continue;
        }    
        
		else
		{
			printf("\n未知命令!请检查后再输入!\n");                 //未知命令
			continue;
		}
	}

	return NULL;
}

/****************************************
Function:        void *thread_write(void *arg)
Description:     向服务器发送消息,指令
Calls:           void log_or_register()
                 void after_log()
                 block_write(int,char *,int,char *)
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

void *thread_write(void *arg)                  //像服务器发送命令 
{
	char *p;                               
	int sockfd = *((int *)arg);                //连接描述符
	char cmd[CMD_SIZE] = {0};                  // 操作命令
	char target[NAME_LEN] = {0};               //消息发送对象
	char message[MSG_MAX_LEN] = {0};           //消息内容
	char password[PSWOD_LEN] = {0};            //密码
	char passwd[PSWOD_LEN] = {0};              //确认密码
	msg_t msg;                                 //结构体
	time_t ptime;                              //获取时间 
	
	system("reset");
	log_or_register();                         //登录页面
	
	while(1)
	{
		sleep(1);
		
        gets(cmd);

		if(strcmp(cmd,"log") == 0)             //登录
		{
			if(is_log == NO)                   //未登录
			{	
                int id = 0;
				printf("请输入id:");
                scanf("%d",&(msg.id));
				getchar();
                printf("请输入密码:");
				
				set_disp_mode(STDIN_FILENO,0);                 //取消密码回显
				getpasswd(password,sizeof(password));
				p = password;
				
				while(*p != '\n')
				{
				    p++;	
				}	
				*p = '\0';
				
				set_disp_mode(STDIN_FILENO,1);
				
				msg.action = LOG;
				strcpy(msg.password,password);
				write(sockfd,&msg,sizeof(msg));
		    }
			
			else
			{
			    printf("\n您已登录!\n");	
			}	
			
			continue;
		}

		else if(strcmp(cmd,"reg") == 0)                 //注册
		{
			printf("请输入用户名:");
			//scanf("%s",msg.name);
			gets(msg.name);
            
            int i = 0;                                   
            while(*(msg.name + i) != 0)          //用户名是否有空格
            {
                if(*(msg.name + i) == ' ')
                {
                    break;
                }    
                i++;
            }
            
            if(*(msg.name + i) == ' ')           //有空格不能注册
            {
                printf("\n亲,用户名不能有空格哦\n");    
                continue;
            }

			printf("请输入密码:");
			set_disp_mode(STDIN_FILENO,0);                   //取消密码回显
			getpasswd(password,sizeof(password));
			p = password;
			
			while(*p != '\n')
			{
				p++;	
			}	
			*p = '\0';
			
			printf("\n请重新输入密码:");
			getpasswd(passwd,sizeof(passwd));
			p = passwd;
			while(*p != '\n')
			{
			    p++;	
			}	
			*p = '\0';
			
			set_disp_mode(STDIN_FILENO,1);
			
			if(strcmp(password,passwd) != 0)                   //密码不一致        
			{
				printf("\n两次输入的密码不一致!\n");
				continue;
			}	
			
			msg.action = REGISTER;
			strcpy(msg.password,passwd);
			write(sockfd,&msg,sizeof(msg));
			continue;
		}

        else if(strcmp(cmd,"chpswd") == 0)             //修改密码
        {
            printf("\n请输入新密码:");
            gets(password);
            printf("请再次输入新密码:");
            gets(msg.password);
            if(strcmp(password,msg.password) != 0)     //两次输入确认
            {
                printf("\n亲,两次输入的密码不一致哦!\n");
                continue;
            }

			msg.action = CH_PASSWD;
			strcpy(msg.name,present_user);               
            msg.id = id;
            write(sockfd,&msg,sizeof(msg));            //发送修改密码指令
            
            continue;
        }    
		
        else if(strcmp(cmd,"chname") == 0)       //修改用户名
        {
            char name_new[NAME_LEN] = {0};      
            printf("\n请输入新用户名:");
            gets(name_new);
            printf("请再次输入新用户名:");
            gets(msg.name);
 
            if(strcmp(name_new,msg.name) != 0)   //两次输入确认
            {
                printf("\n亲,两次输入的用户名不一致哦!\n");
                continue;
            }

            int i = 0;                                   
            while(*(msg.name + i) != 0)          //用户名是否有空格
            {
                if(*(msg.name + i) == ' ')
                {
                    break;
                }    
                i++;
            }
            
            if(*(msg.name + i) == ' ')           //有空格不能注册
            {
                printf("\n亲,用户名不能有空格哦\n");    
                continue;
            }
			
            msg.action = CH_NAME;
            msg.id = id;
            write(sockfd,&msg,sizeof(msg));      //发送修改用户名指令
            
            continue;
        }    

        else if(strcmp(cmd,"whsp") == 0)                       //私密
		{
			if(is_log == YES && is_slience == NO)
			{	
				printf("请输入接收的用户:");
				//scanf("%s",msg.target);
				gets(msg.target);
                printf("\n请输入消息内容:");
				//getchar();
				gets(msg.msg);
				
				if(strcmp(msg.msg,":)") == 0)                  //表情符转换
				{
					strcpy(msg.msg,"SmIlE");	
				}	
				
				else if(strcmp(msg.msg,":(") == 0)
				{
					strcpy(msg.msg,"SaD");	
                }
				
				msg.id = id;
                msg.action = WHISPER;
				strcpy(msg.name,present_user);
				write(sockfd,&msg,sizeof(msg));
			}
			
			else if(is_log == NO)
			{
			    printf("\n请先登录!\n");	
			}
			
			else if(is_log == YES && is_slience == YES)              //被禁言无法发送
			{
			    printf("\n你已被禁言!\n");	
				printf("你可以输入require请求管理员解禁!\n");
			}	
			
			continue;
		}

		else if(strcmp(cmd,"chat") == 0)
		{
			if(is_log == YES && is_slience == NO)             //已登录且未被禁言
			{	
				strcpy(msg.name,present_user);
				printf("请输入消息内容:");
				//getchar();
				gets(msg.msg);
				
				if(strcmp(msg.msg,":)") == 0)
				{
					strcpy(msg.msg,"SmIlE");	
				}	
				
				else if(strcmp(msg.msg,":(") == 0)
				{
					strcpy(msg.msg,"SaD");	
				}	
				
                msg.id =id;
				msg.action = CHAT;
				write(sockfd,&msg,sizeof(msg));
			}
			
			else if(is_log == NO)                          //未登录
			{
				printf("\n请先登录!\n");	
			}
			
			else if(is_log == YES && is_slience == YES)    //被禁言
			{
			    printf("\n你已被禁言!\n");	
				printf("你可以输入require请求管理员解禁!\n");
			}	
			continue;
		}

		else if(strcmp(cmd,"show") == 0)                   //查询在线用户
		{
			if(is_log == YES)
			{	
				msg.id = id;
                msg.action = SHOW_ONLINE;
				write(sockfd,&msg,sizeof(msg));
	    	}
			
			else
			{
				printf("\n请先登录!\n");                   //未登录
			}
		}

		else if(strcmp(cmd,"quit") == 0)                   //请求退出系统
		{
			msg.id = id;
            msg.action = QUIT_SYSTEM;
            strcpy(msg.name,present_user);
			write(sockfd,&msg,sizeof(msg));
			close(sockfd);
			return NULL;
		}
		
		else if(strcmp(cmd,"table") == 0)                  //返回页面 
		{
			system("reset");
			after_log();
			continue;
		}
		
		else if(strcmp(cmd,"slience") == 0)                //禁言
		{
			if(strcmp(present_user,"admin") == 0)
			{	
				msg.id = id;
                msg.action = SLIENCE;
				printf("请输入想要禁言的用户:");
				//scanf("%s",msg.target);
				gets(msg.target);

				if(strcmp(msg.target,"admin") == 0)        //不能禁言自己
				{
				    printf("\n亲爱的,要对自己好一点啊!\n");	
				}	
				
				else 
				{	
					write(sockfd,&msg,sizeof(msg));
			    }
			}
			
			else                                           //非管理员 
			{
				printf("\n你不是管理员,没有此项操作权利!\n");
		    }
			continue;
		}

		else if(strcmp(cmd,"require") == 0)                //请求解禁
		{
			if(is_slience == YES && is_log == YES)
			{	
				msg.id = id;
                msg.action = REQUIRE;
				strcpy(msg.target,"admin");
				write(sockfd,&msg,sizeof(msg));
				printf("\n管理员大大我错了,放我出去...\n");
			}
			
			else if(is_slience == NO)                      //没有被禁言
			{
				printf("\n莫非你干了坏事管理员大大没有发现,你还没被禁言纳!\n");
			}
			
			else if(is_log == NO)                          //未登录
			{
			    printf("\n亲,你还没有登录哦!\n");	
			}	
			continue;
		}
		
		else if(strcmp(cmd,"say") == 0)                     //解禁
		{
			if(strcmp(present_user,"admin") == 0)
			{	
				msg.id = id;
                msg.action = SAY;
				printf("请输入要解禁的用户:");
				//scanf("%s",msg.target);
				gets(msg.target);
                write(sockfd,&msg,sizeof(msg));
			}
			
			else                                            //非管理员
			{
				printf("\n你不是管理员,没有此项操作权利!\n");
		    }
			continue;
		}
		
		else if(strcmp(cmd,"getout") == 0)                  //踢出聊天室
		{
			if(strcmp(present_user,"admin") == 0)
			{	
				printf("请输入将要被踢出的用户:");
				//scanf("%s",msg.target);
				gets(msg.target);
                msg.id = id;
                msg.action = GETOUT;
				write(sockfd,&msg,sizeof(msg));
			}
			
			else                                            //非管理员  
			{
				printf("\n你不是管理员,没有此项操作权利!\n");
		    }
			continue;
		}
		
		else if(strcmp(cmd,"tran") == 0)                    //发送文件
		{
			if(is_log == YES)
			{	
				printf("请输入文件传输的对象:");                      //接受方  
				//scanf("%s",msg.target);
				gets(msg.target);
                printf("请输入要传输的文件:");                        //文件名
				//scanf("%s",msg.fname);
				gets(msg.fname);
                int fd = open(msg.fname,O_RDONLY);
				
				if(fd < 0)
				{
					printf("\n文件打开失败,请检查文件是否正常!\n");	  //打开文件失败
				}	
                
                else
                {
                    msg.flen = lseek(fd,0,SEEK_END);                  //文件大小 
                    lseek(fd,0,SEEK_SET);

                    strcpy(file_name,msg.fname);                      //文件名,方便写线程读取
                    msg.id = id;
                    msg.action = TRAN;
                    memset(&msg.name,0,sizeof(msg.name));             
                    memset(&msg.msg,0,sizeof(msg.msg));
                    strcpy(msg.name,present_user);
                    strcpy(msg.msg,"wanttotran");
				    write(sockfd,&msg,sizeof(msg));                   //等待对方是否接收
                    
                    printf("\n请等待对方确认...\n");
                    is_tran == YES;                                   //有文件传输指令
                    close(fd);
                }
			}
			
			else if(is_log == NO)
			{
				printf("\n你还没登录哦亲!\n");                       //未登录
		    }
			
			continue;
		}
        
        else if(is_tran == YES && (strcmp(cmd,"yes") == 0))          //有文件传输,指令有效
        {
            if(is_log == NO)                                         //未登录
            {
                printf("\n亲,你还没有登录哦!\n");
                continue;
            }    

            msg.action = TRAN;
            strcpy(msg.msg,"accept");
            strcpy(msg.name,present_user);
            strcpy(msg.target,tran_target);
            write(sockfd,&msg,sizeof(msg_t));                        //同意接收文件  
            
            continue;
        }    
        
        else if(is_tran == YES && (strcmp(cmd,"no") == 0))           //指令no是否可用
        {
            if(is_log == NO)                                         //未登录
            {
                printf("\n亲,你还没有登录哦!\n");
                continue;
            }   

            msg.action = TRAN;
            strcpy(msg.msg,"refuse");
            strcpy(msg.name,present_user);
            strcpy(msg.target,tran_target);
            write(sockfd,&msg,sizeof(msg_t));
            printf("\n你已拒绝接收该文件!\n");                       //拒绝接收文件
            is_tran = NO;            
            continue;
        }    
        
		else
		{
			printf("\n未知命令!请检查后再输入!\n");                 //未知命令
			continue;
		}
	}

	return NULL;
}