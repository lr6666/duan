#include "../../include/myhead.h"

void select_del(peo_p *head, int select)
{

    switch(select)
    {
        case ADD:                              //添加好友
	{
	    insert_data(head);
	    break;
	}
        case DISPLAY:                          //列表好友
	{
	    sort_link_name(head);
	    display_link(*head);    
	    break;
	}
        case FIND:                             //搜索好友
	{
            find(head);	    
	    break;
	}
        case DELETE:                           //删除好友信息
	{
            delete_node(head);	    
	    break;
	}
	case SAVE:
	{
	    save_data(*head);
	    break;
	}
	default:
	{
	    return;
	}
    }

}

int main()
{
    peo_p head;
    head = create_node(&head);                  //创建带头节点双向循环链表
    
    get_data(&head);    
    //getdata_file(&head);

    int select = 0;                             //选择结果
 
    while(1)
    {
        system("clear");                        //清屏 
	select = menu();                        //显示菜单选项

	if(select == SELECT_EXIT)               //退出程序
	{
	    printf("\033[32m\t\tpress any key eixt\n\033[0m");
	    __fpurge(stdin);
	    getchar();
	    break;
	}

	select_del(&head, select);              //
    }
    
    save_data(head);    

    system("clear");

    return EXIT_SUCCESS;
}
