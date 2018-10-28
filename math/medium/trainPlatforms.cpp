#include <stdio.h>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>

using namespace std;

//https://practice.geeksforgeeks.org/problems/minimum-platforms/0
size_t minTrainPlatforms(const vector<int> &arriv, const vector<int> &depar)
{
    vector<int> all = arriv;
    all.insert(all.begin(), depar.begin(), depar.end());
    sort(all.begin(), all.end());

    map<int,int> dmap;
    for (auto a: depar)
    {
        auto it = dmap.find(a);
        if (it == end(dmap))
            dmap[a] = 1;
        else
            it->second++;
    }

    size_t max = 0;
    size_t plat = 0;
    for(const auto a: all)
    {
        auto it_d = dmap.find(a);

        if (it_d != end(dmap) && it_d->second > 0) {
            plat--;
            it_d->second--;
        }
        else {
            plat++;
            if (plat > max)
                max = plat;
        }
    }

    return max;
}

void test(const vector<int> &arriv, const vector<int> &depar) {
    size_t res = minTrainPlatforms(arriv, depar);
    printf("Res: %d\n", (int)res);
}

int main() {
    test({900, 940, 950, 1100, 1500, 1800},
        {910, 1200, 1120, 1130, 1900, 2000});

    test({635, 1032, 632, 940, 1141, 1509, 1359, 128, 123, 626, 313, 350,
        853, 1551, 54, 123, 357, 1500, 334, 1315, 131, 1008, 33, 926,
        907, 932, 452, 127, 704, 1854, 1718, 1924, 1337, 623, 720},
        {1202, 2100, 2151, 941, 1751, 1549, 1411, 949, 1500, 647, 2105,
         1350, 1552, 2300, 2022, 1238, 2013, 1543, 2300, 1332, 2125,
         1300, 200, 941, 924, 1030, 800, 2010, 755, 1857, 1829, 2335,
         1348, 1700, 1500});

    return 0;
}
