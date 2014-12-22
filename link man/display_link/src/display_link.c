/***************************************************************
Copyright (C). 2012-2015,XX
Filename:                display_link.c
Author:                  xuebin
Version:                 1.0
Date:                    2014/08/08
Description:             好友列表显示模块
******************************************************************/

#include "../../include/myhead.h"

/******************************************************************
Function:                void display_link(peo_p)
Description:             好友信息列表显示
Calls:                   int is_null_link(peo_p)
Called By:               int select_menu()
Input:                   peo_p head：源链表头指针
Return:                  no
******************************************************************/

void display_link(peo_p head)
{
    peo_p temp = head->next;              //移动指针
    system("clear");                      //清屏
    is_null_link(head);                   //链表是否为空
    
    while(temp != head)                  //遍历打印
    {
        printf("\n\tid: %d\n", temp->id);
	printf("\t");
        print_n_ch('=', 60);
	printf("\n");

        printf("\t 名字: \t%s\t",temp->name);
        printf("\t 手机号码: \t%s\t  \n",temp->cell);
        printf("\t 地址: \t%s\t",temp->adress);
        printf("\t 公司电话: \t%s\t  \n",temp->company_tel);

	printf("\t");
        print_n_ch('=', 60);
	printf("\n");

	temp = temp->next;
    }
    printf("\n按任意键继续\n");
    __fpurge(stdin);
    getchar();
}
