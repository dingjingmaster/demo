#include <stdio.h>
#include <stdlib.h>


int main() 
{
    // 创建一个限制特定 IP 的 iptables 规则示例
    system("iptables -A OUTPUT -m owner --uid-owner 1000 -j REJECT");
    // system("iptables -A OUTPUT -p tcp --dport 80 -m owner --uid-owner 1000 -j DROP");

    // 执行子进程
    pid_t pid = fork();
    if (pid == 0) {
        // 子进程执行联网任务
        execlp("curl", "curl", "http://example.com", NULL);
    } else {
        wait(NULL); // 等待子进程完成
        // 清除防火墙规则
        system("iptables -D OUTPUT -m owner --uid-owner 1000 -j REJECT");
    }
    return 0;
}

