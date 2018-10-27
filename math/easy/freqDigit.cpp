#include <stdio.h>
#include <string.h>
#include <ctype.h>
 
//https://practice.geeksforgeeks.org/problems/find-the-most-frequent-digit/0
int freq(const char *val)
{
    if (!val)
        return 0;

    int res[10]; // number of digits
    memset(res, 0, sizeof(res));

    for(int i = 0; val[i]; i++)
    {
        res[val[i] - '0']++;
    }

    int freq_digit = 0;
    int freq_count = 0;
    for (int i = 0; i < 10; i++)
    {
        if (res[i] >= freq_count)
        {
            freq_count = res[i];
            freq_digit = i;
        }
    }
    return freq_digit;
}

void test(const char *val) {
    printf("%s: %d\n", val, freq(val));
}

int main() {
    test("15");
    test("115");
    test("1122334455566");
    return 0;
}
