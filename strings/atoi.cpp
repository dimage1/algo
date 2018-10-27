#include <stdio.h>
#include <string.h>
#include <ctype.h>
 
int myatoi(const char *str)
{
    int result = 0;
    int sign = 1;
    int i = 0;

    if (str == NULL)
        return 0;

    if (str[0] == '-')
    {
        sign = -1;
        i++;
    }
    
    for (; str[i]; i++)
    {
        if (!isdigit(str[i]))
            break;

        result = 10 * result + str[i] - '0';
    }

    return result * sign;
}

int main() {
    printf("Test1: %d\n", myatoi("1"));         // 1
    printf("Test2: %d\n", myatoi("-1"));        // -1
    printf("Test3: %d\n", myatoi("1234567890"));// 1234567890
    printf("Test4: %d\n", myatoi(""));          // 0
    printf("Test5: %d\n", myatoi("test"));      // 0
    printf("Test6: %d\n", myatoi("22test"));    // 22
    printf("Test6: %d\n", myatoi("89test123")); // 22
    printf("Test7: %d\n", myatoi(NULL));        // 0
    return 0;
}
