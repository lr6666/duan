/***************************************************************
Copyright (C). 2012-2015,XX
Filename:                insert_data.c
Author:                  xuebin
Version:                 1.0
Date:                    2014/08/08
Description:             数据插入模块
******************************************************************/

#include "../../include/myhead.h"

/******************************************************************
Function:                peo_p create_node(peo_p *)
Description:             链表头插
Calls:                   no
                         peo_p insert_data(peo_p *)
Input:                   peo_p *head：源链表头指针的地址
                         peo_p *newnode：新节点的指针的地址
Return:                  *head：链表头指针
******************************************************************/

peo_p insert_head_node(peo_p *head, peo_p *newnode)
{
    (*newnode)->next = (*head)->next;
    (*head)->next->prior = (*newnode);

    (*newnode)->prior = *head;
    (*head)->next = *newnode;

    return *head;
}

/******************************************************************
Function:                peo_p create_node(peo_p *)
Description:             创建新节点
Calls:                   no
Called By:               main()
                         peo_p insert_data(peo_p *)
Input:                   peo_p *newnode：新节点的指针的地址
Return:                  *newnode：新节点指针
******************************************************************/

peo_p create_node(peo_p * newnode)
{
    *newnode = (peo_p)malloc(NODE_LEN);
    if(*newnode == NULL)                       //请求分配空间失败退出
    {
        printf("malloc fail!\n");
	exit(MALLOC_FAIL);
    }
    (*newnode)->next = *newnode;                //创建双向循环列表
    (*newnode)->prior = *newnode;

    return *newnode;
}

/******************************************************************
Function:                peo_p input_data(peo_p *)
Description:             节点数据键入
Calls:                   no
Called By:               peo_p insert_data(peo_p *)
Input:                   peo_p *newnode：新节点的指针的地址
Return:                  *newnode：新节点指针
******************************************************************/

peo_p input_data(peo_p *newnode)
{
    system("clear");
    printf("\n\n请输入好友名字:\n");
    __fpurge(stdin);                             //删除缓存区内容
    scanf("%s", (*newnode)->name);
   
    printf("\n请输入好友手机号码:\n");
    __fpurge(stdin);                             //删除缓存区内容
    scanf("%s", (*newnode)->cell);
    
    printf("\n请输入好友地址:\n");
    __fpurge(stdin);                             //删除缓存区内容
    scanf("%s", (*newnode)->adress);

    printf("\n请输入好友公司电话号码:\n");
    __fpurge(stdin);                             //删除缓存区内容
    scanf("%s", (*newnode)->company_tel);

    print_n_ch('=', 60);
    printf("\n");

    return *newnode;
}

/******************************************************************
Function:                nt max_count_node(peo_p)
Description:             链表节点数（不包括头节点）
Calls:                   no
Called By:               peo_p insert_data(peo_p *)
Input:                   peo_p *head：源链表头指针的地址
Return:                  peo_p head：链表的节点总数
******************************************************************/

int max_count_node(peo_p head)
{
    int count = 0;
    peo_p temp = head->next;

    while(temp != head)
    {
        temp = temp->next;
	count++;
    }

    return count;
}

/******************************************************************
Function:                peo_p insert_data(peo_p *)
Description:             添加好友功能入口
Calls:                   peo_p create_node(peo_p *)
                         peo_p input_data(peo_p *)
                         peo_p insert_head_node(peo_p *, peo_p *)
Called By:               int select_menu()
Input:                   peo_p *head：源链表头指针的地址
Return:                  peo_p head：链表的头指针
******************************************************************/

peo_p insert_data(peo_p *head)
{
    char temp;

    while(1) 
    {
        peo_p newnode;

        create_node(&newnode);                     //创建新节点
        newnode = input_data(&newnode);            //新节点键入数据
	newnode->id = max_count_node(*head) + 1;   //bug
        
	insert_head_node(head, &newnode);          //头插法插入链表

	printf("输入q或Q退出，其余输入继续\n");
	__fpurge(stdin);
	temp = getchar();

	if(temp == 'q' || temp == 'Q')
	{
	    break;
	}
    }

    return *head;
}
