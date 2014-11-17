#include "../../include/myhead.h"

/****************************************
Function:        int set_disp_mode(int,int)
Description:     取消密码回显
Calls:           no
Call By:         void *thread_write(void *arg) 
Input:           int fd,int option
Output:          no
return:          no
Others:          no
****************************************/

int set_disp_mode(int fd,int option)        //取消密码回显
{
    int err;
    struct termios term;
	
	if(tcgetattr(fd,&term) == -1)                          
	{
	    perror("cannot get the attribution of thr terminal");
		return 1;
	}	
	
	if(option)
	{
	    term.c_lflag |= ECHOFLAGS;
	}	
	
	else
	{
		term.c_lflag &=~ ECHOFLAGS;	
	}
	err = tcsetattr(fd,TCSAFLUSH,&term);
	
	if(err = -1 && err == EINTR)
	{
	    perror("cannot get the attribution of thr terminal");
		return 1;
	}
}    
