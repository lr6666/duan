#include <assert.h>

#define GREATER 1
#define LESS -1
#define EQUAL 0

int mystrcmp(const char *str1, const char *str2)
{
    assert(str1);
    assert(str2);
    while((*str1) != '\0' && (*str2) != '\0')
    {
        if(*str1 > *str2)
	{
	    return GREATER; 
	}
	else if(*str1 < *str2)
	{
	    return LESS;
	}

	str1++;
	str2++;
    }

    if(*str1 == '\0' && *str2 =='\0')
    {
        return EQUAL;
    }
    else if(*str1 != '\0')
    {
        return GREATER;
    }

    return LESS;

}

#if 0
int main()
{
    printf("%d\n", mystrcmp("as","as"));
    printf("%d\n", mystrcmp("as","bs"));
    printf("%d\n", mystrcmp("at","as"));
    return 0;
}

#endif
