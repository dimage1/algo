#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <array>

using namespace std;
 
enum{
    MAX_NUM = 2
};

//"https://practice.geeksforgeeks.org/problems/sort-an-array-of-0s-1s-and-2s/0/?track=placement"
/*template<size_t SIZE>
void sort012(const array<int, SIZE> &arr)
{
    int total[MAX_NUM+1];
    memset(total, 0, sizeof(total));

    for (int v: arr) {
        total[v]++;
    }
    for (int i = 0; i <= MAX_NUM; i++) {
        for (int j = total[i]; j > 0; --j)
            printf("%d ", i);
    }
}*/

//https://en.wikipedia.org/wiki/Dutch_national_flag_problem
template<size_t SIZE>
void sort012(array<int, SIZE> &arr)
{
    size_t i = 0;
    size_t j = 0;
    size_t n = arr.size() - 1;

    while (j <= n) {
        if (arr[j] < 1) {
            swap(arr[i], arr[j]);
            i++;
            j++;
        }
        else if(arr[j] > 1) {
            swap(arr[j], arr[n]);
            n--;
        }
        else
            j++;
    }

    for (int i = 0; i < arr.size(); i++)
        printf("%d ", arr[i]);
}

template<size_t SIZE>
void test(array<int, SIZE> &arr) {
    for (int v: arr)
        printf("%d ", v);
    printf(" >>> ");
    sort012(arr);
    printf("\n");
}

int main() {

    array<int, 7> arr = {0, 1, 2, 0, 2, 1, 2};
    test(arr);
    return 0;
}
