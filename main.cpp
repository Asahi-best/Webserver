#include <unistd.h>
#include "./server/webserver.h"

int main()
{
    /* 守护进程 后台运行 */
    //daemon(1, 0); 

    WebServer server(1314, 3, 60000, false, 4);
    server.Start();
}