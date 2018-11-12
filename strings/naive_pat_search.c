#include <stdio.h>
#include <string.h>
#include <ctype.h>
 
static size_t calls;

char hohoho[] = "HOHOHOHO";
size_t found = 0;

//https://www.geeksforgeeks.org/naive-algorithm-for-pattern-searching/
// O(m(n-m+1))
static void search(const char *pat, const char *text)
{
    calls++;
    
    if (!pat || !text)
        return;

    size_t patsz = strlen(pat);

    const char *cur = text;
    while (cur) {
        cur = strstr(cur, pat);
        if (cur) 
        {
            found++;
            printf("Pattern found at %lu\n", cur - text);
            cur++;
        }
    }
    printf("-----\n");
}

int main() {

    char str[] = "THIS IS A TEST TEXT";

    search("TEST", str);
    search("IS",   "IS IS IS");
    search("AABA", hohoho);

    hohoho[1] = 'E';
    search("AABA", hohoho);
    search("AA",   "AAAAAAAAAAAAAAAA");

    return 0;
}
