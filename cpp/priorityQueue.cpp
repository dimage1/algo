#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>


// Meeting rooms
// how many rooms would be required to conduct a certain number of meetings (a minimum meeting rooms problem)
int minMeetingRooms(std::vector<std::vector<int>> &meetings) {
    if (meetings.empty()) {
        return 0;
    }

    // sort by start time
    std::sort(meetings.begin(), meetings.end(), [](const auto &x, const auto &y)
        {
            return x[0] < y[0]; 
        });

    // use priority queue
    std::priority_queue<int,  std::vector<int>, std::greater<int>> rooms;
    rooms.push(meetings[0][1]);

    for (size_t i = 1 ; i < meetings.size(); i++) {
        const int start = meetings[i][0];
        const int end = meetings[i][1];

        std::cout << "start=" << start << " top=" << rooms.top() << std::endl;
        if (start >= rooms.top()) {
            rooms.pop();
        }
        rooms.push(end);
    }

    return rooms.size();
}

void priorityQueueTest() {
    
    const auto data = {1, 8, 5, 6, 3, 4, 0, 9, 7, 2};
 
    std::priority_queue<int> q1; // Max priority queue
    for (int n : data)
        q1.push(n);
    std::cout << "q1 " << q1.top() << std::endl;
 
    // Min priority queue
    // std::greater<int> makes the max priority queue act as a min priority queue
    std::priority_queue<int, std::vector<int>, std::greater<int>>
        minq1(data.begin(), data.end());
    std::cout << "miq1 " << minq1.top() << std::endl;
 
    // Second way to define a min priority queue
    std::priority_queue minq2(data.begin(), data.end(), std::greater<int>());
    std::cout << "minq2 " << minq2.top() << std::endl;
 
    // Using lambda to compare elements.
    auto cmp = [](int left, int right) { return (left ^ 1) < (right ^ 1); };
    std::priority_queue<int, std::vector<int>, decltype(cmp)> minq5(cmp);
     for (int n : data)
        minq5.push(n);
    std::cout << "minq5 " << minq5.top() << std::endl;
}

int main() {
    priorityQueueTest();

    std::vector<std::vector<int>> test1 = {{6,8}, {4,6}, {3,5}, {8,9}};
    std::vector<std::vector<int>> test2 = {{1,2}, {1,2}, {1,2}, {1,2}, {1,2}};
    std::vector<std::vector<int>> test3 = {{1,2}, {2,3}, {3,4}, {4,5}};
    std::vector<std::vector<int>> test4 = {};

    std::cout << "---test1: " << minMeetingRooms(test1) << std::endl;
    std::cout << "---test2: " << minMeetingRooms(test2) << std::endl;
    std::cout << "---test3: " << minMeetingRooms(test3) << std::endl;
    std::cout << "---test4: " << minMeetingRooms(test4) << std::endl;


    return 0;
}