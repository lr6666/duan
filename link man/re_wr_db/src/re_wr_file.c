#include "../../include/myhead.h"

/******************************************************************
Function:                char *invert_str(char *src)
Description:             字符串逆序
Calls:                   no
Called By:               char *int2str(char *, int)
Input:                   char *src:原字符串
Return:                  char *deat:目的字符串
******************************************************************/

char *invert_str(char *src)
{
    int len = strlen(src);
    int i = 0;
    /*首尾换位*/
    for(; i < len / 2; i++)
    {
        char temp;                            //交换临时变量

        temp = *(src + i);
	*(src + i) = *(src + len - 1 - i);
	*(src + len - 1 - i) = temp;
    }

    return src;
}

/******************************************************************
Function:                char *int2str(char *deat, int src)
Description:             整数转成字符串
Calls:                   int main(int, char *)
Called By:               char *array2str(int *, int , char *)
Input:                   char *deat:目的字符串
                         int *src:原数据
Return:                  char *deat:目的字符串
******************************************************************/

char *int2str(char *deat, int src)
{
    char *temp = deat;                        //保留头指针

    while(src != 0)
    {
        *temp++ = src % 10 + '0';
	src /= 10;
    }
    *temp = '\0';

    invert_str(deat);

    return deat;
}


char * people2str(char *deat, peo_p srcnode)
{
    char *temp_id = (char *)malloc(sizeof(char) * SIZE);
    int2str(temp_id,srcnode->id);
    sprintf(deat,"%s:%s:%s:%s:%s\n", temp_id,srcnode->name,srcnode->cell,
        srcnode->adress,srcnode->company_tel);

    free(temp_id);
    return deat;
}
#if 0
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
#endif
peo_p str2people(char *src, peo_p *newnode)
{
    char *temp = src;
    char *temp_str = (char *)malloc(sizeof(char) * SIZE);
    char *first = temp_str;

    (*newnode)->id = str2int(temp);

    while(*temp != ':')
    {
        temp++;
    }
    temp++;

    while(*temp != ':')
    {
        *temp_str = *temp;
	temp++;
	temp_str++;
    }

    temp++;
    *temp_str = '\0';
    temp_str = first;
    strcpy((*newnode)->name, temp_str);

    while(*temp != ':')
    {
        *temp_str = *temp;
	temp++;
	temp_str++;
    }

    temp++;
    *temp_str = '\0';
    temp_str = first;
    strcpy((*newnode)->cell, temp_str);

    while(*temp != ':')
    {
        *temp_str = *temp;
	temp++;
	temp_str++;
    }

    temp++;
    *temp_str = '\0';
    temp_str = first;
    strcpy((*newnode)->adress, temp_str);

    while(*temp != '\0')                      //从文件读的时候'\n'变成'\0'
    {
        *temp_str = *temp;
	temp++;
	temp_str++;
    }

    temp++;
    *temp_str = '\0';
    temp_str = first;
    strcpy((*newnode)->company_tel, temp_str);

    free(temp_str);

    //printf("id = %d\t name = %s\t cell = %s\t adress = %s\t tel = %s\n",(*newnode)->id, (*newnode)->name, (*newnode)->cell, (*newnode)->adress, (*newnode)->company_tel);

    return *newnode;
}

/******************************************************************
Function:                char *read_file(char *, char *)
Description:             从文件读取一行数据
Calls:                   no
Called By:               int main(int, char *)
Input:                   char *deat:读出的数据
                         char *filename:文件名
Return:                  char *deat:读出的数据
******************************************************************/

int read_line(FILE *fp, char *buf, int count)
{
    int i;
    char ch;
    for(i = 0; i < count - 1; i++)
    {
        if((ch = fgetc(fp)) == EOF)
        {
	     perror("read error or file finish");
	     return EOF;
	}

	if(ch == '\n')
	{
             buf[i] = '\0';
             return i;
	}
	buf[i] = ch;
    }

	buf[i] = '\0';
	
	return i;
}

void save_file(peo_p head)
{
    FILE *fp;
    if((fp = fopen(FILENAME,"w")) == NULL)
    {
        perror("fopen error");
        return;//exit(1);
    }
    peo_p temp = head->next;
    while(temp != head)
    {
         char *str = (char *)malloc(sizeof(char) * SIZE);
    //printf("id = %d\t name = %s\t cell = %s\t adress = %s\t tel = %s\n",temp->next->id, temp->prior->name, temp->cell, temp->adress, temp->company_tel);

         people2str(str,temp);
                 
         if(fputs(str,fp) == EOF)
         {
              perror("fputs error");
         }

	 temp = temp->next;

	 free(str);
    }

    fclose(fp);
}

peo_p getdata_file(peo_p *head)
{
    char *buf = (char *)malloc(sizeof(char) * SIZE);

    FILE *fp = NULL;
    if((fp = fopen(FILENAME,"r")) == NULL)
    {
        perror("fopen error");
        return *head;	
    }

    while(read_line(fp, buf, SIZE) != EOF)
    {
        peo_p newnode;

	create_node(&newnode);

        str2people(buf,&newnode);

    //printf("get id = %d\t name = %s\t cell = %s\t adress = %s\t tel = %s\n",newnode->id, newnode->prior->name, newnode->cell, newnode->adress, newnode->company_tel);
	insert_head_node(head,&newnode);

    }
    fclose(fp);

    free(buf);
    return *head;
}



#if 0

int main()
{
    peo_p newnode;/* = {
     .id = 1,
     .name = "name",
     .cell = "cell",
     .adress = "adress",
     .company = "company_tel",
     .prior = NULL,
     .next = NULL
     };*/
    newnode = (peo_p)malloc(NODE_LEN);
/*
     newnode->id = 1;
     strcpy(newnode->name,"name");
     strcpy(newnode->cell,"cell");
     strcpy(newnode->adress,"adress");
     strcpy(newnode->company_tel,"tel");
     

    char *temp_str = (char *)malloc(sizeof(char) * SIZE);

    temp_str = people2str(temp_str, newnode);

    printf("str = %s\n", temp_str);
*/
    peo_p pe = (peo_p)malloc(NODE_LEN);

    str2people("2:xx:3213:sdfda:6543", &newnode);

    printf("id = %d  name = %s\n", newnode->id, newnode->name);

    free(pe);
    //free(newnode);

    return 0;
}
#endif
