// 1. SocksLaundering - From drawers containing both clean and dirty socks, choose socks to launder in order to obtain the maximum number of clean pairs of socks.
#include <algorithm>

int solution(int K, vector<int> &C, vector<int> &D) {
    int result = 0;

    std::sort(C.begin(), C.end());
    std::sort(D.begin(), D.end());

    // delete & count clean pairs
    size_t i = 0;
    while (C.size() && i < C.size()) {
        if (i + 1 < C.size() && C[i] == C[i+1]) {
            result++;
            C.erase(C.begin() + i);
            C.erase(C.begin() + i);
        }
        else {
            i++;
        }
    }

    // count 1 clean & 1 dirty
    size_t cPos = 0;
    size_t dPos = 0;
    while (K > 0 && cPos < C.size() && dPos < D.size()) {
        if (C[cPos] == D[dPos]) {
            D.erase(D.begin() + dPos);
            result++;
			cPos++;
            K--;
        }
        else if (C[cPos] > D[dPos]) {
            dPos++;
        }
        else {
            cPos++;
        }
    }

    // count 2 dirty
    dPos = 0;
    while (K > 1 && dPos + 1 < D.size()) {
        if (D[dPos + 1] == D[dPos]) {
            dPos += 2;
            result++;
            K -= 2;
        }
        else {
            dPos++;
        }
    }

    return result;
}

// AVG: FloodDepth - Find the maximum depth of water in mountains after a huge rainfall.
#include <algorithm>
#include <stack>

int solution(vector<int> &A) {
    int result = 0;
    std::vector<int> middle;
    middle.reserve(A.size());

    int left = 0;
    for (size_t i = 0; i < A.size(); i++) {
        int right = A[i];
        if (right >= left || i == A.size() - 1) {
            if (left != 0) {
                for (int m: middle) {
                    int diff = left - m;
                    if (diff > result) {
                        result = diff;
                    }
                }
                middle.clear();
            }
            left = a;
        }
        else {
            middle.push_back(a);
        }
    }

    return result;
}