#include <stdio.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <climits>
 
//Amadeus: find 2 max elements in the array
void max2(std::vector<int> &arr)
{
    int max1 = 0;
    int max2 = 0;

    if (arr.size() == 1)
        max1 = max2 = arr.at(0);
    else
    {
        std::sort(arr.begin(), arr.end());
        max1 = arr.rbegin()[0];
        max2 = arr.rbegin()[1];
    }
    printf("%d %d\n", max1, max2);
}

void max2x(const std::vector<int> &arr)
{
    int max1 = INT_MIN;
    int max2 = INT_MIN;

    for (size_t i = 0; i < arr.size(); i++)
    {
        if (arr[i] > max1)
        {
            max2 = max1;
            max1 = arr[i];
        }
        else if (arr[i] > max2)
            max2 = arr[i];
    }
    printf("%d %d\n", max1, max2);
}

int main() {
    std::vector<int> v1 = {1,2,3,4,5,6,7,8,9};
    max2x(v1);
    std::vector<int> v2 = {123,13,445,667,87,5,345,2};
    max2x(v2);
    std::vector<int> v3 = {0,0,0,0,0};
    max2x(v3);

    return 0;
}
