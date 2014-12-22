/***************************************************************
Copyright (C). 2012-2015,XX
Filename:                is_null_link.c
Author:                  xuebin
Version:                 1.0
Date:                    2014/08/08
Description:             链表是否为空
******************************************************************/

#include "../../include/myhead.h"

/******************************************************************
Function:                int is_null_link(peo_p)
Description:             链表是否为空
Calls:                   no
                         void display_link(peo_p)
Input:                   peo_p head：源链表头指针
Return:                  int:成功与否
******************************************************************/

int is_null_link(peo_p head)
{
    if(head->next == head)
    {
        return LINK_NULL;          //链表为空
    }

    return LINK_N_NULL;
}
