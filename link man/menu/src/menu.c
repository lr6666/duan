/***************************************************************
Copyright (C). 2012-2015,XX
Filename:                menu.c
Author:                  xuebin
Version:                 1.0
Date:                    2014/08/08
Description:             主界面模块
******************************************************************/

#include "../../include/myhead.h"

/******************************************************************
Function:                void print_n_ch(char, int)
Description:             打印一行相同字符
Calls:                   no
Called By:               int menu()
Input:                   char ch：打印的字符
                         int n:打印的个数
Return:                  no
******************************************************************/

void print_n_ch(char ch, int n)
{
    int i = 0;
    while(i++ < n)
    {
        printf("%c",ch);
    }
}

/******************************************************************
Function:                int del_input(char *)
Description:             字符串转成对应选项的整型数
Calls:                   mystrcpy(char *,char *)
Called By:               int select_menu()
Input:                   char *sel：源字符串                        
Return:                  int result：转换后的整型
******************************************************************/

#if 1

int del_input(char *sel)
{
    int result = 0;                              //转换结果
   
    if(*sel > '0' && *sel < '9')             //截取第一个字符，数字直接转换
    {
        result = *sel - '0';
    }
    else
    {
        if(mystrcmp(sel,"ADD") == EQUAL)
	{
	    result = ADD;
	}
	else if(mystrcmp(sel,"DISPLAY") == EQUAL)
	{
	    result = DISPLAY;
	}
	else if(mystrcmp(sel, "FIND") == EQUAL)
	{
	    result = FIND;
	}
	else if(mystrcmp(sel, "DELETE") == EQUAL)
	{
	    result = DELETE;
	}
	else if(mystrcmp(sel, "SAVE") == EQUAL)
	{
	    result = SAVE;
	}
	else if(mystrcmp(sel, "EXIT") == EQUAL)
	{
	    result = INPUT_ERROR;
	}
    }

    return result;
}

/******************************************************************
Function:                int select_menu()
Description:             主菜单选项处理
Calls:                   int del_input(char *)
Called By:               int menu()
Input:                   no
Return:                  int result：功能对应的选项
******************************************************************/

int select_menu()
{
    char *sel = (char *)malloc(sizeof(char) * SIZE_STR);
    int result = 0;                            //选择结果

    printf(":");
    scanf("%s",sel);

    result = del_input(sel);               //输入的字符串转换成整数
    
    free(sel);

    return result;
}
#endif

/******************************************************************
Function:                int menu()
Description:             主菜单界面和入口
Calls:                   int select_menu()
Called By:               void select_del(peo_p *, int)
Input:                   no
Return:                  int result：功能对应的选项
******************************************************************/

int menu()
{
    int select = INPUT_ERROR;

    printf("\n\n\n\n");
    printf("\t\t\t        \033[32m通讯录\033[0m\n");
    printf("\t\t");
    print_n_ch('=', 41);
    printf("\n");

    printf("\t\t|       \033[32m1.添加好友信息\033[0m      \t\t|\n");
    printf("\t\t|       \033[32m2.列出好友信息\033[0m      \t\t|\n");
    printf("\t\t|       \033[32m3.搜索好友信息\033[0m      \t\t|\n");
    printf("\t\t|       \033[32m4.删除好友信息\033[0m      \t\t|\n");
    printf("\t\t|       \033[32m5.保存\033[0m              \t\t|\n");
    printf("\t\t|       \033[32m6.退出\033[0m              \t\t|\n");
    
    printf("\t\t");
    print_n_ch('=', 41);
    printf("\n");

    printf("\t\t| \033[32minput:1,2,3,4,5 & ADD,DISPLAY,\033[0m\t|\n");
    printf("\t\t|       \033[32mFIND,DELETE,EXIT\033[0m        \t|\n");

    printf("\t\t");
    print_n_ch('=', 41);
    printf("\n");
    printf("\t\t\t");
#if 1 
    while(1)
    {

        select = select_menu();

        if(0 < select && 7 > select)             //输入值符合条件
	{
	    break;
	}
	printf("\033[32minput error,please resume!\033[0m\n");
    }
#endif
    return select;
}

#if 0
int main()
{
    int r = menu();

    printf("r = %d\n",r);
    return 0;
}
#endif
