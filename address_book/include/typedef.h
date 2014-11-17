#include<malloc.h>

#define SUCCESS 1
#define FAIL 0
#define MALLOC_ERROR -1

struct friend
{
    long int Id;
    char Name[20];
    char Address[30];
    char Phone_num[20];
    char Firm_num[20];

    struct friend *next;
    struct friend *prior;
};

typedef struct friend * link;
