//
// Created by yangfu on 2022/10/19.
//

#ifndef WEBSERVER_HTTPRESPONSE_H
#define WEBSERVER_HTTPRESPONSE_H

#include <unordered_map>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "../buffer/Buffer.h"
#include "../log/Log.h"

class HttpResponse {
public:
    HttpResponse();
    ~HttpResponse();
    void Init(const string& srcDir,string& path,bool isKeepAlive=false,int code=-1);
    void MakeResponse(Buffer& buff);
    void UnmapFile();
    char* File();
    size_t FileLen() const;
    void ErrorContent(Buffer& buff,string message);
    int Code() const {return code_;}

private:
    void AddStateLine_(Buffer &buff);
    void AddHeader_(Buffer &buff);
    void AddContent_(Buffer &buff);

    void ErrorHtml_();
    string GetFileType_();

    int code_;
    bool isKeepAlive_;

    string path_;
    string srcDir_;

    char* mmFile_;
    struct stat mmFileStat_;

    static const unordered_map<string,string> SUFFIX_TYPE;
    static const unordered_map<int,string> CODE_STATUS;
    static const unordered_map<int,string> CODE_PATH;

};


#endif //WEBSERVER_HTTPRESPONSE_H
