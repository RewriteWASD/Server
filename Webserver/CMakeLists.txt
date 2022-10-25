cmake_minimum_required(VERSION 3.21)
project(Webserver)

set(CMAKE_CXX_STANDARD 14)

link_libraries(-lpthread -lmysqlclient)

add_executable(Webserver code/buffer/Buffer.cpp code/log/BlockDeque.h code/log/Log.cpp code/buffer/Buffer.h code/log/Log.h code/pool/ThreadPool.h code/http/HttpConn.cpp code/http/HttpConn.h code/pool/SqlConnRAII.h code/pool/SqlConnPool.cpp code/pool/SqlConnPool.h code/http/HttpRequest.cpp code/http/HttpRequest.h code/http/HttpResponse.cpp code/http/HttpResponse.h code/server/Epoller.cpp code/server/Epoller.h code/server/WebServer.cpp code/server/WebServer.h code/timer/HeapTimer.cpp code/timer/HeapTimer.h code/main.cpp)
