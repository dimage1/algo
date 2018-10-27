#include <stdio.h>
#include <string.h>
#include <ctype.h>
 
//https://practice.geeksforgeeks.org/problems/check-if-a-number-is-divisible-by-8/0
int div8(const char *val)
{
    if (!val)
        return -1;

    size_t len = strlen(val);
    int hund = 0;
    int decs = 0;
    int ones = 0;
    if (len > 2)
        hund = val[len - 3] - '0';
    if (len > 1)
        decs = val[len - 2] - '0';
    if (len > 0)
        ones = val[len - 1] - '0';

    return (hund * 100 + decs * 10 + ones) % 8 ? -1 : 1;
}

int main() {
    printf("15: %d\n",            div8("15"));
    printf("16: %d\n",            div8("16"));
    printf("1231231232: %d\n",    div8("1231231232"));
    printf("1298931231213: %d\n", div8("1298931231213"));
    return 0;
}
