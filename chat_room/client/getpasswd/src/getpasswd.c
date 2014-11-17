#include "../../include/myhead.h"

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
