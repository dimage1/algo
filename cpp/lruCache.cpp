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
#include <list>
#include <deque>
#include <unordered_map>
#include <optional>

using namespace std;

template<typename KEY, typename VAL>
class LRUCache {
    using lruList = list<pair<KEY, VAL>>;
    using lruIter = typename list<pair<KEY, VAL>>::iterator;
    using lruMap = unordered_map<KEY, lruIter>;

public:
    LRUCache(size_t size) : m_MaxSize(size){};

    void put(const KEY &key, const VAL &val) {
        auto iter = m_Map.find(key);
        if (iter != m_Map.end()) {
            // update value
            *(iter->second) = make_pair(key, val);

            // refresh usage
            get(key);
            return;
        }

        if (m_List.size() == m_MaxSize) {
            // remove the last used elem
            KEY k = m_List.back().first;
            m_List.remove(m_List.back());
            m_Map.erase(k);
        }
        m_List.push_front(make_pair(key, val));
        m_Map[key] = m_List.begin();
    }

    optional<VAL> get(const KEY &key) {
        const auto iter = m_Map.find(key);
        if (iter == m_Map.end()) {
            return {};
        }

        // move to front (refresh usage)
        m_List.splice(m_List.begin(), m_List, iter->second);
        return iter->second->second;
    }

private:
    const size_t m_MaxSize;

    lruList m_List;
    lruMap m_Map;
};

int main() {

    LRUCache<int, int> cache(8);
    for (int i = -2; i < 7; i++) {
        cache.put(i, i);
    }
    cache.get(-1);
    cache.get(1);
    for (int i = 7; i < 10; i++) {
        cache.put(i, i);
    }

    for (int i = -4; i < 14; i++) {
        auto val = cache.get(i);
        cout << "key=" << i << ", val=" << (val ? *val : -111) << endl;
    }

    return 0;
}