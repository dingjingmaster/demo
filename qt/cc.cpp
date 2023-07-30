/*************************************************************************
> FileName: cc.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 24 Mar 2023 10:55:34 AM CST
 ************************************************************************/
#include <QLocalSocket>
#include <stdlib.h>
#include <unistd.h>

#define DSM_FILE_CONTROL_SERVER_SOCKET_PATH "/tmp/aa.sock"
 int main(int argc, char **argv)
{
    QLocalSocket ls;
    ls.connectToServer(DSM_FILE_CONTROL_SERVER_SOCKET_PATH);
    srandom(1000);
     if (ls.waitForConnected()){
        while (1){
            QTextStream ts(&ls);
            ts << QString::number(random()) + "\nTTTTTTTTTT" + "\nXXXXXXXX";
            ts.flush();
            ls.waitForBytesWritten();
            sleep(1);
        }
    }
    return 0;
}
