/*************************************************************************
> FileName: qclient.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Fri 24 Mar 2023 10:17:24 AM CST
 ************************************************************************/
#include <QLocalSocket>

#include <unistd.h>
#include <stdlib.h>

#define DSM_FILE_CONTROL_SERVER_SOCKET_PATH "/tmp/aa.sock"

int main (int argc, char* argv[])
{
    QLocalSocket s;

    s.connectToServer(DSM_FILE_CONTROL_SERVER_SOCKET_PATH);

    srandom(1000);

    if (s.waitForConnected()) {
        while (true) {
            QTextStream ts(&s);
            ts << (QString::number(random()) + "aaaaaaaaa");
            ts.flush();
            s.waitForBytesWritten();
            sleep(1);
        }
    }

    return 0;
}
