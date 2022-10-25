#include <algorithm>


int solution(int A, int B) {
    long m = static_cast<long>(A) * B;

    int result = 0;
    for (; m > 0; m >>= 1) {
        if (m & 1) {
            result++;
        }
    }

    return result;
}
///

#include <algorithm>

int solution(vector<int> &blocks) {

    // count all ways for the frog jumping right
    vector<int> dpRight(blocks.size());
    dpRight[0] = 0;
    for (size_t i = 1; i < blocks.size(); i++) {
        if (blocks[i] >= blocks[i - 1]) {
            dpRight[i] = dpRight[i - 1] + 1;
        }
        else {
            dpRight[i] = 0;
        }
        cout << "right. " << i << ": " << dpRight[i] << endl;
    }

    // count all ways for the frog jumping left
    vector<int> dpLeft(blocks.size());
    dpLeft[blocks.size() - 1] = 1;
    for (int i = (int)blocks.size() - 2; i >= 0; i--) {
        if (blocks[i] >= blocks[i + 1]) {
            dpLeft[i] = dpLeft[i + 1] + 1;
        }
        else {
            dpLeft[i] = 0;
        }
        cout << "left. " << i << ": " << dpLeft[i] << "" << endl;
    }

    // finally calc the max distance
    int maxDistance = 0;
    for (size_t i = 0; i < blocks.size(); i++) {
        int distance = dpRight[i] + dpLeft[i];
        maxDistance = std::max(maxDistance, distance);
        cout << "max. " << i << ": " << maxDistance << endl;
    }

    return maxDistance + 1;
}


#include <algorithm>

// size: 2 .. 200k; elems: 1 .. 1kkk
int solution(vector<int> &blocks) {

    // count all ways for the frog jumping right
    vector<int> dpRight(blocks.size());
    dpRight[blocks.size() - 1] = 0;
    for (int i = static_cast<int>(blocks.size()) - 2; i >= 0; i--) {
        if (blocks[i] <= blocks[i + 1]) {
            dpRight[i] = dpRight[i + 1] + 1;
        }
        else {
            dpRight[i] = 0;
        }
    }

    // count all ways for the frog jumping left
    vector<int> dpLeft(blocks.size());
    dpLeft[0] = 0;
    for (size_t i = 1; i < blocks.size(); i++) {
        if (blocks[i] <= blocks[i - 1]) {
            dpLeft[i] = dpLeft[i - 1] + 1;
        }
        else {
            dpLeft[i] = 0;
        }
    }

    // finally calc the max distance
    int maxDistance = 0;
    for (size_t i = 0; i < blocks.size(); i++) {
        int distance = dpRight[i] + dpLeft[i];
        maxDistance = std::max(maxDistance, distance);
    }

    return maxDistance + 1;
}


