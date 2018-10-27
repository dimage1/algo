#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <vector>
 
using namespace std;

//https://practice.geeksforgeeks.org/problems/sort-the-given-array-after-applying-the-given-equation/0
void abcSort(int a, int b, int c, vector<long> &arr)
{
    for (size_t i = 0; i < arr.size(); i++)
    {
        long val = arr[i];
        arr[i] = val * val * a + val * b + c;
    }

    std::sort(arr.begin(), arr.end());
}

void test(int a, int b, int c, vector<long> &arr) {
    printf("a=%d, b=%d, c=%d: ", a, b, c);
    for (long v: arr)
        printf("%ld ", v);
    printf(" >>> ");
    abcSort(a, b, c, arr);
    for (long v: arr)
        printf("%ld ", v);
    printf("\n");
}

int main() {
    vector<long> arr = {-1, 0, 1, 2, 3, 4};
    test(-1, 2, -1, arr);

    vector<long> arr2 = {-32175, -32027, -31935, 
        -31925, -29917, -28207, -27083, -26423, -25219, -22771, 
        -22751, -22017, -21025, -20707, -20589, -19419, -18157, 
        -17135, -17119, -17031, -15841, -15171, -15101, -13745, -13493,
        -11031, -10695, -9815,  -9649,  -8499,  -6025,  -5707,
        -3723,  -771,    2550,   3630,   3902,   6368, 7584, 
         7872,   8790,   9180,   9512,   10162,   10200, 11590, 
         12336,  14996,  16330,  17010,   17526,  20370, 21108, 22102,
         24886, 25092, 25890, 26282, 27028, 29608, 30288, 30496, 31522, 31866};
    test(-25817, 8824, -8809, arr2);
    return 0;
}
