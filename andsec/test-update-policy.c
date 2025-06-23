/*************************************************************************
> FileName: test-html-tooltip.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年06月18日 星期三 17时39分34秒
 ************************************************************************/
#include "ipc.h"


int main (void)
{
    IpcMessage msg;
    msg.type = IPC_TYPE_UPDATE_POLICY;
    msg.dataLen = 0;

    char buffer[4096] = {0};

    memcpy(buffer, &msg, sizeof(IpcMessage));

    sendDataToDaemon(buffer, sizeof (IpcMessage));

    return 0;
}

