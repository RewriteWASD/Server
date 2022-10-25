//
// Created by yangfu on 2022/10/25.
//

#ifndef WEBSERVER_HEAPTIMER_H
#define WEBSERVER_HEAPTIMER_H

#include <queue>
#include <unordered_map>
#include <time.h>
#include <algorithm>
#include <arpa/inet.h>
#include <functional>
#include <assert.h>
#include <chrono>
#include "../log/Log.h"

typedef std::function<void()> TimeoutCallBack;
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds MS;
typedef Clock::time_point TimeStamp;

struct TimerNode{
    int id;
    TimeStamp expires;
    TimeoutCallBack cb;
    bool operator<(const TimerNode& t){
        return expires<t.expires;
    }

};

class HeapTimer {
public:
    HeapTimer() { heap_.reserve(64); }

    ~HeapTimer() { clear(); }

    void adjust(int id, int newExpires);

    void add(int id, int timeOut, const TimeoutCallBack& cb);

    void doWork(int id);

    void clear();

    void tick();

    void pop();

    int GetNextTick();
private:
    void del_(size_t i);

    void siftup_(size_t i);

    bool siftdown_(size_t index,size_t n);

    void SwapNode_(size_t i,size_t j);

    vector<TimerNode> heap_;
    unordered_map<int,size_t> ref_;
};


#endif //WEBSERVER_HEAPTIMER_H
