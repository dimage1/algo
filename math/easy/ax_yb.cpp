#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
 
//https://practice.geeksforgeeks.org/problems/find-smallest-values-of-x-and-y-such-that-ax-by-0/0
void ax_yb(int a, int b)
{
    int div = a > b ? b : a;
    for (int i = div; i >= 2; i--)
    {
        if (!(a % i) && !(b % i))
        {
            a /= i;
            b /= i;
        }
    }

    printf("%d %d\n", b, a);
}

void test(int a, int b) {
    printf("%dx - %dy = 0 >>> ", a, b);
    ax_yb(a, b);
}

int main() {
    test(25,  35);
    test(35,  25);
    test(7,   8);
    test(590, 59);
    test(772, 536);
    return 0;
}
