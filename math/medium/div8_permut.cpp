#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <array>

using namespace std;
 
enum{
    MAX_NUM = 2
};

//https://practice.geeksforgeeks.org/problems/divisible-by-8/0
bool div8Perm(int num)
{
    int perm[3];
    int digs[6];

    // 0 - 100000
    if (num > 100000)
        return false;

    if (num % 8 == 0)
        return true;

    if (num < 10)
        return false;
    
    size_t pos = 0;
    while (num) {
        digs[pos++] = num % 10;
        num /= 10;
    }
    if (2 == pos)
        return digs[1] + digs[0] * 10;

    //std::next_permutation
    for (int i = 0; i < pos; i++) {
        for (int j = 0; j < pos; j++) {
            for (int k = 0; k < pos; k++) {
                if (i != j && j != k && k != i) {
                    int val = digs[i] * 100 + digs[j] * 10 + digs[k];
                    if (val % 8 == 0)
                        return true;
                }
            }
        }
    }
    return false;
}

void test(int num) {
    printf("%d - %s\n", num, div8Perm(num) ? "Yes" : "No");
}

int main() {

    test(124);
    test(8);
    test(32);
    test(562);
    return 0;
}
