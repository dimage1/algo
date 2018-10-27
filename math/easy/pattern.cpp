#include <stdio.h>
#include <string.h>
#include <ctype.h>
 
//https://practice.geeksforgeeks.org/problems/print-the-pattern-set-1/1
void printPat(int n)
{
    for (int dub = n; dub > 0; --dub)
    {
        for (int j = n; j > 0; --j)
        {
            for(int k = 0; k < dub; ++k)
                printf("%d ", j);
        }
        printf("$");
    }
    printf("\n");
}

int main() {
    printPat(1);
    printPat(2);
    printPat(3);
    printPat(4);
    return 0;
}
