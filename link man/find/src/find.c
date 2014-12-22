/***************************************************************
Copyright (C). 2012-2015,XX
Filename:                find.c
Author:                  xuebin
Version:                 1.0
Date:                    2014/08/08
Description:             搜索好友信息
******************************************************************/

#include "../../include/myhead.h"

/******************************************************************
Function:                int find_name(peo_p, peo_p *, char *)
Description:             按名字查找好友
Calls:                   peo_p create_node(peo_p *)
                         peo_p node_cpy(peo_p *, peo_p)
                         peo_p insert_head_node(peo_p *, peo_p *)
Called By:               int find(peo_p *);
Input:                   peo_p head:目标链头指针
                         peo_p *find_result：查找结果链头指针的地址
                         char *name:查找依据
Return:                  count;查找好友总数
******************************************************************/


int find_name(peo_p head, peo_p *find_result, char *name)
{
    int count = 0;
    peo_p temp = head->next;                    //移动指针
    *find_result = create_node(find_result);    //创建链表存放查询结果
    
    while(temp != head)
    {
        if(mystrcmp(temp->name, name) == 0)
	{
	    peo_p newnode;
            node_cpy(&newnode, temp);            //复制节点数据

	    insert_head_node(find_result, &newnode);

	    count++;                             //统计查询数量
	}
	temp = temp->next;
    }

    return count;
}

/******************************************************************
Function:                void find_result_operate(peo_p *, peo_p)
Description:             查找结果再操作
Calls:                   __fpurge(stdin);
                         int delete_node_id(peo_p *,int)
Called By:               int find(peo_p *);
Input:                   peo_p head:目标链头指针的地址
                         peo_p *find_result：查找结果链头指针的地址
Return:                  no
******************************************************************/

void find_result_operate(peo_p *head, peo_p find_result)
{
    int id;
    int select;

    do
    {
        printf("\n\n");
        printf("请再次输入删除或修改的人的id以确认操作:\n");
        __fpurge(stdin);
    }
    while(scanf("%d",&id) == 0);

    while(1)
    {
        printf("1.删除\n");
        //printf("2.修改\n");                    //修改信息
        printf(":");

        __fpurge(stdin);                         //清楚缓存区
        scanf("%d",&select);                     //缺少非法输入检测

	if(0 < select && 1 > select)
	{
	    break;
	}
	else
	{
	    printf("输入超出范围，请重新选择！\n");
	}
     }

    switch(select)
    {
        case F_DELETE:
	{
	    if(delete_node_id(head,id) == SUCCESS)
	    {
	        printf("SUCCESS\n");
	    }
	    break;
	}
	case F_MODIFY:
	{
	    break;
	}
	default:
	{
	    printf("已经返回\n");
	}
    }

}

/******************************************************************
Function:                int find(peo_p *)
Description:             查找好友功能入口
Calls:                   int find_name(peo_p, peo_p *, char *)
                         void display_link(peo_p)
                         void find_result_operate(peo_p *, peo_p)
Called By:               void select_del(peo_p *, int)
Input:                   peo_p head:目标链头指针
Return:                  count;查找好友总数
******************************************************************/

int find(peo_p *head)
{
    int count = 0;
    char *str = (char *)malloc(sizeof(char) * SIZE_STR);

    system("clear");
    printf("\n\n请输入要查找人的名字:\n");
    __fpurge(stdin);
    scanf("%s", str);

    peo_p find_result;
    count = find_name(*head, &find_result, str);         //按名字查

    if(count == 0)
    {
        printf("没有这个人!\n");
    }
    else
    {
        display_link(find_result);
	find_result_operate(head,find_result);
    }

    free(str);

    printf("\n按任意键继续\n");
    __fpurge(stdin);
    getchar();

    return count;
}

/******************************************************************
Function:                peo_p node_cpy(peo_p *,peo_p)
Description:             复制节点数据
Calls:                   peo_p create_node(peo_p *)
                         mystrcpy(char *,char *)
Called By:               int find_name(peo_p, peo_p *, char *)
Input:                   peo_p *newnode:目标节点指针的地址

                         peo_p src：源节点指针
Return:                  *newnode：目标节点指针
******************************************************************/

peo_p node_cpy(peo_p *newnode, peo_p src)
{
    *newnode = create_node(newnode);
    
    (*newnode)->id = src->id;
    mystrcpy((*newnode)->name, src->name);
    mystrcpy((*newnode)->cell, src->cell);
    mystrcpy((*newnode)->adress, src->adress);
    mystrcpy((*newnode)->company_tel, src->company_tel);

    return *newnode;
}
