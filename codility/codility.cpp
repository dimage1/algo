// Arrays: CyclicRotation - Rotate an array to the right by a given number of steps.
#include <algorithm>

vector<int> solution(vector<int> &A, int K) {
    if (A.size() <= 1 || K == 0) {
        return A;
    }

    for (int i = 0; i < K; i++) {
        int last = *A.rbegin();
        for (size_t j = 0; j < A.size(); j++) {
                int tmp = A[j];
                A[j] = last;
                last = tmp;
        }
    }

    return A;
}

// Time: TapeEquilibrium -- Minimize the value |(A[0] + ... + A[P-1]) - (A[P] + ... + A[N-1])|.
#include <algorithm>
#include <limits.h>

int solution(vector<int> &A) {
    if (A.size() < 2) {
        return -1;
    }

    int totalSum = 0;
    for (auto &a: A) {
        totalSum += a;
    }

    int minDiff = INT_MAX; // > 2k * 100k
    int sum1 = 0;
    int sum2 = totalSum;
    for (size_t i = 0; i < A.size() - 1; i++) {
        sum1 += A[i];
        sum2 -= A[i];
        int diff = std::abs(sum1 - sum2);
        if (diff < minDiff) {
            minDiff = diff;
        }
    }
    
    return minDiff;
}


// ??
int solution(vector<int> &A) {
    std::sort(A.begin(). A.end());

    int firstNum = 0;
    for (const int &a: A) {
        if (firstNum == 0)
            firstNum = a;
        else if (firstNum == a)
            firstNum = 0; // pair found
        else
            break; // unpair val found !
        
    }

    return firstNum;
}

 // Time: FrogJmp - Count minimal number of jumps from position X to Y.
 int solution(int X, int Y, int D) {
    int distance = Y - X;
    return (distance % D == 0) ? (distance / D) : (distance / D) + 1;
}

// Time: PermMissingElem - Find the missing element in a given permutation.
#include <algorithm>
int solution(vector<int> &A) {
    if (A.size() == 0) {
        return 1;
    }

    std::sort(A.begin(), A.end());

    for (size_t i = 0; i < A.size(); i++) {
        if (A[i] != static_cast<int>(i + 1)) {
            return i + 1;
        }
    }
    return A.size() + 1;
}

// Counting: FrogRiverOne - Find the earliest time when a frog can jump to the other side of a river.
#include <algorithm>
int solution(int X, vector<int> &A) {

    int requiredPos = X;
    bool filledPos[X+1] = {};

    for (size_t i = 0; i < A.size() ; i++) {
        if (filledPos[A[i]] == false) {
            filledPos[A[i]] = true;
            requiredPos--;
            if (requiredPos == 0) {
                return i;
            }
        }
    }

    return -1;
}

//Counting: PermCheck - Check whether array A is a permutation.
#include <algorithm>

int solution(vector<int> &A) {
    std::sort(A.begin(), A.end());

    for (size_t i = 0; i < A.size(); i++) {
        if (A[i] != static_cast<int>(i+1))
            return 0;
    }

    return 1;
}

// Greedy: MaxNonoverlappingSegments -- Find a maximal set of non-overlapping segments.
#include <algorithm>
#include <vector>

int solution(vector<int> &A, vector<int> &B) {
    vector<size_t> segs;
    segs.reserve(A.size());

    for (size_t i = 0; i < A.size(); i++) {
        bool overlap = false;
        // check overlapping for the last storing segment
        if (segs.size() > 0) {
            int lastSeg = *segs.rbegin();
            if (B[lastSeg] >= A[i]) {
                overlap = true;
            }

        }

        if (!overlap) {
            segs.push_back(i);
        }
    }

    return segs.size();
}

// Greedy: TieRopes -- Tie adjacent ropes to achieve the maximum number of ropes of length >= K.
#include <algorithm>

int solution(int K, vector<int> &A) {
    int ropesNum = 0;
    int lastSum = 0;
    for (auto const a: A) {
        lastSum += a;
        if (lastSum >= K) {
            ropesNum++;
            lastSum = 0;
        }
    }
    return ropesNum;
}

// Euclidian: ChocolatesByNumbers - There are N chocolates in a circle. Count the number of chocolates you will eat.
#include <algorithm>

int gcd(int a, int b) {
    if (a % b == 0) {
        return b;
    }
    else {
        return gcd(b, a % b);
    }
}

int solution(int N, int M) {
    long lcm = (long)N * M / gcd(N, M); // 1kkk * 1kkk
    long eaten = lcm / M;

    return (int)eaten;
}

// Binary search in O(log n).
int binarySearch(A, x):
	size_t n = A.size();
	int beg = 0;
	int end = n - 1;
	int result = -1;
	while (beg <= end) {
		mid = (beg + end) / 2;
		if (A[mid] <= x) {
			beg = mid + 1;
			result = mid;
		}
		else {
			end = mid - 1;
		}
	return result;


// Binary search: minimize the large sum in the array.
#include <algorithm>
#include <cstdint>

int solution(int K, int M, vector<int> &A) {
    vector<int> B(A.size());

    int sum = 0;
    for (size_t i = 0; i < A.size(); i++) {
        sum += A[i];
        B[i] = sum;
    }

    int res = INT32_MAX;
    for (size_t i = 0; i < A.size(); i++) {
        int sum1 = B[i];
        //int sum23 = sum - B[i];
        for (size_t j = i+1; j < A.size(); j++) {
            int sum2 = B[j] - B[i];
            int sum3 = sum - sum2 - sum1;

            int maxSum = std::max(sum1, std::max(sum2, sum3));
            if (res > maxSum) {
                res = maxSum;
            }
        }
    }

    return res;
}

// Dynamic: Hard - 54% -- Given array of integers, find the lowest absolute sum of elements.
#include <vector>
using namespace std;

int sum(vector<int>& A, size_t i, int s)
{
   if (i == A.size())     
      return s;

   int tmpl = s + A[i];
   int tmpr = s - A[i];
   return min (abs(sum(A, i+1, tmpl)), abs(sum(A, i+1, tmpr)));
}

int solution(vector<int> &A) {
   return sum(A, 0, 0);   
}

// Improve ^^
#include <cmath>
#include <algorithm>
#include <numeric>

int solution(vector<int> &A) {
    // write your code in C++14 (g++ 6.2.0)
    const int N = A.size();
    int M = 0;
    for (int i=0; i<N; i++) {
        A[i] = abs(A[i]);
        M = max(M, A[i]);
    }
    int S = std::accumulate(A.begin(), A.end(), 0);
    std::vector<int> counts(M+1, 0);
    for (int i=0; i<N; i++) {
        counts[A[i]]++;
    }
    std::vector<int> dp(S+1, -1);
    dp[0] = 0;
    for (int a=1; a<M+1; a++) {
        if (counts[a] > 0) {
            for (int j=0; j<S; j++) {
                if (dp[j] >= 0) {
                    dp[j] = counts[a];
                } 
                else if ((j >= a) && (dp[j-a] > 0)) {
                    dp[j] = dp[j-a]-1;
                }
                else {
                }
            }
        }
    }
    int result = S;
    for (int i =0; i<(S/2+1); i++) {
        if (dp[i] >= 0) {
            result = std::min(result, S-2*i);
        }
    }
    return result;
}

// Dynamic: NumberSolitaire -- In a given array, find the subset of maximal sum in which the distance between consecutive elements is at most 6.
#include <algorithm>
#include <limits.h>

int solution(vector<int> &A) {
    // A: -10k .. 10k; A.size(): 2 .. 100.000
    int cache[100000] = {};
    bool cacheInit[100000] = {};

    cache[0] = A[0];
    cacheInit[0] = true;
    for (size_t i = 1; i < A.size(); i++) {
        int maxSum = INT_MIN;

        // check all 6 try options
        for (size_t k = 1; k <= 6 && i >= k; k++) {
            size_t pos = i - k;
            int sum = A[i] + cache[pos];
            if (maxSum < sum) {
                maxSum = sum;
            }
        }
        cache[i] = maxSum;
    }

    return cache[A.size() - 1];
}
[1,1]
[0,0]
[1,-1,-2,-3,1,100]
[1,-1,-2,-3,-4,-5,-6,1,100]
[1,-1,-2,-3,-4,-5,16,1,100]
[100, 8]
[-10, 1, 10]
[-10, -1, 10]

//AbsDistinct: Caterpillar method -- Compute number of distinct absolute values of sorted array elements.
#include <algorithm>
#include <set>

int solution(vector<int> &A) {
    int result = 0;

    for (int &a: A) {
        a = std::abs(a);
    }

    //std::set<int> nonDistinct;
    int j = static_cast<int>(A.size()) - 1;
    int i = 0;
    int prevJ = 0;
    int prevI = 0;
    while (i <= j) {
        result++;
        if (A[j] > A[i]) {
            if (j != (int)A.size() - 1 && A[j] == prevJ) {
                result--;
            }
            prevJ = A[j];
            j--;
        }
        else if (A[j] < A[i]) {
            if (i > 0 && A[i] == prevI) {
                result--;
            }
            prevI = A[i];
            i++;
        }
        else {
            if (i > 0 && A[i] == prevI) {
                result--;
            }
            prevJ = prevI = A[j];
            j--;
            i++;
        }


    }
    return result;
}

// Prefix Sums: PassingCars -- Count the number of passing cars on the road.
#include <algorithm>

int solution(vector<int> &A) {
    
    vector<int> pairs;
    pairs.reserve(A.size());

    int result = 0;
    int cumul1 = 0;
    for (int i = static_cast<int>(A.size()); i >=0; i--) {
        if (A[i] == 1) {
            cumul1++;
        }
        else {
            result += cumul1;
            if (result > 1'000'000'000) {
                result = -1;
                break;
            }
        }
    }
    return result;
}

// Prefix Sums: CountDiv - Compute number of integers divisible by k in range [a..b].
int solution(int A, int B, int K) {
    if (K > B) {
        return 0;
    }

    B -= B % K;

    int divA = A % K; 
    if (divA > 0) {
        A += K - divA;
    }

    return (B - A) / K + 1;    
}
[0,2000000000,1]
[0,2000000000,2]
[2000000000,2000000000,2]
[0,2000000000,2]
[1,1,2000000000]
[1,1999999999,1000000000]

// Sort: distinct values -- Compute number of distinct values in an array.
#include <algorithm>
int solution(vector<int> &A) {
    std::sort(A.begin(), A.end(), [](int x, int y) {return x < y;});

    int result = 0;
    for (size_t i = 0; i < A.size(); i++) {
        if (i == 0) {
            result++;
        }
        else if (A[i] != A[i-1]) {
            result++;
        }
    }
    return result;
}

// Sort: MaxProductOfThree -- Maximize A[P] * A[Q] * A[R] for any triplet (P, Q, R).
#include <algorithm>
int solution(vector<int> &A) {
    std::sort(A.begin(), A.end());

    int s1 = A[0] * A[1] * A[2];
    int s2 = A[0] * A[1] * A[A.size() - 1];
    int s3 = A[0] * A[A.size() - 2] * A[A.size() - 1];
    int s4 = A[A.size() - 3] * A[A.size() - 2] * A[A.size() - 1];

    vector<int> result;
    result.push_back(s1);
    result.push_back(s2);
    result.push_back(s3);
    result.push_back(s4);
    std::sort(result.begin(), result.end());

    return *result.rbegin();
}

// Sort: NumberOfDiscIntersections -- Compute the number of intersections in a sequence of discs.
#include <algorithm>

typedef struct range_{
    long x = 0;
    long y = 0;
}range_t;

int solution(vector<int> &A) {
    if (A.size() < 2) {
        return 0;
    }

    vector<range_t> B;
    B.resize(A.size());

    for (size_t i = 0; i < A.size(); i++) {
        B[i].x = i - A[i];
        B[i].y = i + A[i];
    }

    std::sort(B.begin(), B.end(), [](range_t &r1, range_t &r2) { return r1.x < r2.x; });

    int result = 0;
    for (size_t i = 0; i < B.size(); i++) {
        auto &r = B[i];
        for (size_t j = i + 1; j < B.size(); j++) {
            auto &f = B[j];
            if (r.y >= f.x) {
                result++;
                if (result > 10'100'000) {
                    return -1;
                }
            } 
            else {
                break;
            }
        }
    }

    return result;
}

// Sort: Triangle Triplets : Determine whether a triangle can be built from a given set of edges.
#include <algorithm>

int solution(vector<int> &A) {
    // should be at least 3 elems inside
    if (A.size() < 3) {
        return 0;
    }

    // sort array for a fast triplets search
    std::sort(A.begin(), A.end());

    int result = 0;
    // check every sorted triplet
    for (int i = (int)A.size() - 3; i >= 0 && A[i] >= 0; i--) {
        // cast to long to support edge cases (e.g. MAX_INT, MAX_INT, MAX_INT)
        if (static_cast<long>(A[i]) + A[i+1] > A[i+2]) {
            result = 1;
            // stop after thw result is found
            break;
        }
    }

    return result;
}

//Stack - brackets : - Determine whether a given string of parentheses (multiple types) is properly nested.
#include <algorithm>
#include <vector>

int solution(string &S) {
    std::vector<char> brackets;
    brackets.reserve(S.size() + 1);
    brackets.push_back('.'); // initial

    for (char &c: S) {
        char last = *brackets.rbegin();

        switch (c) {
            case '(':
            case '[':
            case '{': brackets.push_back(c); break;
            case ')': {if (last == '(') brackets.pop_back(); else return 0;} break;
            case ']': {if (last == '[') brackets.pop_back(); else return 0;} break;
            case '}': {if (last == '{') brackets.pop_back(); else return 0;} break;
        }
    }

    return brackets.size() == 1 ? 1 : 0;
}

// stack: Fish -- N voracious fish are moving along a river. Calculate how many fish are alive.
// you can use includes, for example:
#include <algorithm>
#include <vector>

int solution(vector<int> &A, vector<int> &B) {
    std::vector<size_t> alive;
    alive.reserve(A.size());

    for (size_t i = 0; i < A.size(); i++) {
        if (alive.empty()) {
            alive.push_back(i);
            continue;
        }

        while (alive.size() > 0) {
            size_t lastFish = *alive.rbegin();
            if (B[lastFish] == B[i] || B[lastFish] == 0) {
                alive.push_back(i);
                break;
            }
            else if (A[lastFish] > A[i]) {
                // lastFish eats the new one
                break;
            }
            else {
                alive.pop_back(); // new fish eats the prev one
            }
        }

        if (alive.empty()) {
            alive.push_back(i);
        }
    }

    return alive.size();
}

([9,10,1], [1,0,0])
([19,10,1], [1,0,0])
([20,19,10,1], [1,1,0,0])
([20,19,10,1], [1,1,0,0])

// Leader: Dominator -- Find an index of an array such that its value occurs at more than half of indices in the array.
int solution(vector<int> &A) {
    
    if (A.size() < 1) {
        return -1;
    }
    if (A.size() == 1) {
        return 0;
    }

    int val = 0;
    int size = 0;
    size_t index = 0;

    for (size_t i = 0; i < A.size(); i++) {
        if (size == 0) {
            val = A[i];
            index = i;
            size = 1;
        }
        else if (val == A[i]) {
            size++;
        }
        else {
            size--;
        }
    }

    size_t count = 0;
    for (const int &a: A) {
        if (a == val) {
            count++;
        }
    }

    return (count > A.size() / 2) ? static_cast<int>(index) : -1;
}
[1]
[0]
[1,1]
[1,0]
[1,1,1,2,2,2,2]
[1,1,1,3,3,3,3,3,3,3,2,2,2]

// Leader: EquiLeader -- Find the index S such that the leaders of the sequences A[0], A[1], ..., A[S] and A[S + 1], A[S + 2], ..., A[N - 1] are the same.

int solution(vector<int> &A) {
    int pos = solution1(A); // Leader (check above)
    if (pos == -1) {
        return 0;
    }

    int val = A[pos];
 
    vector<int> B;
    B.reserve(A.size());
    int count = 0;
    for (size_t i = 0; i < A.size(); i++) {
        if (A[i] == val) {
            count++;
        }
        B[i] = count;
    }

    int result = 0;
    for (int i = 0; i < (int)A.size(); i++) {
        int sLeft = B[i];
        int sRight = count - B[i];
        if (sLeft > (i+1)/2 && sRight > (A.size() - i - 1) / 2) {
            result++;
        }
    }

    return result;
}

// MaxSliceSum - Easy: Find a maximum sum of a compact subsequence of array elements.
#include <algorithm>
#include <cstdint>

int solution(vector<int> &A) {
    int maxEnding = 0;
    int maxSlice = 0;

    int maxA = INT32_MIN;
    for (int a: A) {
        maxEnding = std::max(0, maxEnding + a);
        maxSlice = std::max(maxSlice, maxEnding);
        maxA = std::max(a, maxA);
    }

    if (maxSlice == 0 && maxA < 0) {
        return maxA;
    }

    return maxSlice;
}


// MaxSlice Easy: MaxProfit -- Given a log of stock prices compute the maximum possible earning.
#include <algorithm>

int solution(vector<int> &A) {
    if (A.size() <= 1) {
        return 0;
    }

    int prev = A[0];
    for (int &a: A) {
        int tmp = a - prev;
        prev = a;
        a = tmp;
    }

    int maxEnding = 0;
    int maxSlice = 0;
    for(int a: A) {
        maxEnding = std::max(0, maxEnding + a);
        maxSlice = std::max(maxSlice, maxEnding);
    }

    return maxSlice;
}

// Prime numbers: Reversing coins — O(n log n).
def coins(n):
	result = 0
	coin = [0] * (n + 1)
	for i in xrange(1, n + 1):
		k = i
		while (k <= n):
			coin[k] = (coin[k] + 1) % 2
			k += i
		result += coin[i]
		
// PrimeNumbers: CountFactors -- Count factors of given number n.
// big3
N=479,001,600=12!, N=780291637(prime), N=449,991,369✘TIMEOUT ERROR
running time: 2.288 sec., time limit: 0.100 sec.
▶extreme_maxint
N=1,000,000,000, N=MAX_INT, N=2147,395,600
#include <algorithm>

int solution(int N) {

    int prevResult = 0;
    int result = 0;
    for (int i = 2; i <= N; i++) {
        for (; N % i == 0; N /= i) {
            result += prevResult + 1; // count the divisor itself
        }
        prevResult = result;
    }

    return result + 1; // +1 as all numbers are devied by 1
}

// 1. StrSymmetryPoint -- Find a symmetry point of a string, if any.
#include <algorithm>

int solution(string &S) {
    if (S.size() == 1) {
        return 0;
    }

    // must be odd number of chars. skip 0 size as well
    if (S.size() % 2 == 0) {
        return -1;
    }

    int pos = static_cast<int>(S.size()) / 2;
    for (int i = 0; i + pos < (int)S.size(); i++) {
        if (S[pos - i] != S[pos + i]) {
            pos =- 1;
            break;
        }
    }

    return pos;

}

//  TreeHeight -- Compute the height of a binary tree.
#include <algorithm>
#include <stack>

int solution(tree * T) {
    
    if (T == nullptr) {
        return -1;
    }

    std::stack<tree*> st;
    st.push(T);

    size_t maxSize = 0;
    while (st.size()) {
        size_t treeSize = st.size() - 1;
        if (maxSize < treeSize) {
            maxSize = treeSize;
        }
        
        tree *t = st.top();
        if (t->l) {
            st.push(t->l);
            t->l = nullptr;
        }
        else if (t->r) {
            st.push(t->r);
            t->r = nullptr;
        }
        else {
            st.pop();
        }
    }

    return (int)maxSize;
}

// Reversed pairs
// Simple - stupid:
#include <algorithm>

int solution(vector<int> &A) {
    if (A.size() <= 1) {
        return 0;
    }

    int count = 0;

    for (size_t i = 0; i < A.size() - 1; i++) {
        for (size_t j = i; j < A.size(); j++) {
            if (A[i] > A[j]) {
                count++;
                if (count > 100'000) {
                    count = -1;
                    break;
                }
            }
        }
    }

    return count;
}
int solution(vector<int> &A) {
    if (A.size() <= 1) {
        return 0;
    }

    vector<int> B;
    B.resize(A.size(), 0);

    int count = 0;
    for (int i = static_cast<int>(A.size()) - 1; i >=0; i--) {
        for (int j = i + 1; j < static_cast<int>(A.size()); j++) {
            if (A[i] > A[j]) {
                B[i] = B[j] + 1;
                std::cout << A[i] << ">" << A[j] << ", B[i]=" << B[i] << std::endl;
                count += B[i];
                if (count > 100'000) {
                    return -1;
                }
                break;
            }
        }
    }

    return count;
}

// ParityDegree - Find the highest power of 2 that divides N.
#include <algorithm>

int solution(int N) {
    
    int power = 0;
    while((N & 1) == 0) {
        N >>= 1;
        power++;
    }

    return power;
}

// ThreeLetters - return a string which contains A letters "a" and B letters "b" with no three consecutive letters being the same.
#include <algorithm>
#include <stack>

#include <algorithm>

string solution(int A, int B) {
    if (A == 0) {
        switch(B) {
            case 0: return "";
            case 1: return "b";
            case 2: return "bb";
        }
    }
    if (B == 0) {
        switch(A) {
            case 0: return "";
            case 1: return "a";
            case 2: return "aa";
        }
    }

    std::string s;
    s.reserve(A+B);

    if (A > B) {
        int aDiv = B / A;
        while (A) {
            s.push_back('a');
            A--;
            if (aDiv > 1 && A > 1) {
                s.push_back('a');
                A--;
            }
            if (B > 0) {
                s.push_back('b');
                B--;
            }
        }
    }
    else
    if (B > A) {
        int bDiv = B / A;
        while (B) {
            s.push_back('b');
            B--;
            if (bDiv > 1) {
                s.push_back('b');
                B--;
            }
            if (A > 0) {
                s.push_back('a');
                A--;
            }
        }
    }
    else {
        for (int i = 0; i < A; i++) {
            s.push_back('a');
            s.push_back('b');
        }
    }


    return s;
}

// LongestPassword -- Given a string containing words, find the longest word that satisfies specific conditions.
#include <algorithm>

bool isLetter(char c) {
    return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
}

int solution(string &S) {
    //add space in the end
    S.push_back(' ');

    int maxPassword = -1;

    bool wordFound = false;
    bool wordOk = true;
    int letters = 0;
    int digits = 0;
    for (char c: S) {
        if (c == ' ') {
            if (wordFound && wordOk && (digits & 1) && !(letters & 1)) {
                if (letters + digits > maxPassword) {
                    maxPassword = letters + digits;
                }
            }
            wordFound = false;
            wordOk = true;
            letters = 0;
            digits = 0;
        }
        else if (wordOk) { // skip not OK sequences
            wordFound = true;
            if (isLetter(c)) {
                letters++;
            }
            else if(std::isdigit(c)) {
                digits++;
            }
            else {
                wordOk = false;
            }
        }
    }

    return maxPassword;
}

// ArrListLen -- Compute length of a single-link list encoded in an array.
#include <algorithm>

int solution(vector<int> &A) {
    int len = 1;

    int i = 0;
    while (1) {
        i = A[i];
        if (i == -1) {
            break;
        }
        len++;
    }
    return len;
}


//FirstUnique -- Find the first unique number in a given sequence.
#include <algorithm>
#include <map>

int solution(vector<int> &A) {
    if (A.size() == 1) {
        return *A.begin();
    }

    std::map<int, std::tuple<int, size_t>> valMap;

    for (size_t i = 0; i < A.size(); i++) {
        const auto f = valMap.find(A[i]);
        if (f == valMap.end()) {
            valMap[A[i]] = std::make_tuple(1,i);
        }
        else {
            std::get<0>(f->second)++;
        }
    }

    size_t minPos = A.size();
    for (const auto a: valMap) {
        if (std::get<0>(a.second) == 1 && std::get<1>(a.second) < minPos) {
            minPos = std::get<1>(a.second);
        }
    } 

    return minPos < A.size() ? A[minPos] : -1;
}