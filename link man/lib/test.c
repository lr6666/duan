#include <stdio.h>
extern int mystrcmp(char *, char *);

int main()
{
    char str[10] = "1234";
    char src[10] = "abcd";

    mystrcpy(str,src);
    mystrcmp(str,src);
    return 0;
}
