/***************************************************************
Copyright (C). 2012-2015,XX
Filename:                delete.c
Author:                  xuebin
Version:                 1.0
Date:                    2014/08/08
Description:             删除好友模块
******************************************************************/

#include "../../include/myhead.h"

/******************************************************************
Function:                int delete_node_id(peo_p *, int)
Description:             根据id删除节点
Calls:                   no
Called By:               int delete_node(peo_p *)
Input:                   peo_p *head：源链表头指针的地址
                         int id:删除依据
Return:                  int;成功与否
******************************************************************/

int delete_node_id(peo_p *head, int id)
{
     peo_p temp = (*head)->next;                //移动指针

     while(temp != *head )
     {
         if(temp->id == id)
	 {
	     temp->prior->next = temp->next;
	     temp->next->prior = temp->prior;

	     free(temp);
	     temp = NULL;

	     return SUCCESS;
	 }

	 temp = temp->next;
     }
     printf("no find,delete fail\n");
     return FAIL;
}

/******************************************************************
Function:                int delete_node(peo_p *)
Description:             删除好友功能入口
Calls:                   int delete_node_id(peo_p *, int)
Called By:               int select_menu()
Input:                   peo_p *head：源链表头指针的地址
Return:                  int;成功与否
******************************************************************/

int delete_node(peo_p *head)
{
    int result;                         //删除成功结果
    int id;                             //删除人的id
    
    system("clear");
    printf("输入你要删除的人的id:\n");
    __fpurge(stdin);
    scanf("%d",&id);
    result = delete_node_id(head, id);   //根据id删除

    if(SUCCESS == result)
    {
        printf("delete success!\n");
    }

    return result;
}
