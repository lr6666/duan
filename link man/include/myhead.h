#include <stdio.h>
#include <stdio_ext.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <sqlite3.h>

#define PEOPLE_MSG struct people_msg
#define NODE_LEN sizeof(PEOPLE_MSG)
#define SIZE_STR 20
#define SIZE 100

#define LESS -1                           //比较大小结果
#define EQUAL 0
#define GREATER 1

#define F_DELETE 1                        //搜索操作选项
#define F_MODIFY 2

#define SUCCESS 0                         //删除结果
#define FAIL -1

#define FILENAME "./lib/1.txt"            //文件名
#define DB_NAME "./lib/address_list.db"   //数据库名
#define TABLE_NAME "address_list"         //表名
#define HEAD_FORM "id,name,cell,adress,company_tel"
                                          //表头
//#define CREATE_ROEM "id integer primary key,name text,cell text,adress text,company_tel text"

enum menu
{
    INPUT_ERROR = -1,
    ADD = 1,
    DISPLAY,
    FIND,
    DELETE,
    SAVE,
    SELECT_EXIT
};

enum ret
{
    RETURN_SUCCESS = 0,
    MALLOC_FAIL = 1,
    NO_FIND,
    LINK_NULL,
    LINK_N_NULL
};

PEOPLE_MSG
{
    int id;
    char name[SIZE_STR];
    char cell[SIZE_STR];
    char adress[SIZE];
    char company_tel[SIZE_STR];

    PEOPLE_MSG *prior;
    PEOPLE_MSG *next;
};

typedef PEOPLE_MSG * peo_p;

extern peo_p create_node(peo_p *);
extern int is_null_link(peo_p);
extern peo_p insert_head_node(peo_p *, peo_p *);
extern int max_count_node(peo_p);
extern peo_p insert_data(peo_p *);
extern peo_p sort_link_name(peo_p *);
extern void display_link(peo_p);
extern int delete_node_id(peo_p *, int);
extern int delete_node(peo_p *);
extern int find_name(peo_p, peo_p * ,char *);
extern int find(peo_p *);
extern peo_p node_cpy(peo_p *, peo_p);
extern int mystrcmp(char *, char *);
extern char *mystrcpy(char *, char *);
extern int menu();
extern void print_n_ch(char, int);
extern void save_file(peo_p);
extern peo_p getdata_file(peo_p *);
extern peo_p get_data(peo_p *);
extern void save_data(peo_p);
