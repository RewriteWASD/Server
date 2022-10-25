//
// Created by yangfu on 2022/8/30.
//

#ifndef WEBSERVER_BLOCKDEQUE_H
#define WEBSERVER_BLOCKDEQUE_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include <sys/time.h>
#include <assert.h>
using namespace std;
template<class T>
class BlockDeque{
public:
    explicit BlockDeque(size_t MaxCapacity=1000);

    ~BlockDeque();

    void clear();

    bool empty();

    bool full();

    void CLose();

    size_t size();

    size_t capacity();

    T front();

    T back();

    void push_back(const T&item);

    void push_front(const T &item);

    bool pop(T &item);

    bool pop(T &item,int timeout);

    void flush();
private:
    deque<T> deq_;
    size_t capacity_;
    mutex mtx_;
    bool isCLose_;
    condition_variable condConsumer_;
    condition_variable condProducer_;
};

template<class T>
BlockDeque<T>::BlockDeque(size_t MaxCapacity) :capacity_(MaxCapacity){
    assert(MaxCapacity>0);
    isCLose_=false;
}

template<class T>
void BlockDeque<T>::CLose() {
    {
        lock_guard<mutex> locker(mtx_);
        deq_.clear();
        isCLose_=true;
    }
    condConsumer_.notify_all();
    condProducer_.notify_all();
}

template<class T>
BlockDeque<T>::~BlockDeque<T>() {
    CLose();
}

template<class T>
void BlockDeque<T>::flush() {
    condConsumer_.notify_one();
}

template<class T>
T BlockDeque<T>::front() {
    lock_guard<mutex> locker(mtx_);
    return deq_.front();
}

template<class T>
void BlockDeque<T>::clear() {
    lock_guard<mutex> locker(mtx_);
    deq_.clear();
}

template<class T>
T BlockDeque<T>::back() {
    lock_guard<mutex> locker(mtx_);
    return deq_.back();
}

template<class T>
size_t BlockDeque<T>::size() {
    lock_guard<mutex> locker(mtx_);
    return capacity_;
}

template<class T>
void BlockDeque<T>::push_back(const T &item) {
    unique_lock<mutex> locker(mtx_);
    while(deq_.size()>=capacity_){
        condProducer_.wait(locker);
    }
    deq_.emplace_back(item);
    condConsumer_.notify_one();
}

template<class T>
void BlockDeque<T>::push_front(const T &item) {
    unique_lock<mutex> locker(mtx_);
    while(deq_.size()>=capacity_){
        condProducer_.wait(locker);
    }
    deq_.template emplace_front(item);
    condConsumer_.notify_one();
}

template<class T>
bool BlockDeque<T>::pop(T &item) {
    unique_lock<mutex> locker(mtx_);
    while(deq_.empty()){
        condConsumer_.wait(locker);
        if(isCLose_){
            return false;
        }
    }
    item=deq_.front();
    deq_.pop_front();
    condProducer_.notify_one();
    return true;
}

template <class T>
bool BlockDeque<T>::pop(T &item, int timeout) {
    unique_lock<mutex> locker(mtx_);
    while(deq_.empty()){
        if(condConsumer_.wait_for(locker,chrono::seconds(timeout))== cv_status::timeout){
            return false;
        }
    }
    item=deq_.front();
    deq_.pop_front();
    condProducer_.notify_one();
    return true;
}

template<typename T>
bool BlockDeque<T>::empty() {
    lock_guard<mutex> locker(mtx_);
    return deq_.empty();
}

template <typename T>
bool BlockDeque<T>::full() {
    lock_guard<mutex> locker(mtx_);
    return deq_.size()>=capacity_;
}

template <typename T>
size_t BlockDeque<T>::capacity() {
    lock_guard<mutex> locker(mtx_);
    return capacity_;
}
#endif //WEBSERVER_BLOCKDEQUE_H
