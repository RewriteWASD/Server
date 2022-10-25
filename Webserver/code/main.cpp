//
// Created by yangfu on 2022/10/25.
//

#include <unistd.h>
#include "server/WebServer.h"

int main() {
    /* 守护进程 后台运行 */
    //daemon(1, 0);

    WebServer server(
            8088, 3, 60000, false,             /* 端口 ET模式 timeoutMs 优雅退出  */
            3306, "root", "587051.", "yourdb", /* Mysql配置 */
            12, 6, true, 1, 1024);             /* 连接池数量 线程池数量 日志开关 日志等级 日志异步队列容量 */
    server.Start();
}
