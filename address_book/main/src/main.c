/*************************************************
  Copyright (C), 1994-2014, Wangxin
  File name:      count_9
  Author:    Wanxin   Version: 1.0       Date: 2014.6.1
  Description:    该程序实现基本输入输出
  Others:         无
  Function List:  
                  main
		  基本输入输出
  History:        no
*************************************************/

#include <stdio.h>
#include "../../include/typedef.h"

/*************************************************
  Function:       main
  Description:    实现基本输入输出
  Calls:          no
  Called By:      no
  Input:          no
  Output:         no
  Return:         0
  Others:         no
*************************************************/

int main()
{
    link head;
    link newfriend;
    
    create(&head);              //创建结点
    interface(&head,newfriend); //函数调用

    return 0;
}
