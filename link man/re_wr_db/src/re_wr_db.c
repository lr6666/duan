#include "../../include/myhead.h"

int str2int(char *src)
{
    char *temp = src;
    int temp_num = 0;

    while(*temp != '\0')
    {
        if(*temp <= '9' && *temp >= 0)
	{
	    if(*(temp + 1) <= '9' && *(temp + 1) >= '0')
	    {
                temp_num += (*temp - '0') * 10;
	    }
	    else
	    {
		temp_num += *temp - '0';	     
		
		return temp_num;
	    }
	}
    }

    return -1;
}

void is_sqlite3_error(int rc, int nrow, char *errormsg)
{
    if(rc == SQLITE_OK)
    {
        //printf("%d:sqlite3 ok\n", nrow);
    }
    else 
    {
        printf("%d:sqlite3 error\n", nrow);
        printf("error:%s\n",errormsg);
    }
}

void create_db(sqlite3 **db)
{
    int rc;
    char *errormsg;
    
    rc = sqlite3_open(DB_NAME, db);
    is_sqlite3_error(rc, __LINE__,errormsg);
    
    char *sql = (char *)malloc(sizeof(char) * SIZE);
    char *sql1 = (char *)malloc(sizeof(char) * SIZE);

    sprintf(sql1,"drop table %s", TABLE_NAME);
    rc = sqlite3_exec(*db, sql1, NULL, NULL,&errormsg);
    is_sqlite3_error(rc, __LINE__,errormsg);
    free(sql1);
   
    sprintf(sql,"create table %s(%s)", TABLE_NAME,"id integer primary key,name text,cell text,adress text,company_tel text");

    rc = sqlite3_exec(*db, sql, NULL, NULL,&errormsg);
    is_sqlite3_error(rc, __LINE__,errormsg);
    
    free(sql);
}

void insert_db(sqlite3 **db, peo_p node_p)
{
    int rc;
    char *errormsg;
    char *sql = (char *)malloc(sizeof(char) * SIZE);

    sprintf(sql, "insert into %s values (%d,'%s','%s','%s','%s')",
        TABLE_NAME, node_p->id, node_p->name, node_p->cell, 
	node_p->adress, node_p->company_tel);

    rc = sqlite3_exec(*db, sql, NULL, NULL, &errormsg);
    is_sqlite3_error(rc, __LINE__,errormsg);

    free(sql);
}


void save_data(peo_p head)
{
    peo_p temp = head->next;
    sqlite3 *db = NULL;

    create_db(&db);                              //创建数据库，建表

    while(temp != head)                             //链表数据存入数据库
    {
        insert_db(&db,temp);    
	
	temp = temp->next;
    }

    is_sqlite3_error(sqlite3_close(db), __LINE__,NULL);  //关闭数据库
}

peo_p select_db(sqlite3 *db, peo_p *head)
{
    int rc;
    char *sql = (char *)malloc(sizeof(char) * SIZE);
    sqlite3_stmt *stmt = NULL;

    sprintf(sql,"select * from %s", TABLE_NAME);

    rc = sqlite3_prepare(db, sql, SIZE, &stmt, 0);

    is_sqlite3_error(rc, __LINE__,NULL);

    rc = sqlite3_step(stmt);
    while(rc == SQLITE_ROW)
    {
        peo_p newnode;

	create_node(&newnode);

	newnode->id = str2int((char *)sqlite3_column_text(stmt, 0));
	mystrcpy(newnode->name, (char *)sqlite3_column_text(stmt, 1));
	mystrcpy(newnode->cell, (char *)sqlite3_column_text(stmt, 2));
	mystrcpy(newnode->adress, (char *)sqlite3_column_text(stmt, 3));
	mystrcpy(newnode->company_tel, (char *)sqlite3_column_text(stmt, 4));

	insert_head_node(head, &newnode);

	rc = sqlite3_step(stmt);
    }

    sqlite3_finalize(stmt);

    free(sql);
    return *head;
}

peo_p get_data(peo_p *head)
{
    sqlite3 *db = NULL;

    int rc = sqlite3_open(DB_NAME, &db);
    is_sqlite3_error(rc, __LINE__, NULL);

    select_db(db, head);

    is_sqlite3_error(sqlite3_close(db),__LINE__, NULL);

    return *head;
}

#if 0
int main()
{
    peo_p newnode;

    newnode = (peo_p)malloc(NODE_LEN);

     newnode->id = 1;
     strcpy(newnode->name,"name");
     strcpy(newnode->cell,"cell");
     strcpy(newnode->address,"adress");
     strcpy(newnode->company_tel,"tel");

     save_db(&newnode);

    return 0;
}
#endif
