#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <thread>
#include <unistd.h>
#include <queue>
#include <atomic>
#include <mutex>

using namespace std;
static int counter = 1;
static int value = 1;

void worker(int* cnt, bool verbose) {
    while (1) {
        if (verbose) {
            printf("%04d%c", *cnt, (*cnt % 16 == 0) ? '\n' : ' ');
        }
        (*cnt) += 1;
        usleep(100'000);
    }
}

static std::atomic_uint64_t sum{0};
static std::queue<int> vals;
static std::mutex m;
size_t statMaxSize = 0;

void generator() {
    /* initialize random seed: */
    srand (time(NULL));

    int i = 0;
    while (1) {
        {
            std::lock_guard<std::mutex> lock(m);
            vals.push(i);
        }
        i++;

        usleep(std::rand() % 100'000);
    }
}


void calculator() {
    while (1) {
        {
            std::lock_guard<std::mutex> lock(m);

            statMaxSize = std::max(statMaxSize, vals.size());
            while (vals.size()) {
                sum += vals.front();
                vals.pop();
            }
        }

        usleep(100'000); // 100msec
    }
}

void threadTest() {
    std::thread(worker, &counter, false).detach();
    std::thread(worker, &counter, false).detach();
    std::thread(worker, &counter, false).detach();

    std::thread(generator).detach();
    std::thread(calculator).detach();
}

int main() {
    threadTest();

    while(1) {
        // just do nothing
        sleep(1);

        cout << "max queue Size=" << statMaxSize << ", sum=" << sum << std::endl;
        statMaxSize = 0;
    }

    return 0;
}