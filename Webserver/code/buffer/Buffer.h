//
// Created by yangfu on 2022/8/24.
//

#ifndef WEBSERVER_BUFFER_H
#define WEBSERVER_BUFFER_H
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/uio.h>
#include <vector>
#include <atomic>
#include <assert.h>
using namespace std;

class Buffer {
public:
    Buffer(int initBufferSize=1024);
    ~Buffer()=default;
    size_t WritableBytes() const;
    size_t ReadableBytes() const;
    size_t PrependableBytes() const;

    const char* Peek() const;
    void EnsureWriteable(size_t len);
    void HasWritten(size_t len);

    void Retrieve(size_t len);
    void RetrieveUntil(const char* end);
    void RetrieveAll();

    string RetrieveAllToStr();
    const char* BeginWriteConst() const;
    char* BeginWrite();

    void Append(const string& str);
    void Append(const char* str,size_t len);
    void Append(const void* date,size_t len);
    void Append(const Buffer& buffer);

    ssize_t ReadFd(int fd,int* Errno);
    ssize_t WriteFd(int fd,int* Errno);

private:
    char* BeginPtr_();
    const char* BeginPtr_() const;
    void MakeSpace_(size_t len);

    vector<char> buffer_;
    atomic<size_t> readPos_;
    atomic<size_t>  writePos_;
};


#endif //WEBSERVER_BUFFER_H
