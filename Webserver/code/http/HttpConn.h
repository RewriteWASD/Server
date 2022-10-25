//
// Created by yangfu on 2022/10/11.
//

#ifndef WEBSERVER_HTTPCONN_H
#define WEBSERVER_HTTPCONN_H

#include <sys/types.h>
#include <sys/uio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <atomic>

#include "../log/Log.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "../pool/SqlConnRAII.h"
#include "../buffer/Buffer.h"

class HttpConn {
public:
    HttpConn();
    ~HttpConn();

    void init(int sockFd,const sockaddr_in& addr);
    ssize_t read(int* saveErrno);
    ssize_t write(int* saveErrno);
    void Close();
    int GetFd() const;
    int GetPort() const;
    const char* GetIP() const;
    sockaddr_in GetAddr() const;

    bool process();
    int ToWriteBytes(){
        return iov_[0].iov_len+iov_[1].iov_len;
    }
    bool IsKeepAlive() const{
        return request_.IsKeepAlive();
    }

    static bool isET;
    static const char* srcDir;
    static atomic<int> userCount;


private:
    int fd_;
    struct sockaddr_in addr_;

    bool isClose_;

    int iovCnt_;
    struct iovec iov_[2];

    Buffer readBuff_;
    Buffer writeBuff_;

    HttpRequest request_;
    HttpResponse response_;
};


#endif //WEBSERVER_HTTPCONN_H
