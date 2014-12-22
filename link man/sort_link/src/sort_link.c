/***************************************************************
Copyright (C). 2012-2015,XX
Filename:                sort_link.c
Author:                  xuebin
Version:                 1.0
Date:                    2014/08/08
Description:             链表按照名字排序
******************************************************************/

#include "../../include/myhead.h"

/******************************************************************
Function:                peo_p sort_link_name(peo_p)
Description:             链表按照名字排序
Calls:                   int is_null_link(peo_p)
Called By:               int select_menu()
Input:                   peo_p *head：源链表头指针的地址
Return:                  no
******************************************************************/

peo_p sort_link_name(peo_p *head)
{
    peo_p first;
    peo_p temp;
    peo_p ptr;
    peo_p flag;

    if(is_null_link(*head) == LINK_NULL)
    {
        printf("the link is null");

	return *head;
    }

    /*只有1个节点*/
    if((*head)->next->next == *head)
    {
        return *head;
    }

    /*冒泡排序*/
    for(flag = *head; flag->prior != *head; flag = flag->prior)
    {
        first = *head;                   //
        ptr = (*head)->next;
	temp = ptr->next;

        /*小的往左移*/
        while(temp != flag)
	{
	    if(mystrcmp(ptr->name, temp->name) == GREATER)
	    {
                temp->next->prior = ptr;
	        ptr->next = temp->next;
		temp->next = ptr;
                ptr->prior->next = temp;
		temp->prior = ptr->prior;
		ptr->prior = temp;
	    }

	    first = first->next;
            ptr = first->next;
            temp = ptr->next;
	}
    }

    return *head;
}
