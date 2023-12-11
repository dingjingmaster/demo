/*************************************************************************
> FileName: ev-demo7.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 11 Dec 2023 01:56:02 PM CST
 ************************************************************************/
// 只需导入单个头文件
#include <ev.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// 当使用了键盘的组合键`CTRL`+`C`之后回调会被调用.
static void child_cb (struct ev_loop *loop, ev_child *w, int revents)
{
  printf("Loop ot ProcessID: %d, Process status: %d\n", w->rpid, w->rstatus);
  ev_break(loop, EVBREAK_ALL);
}


int main (void)
{

  // 调用`fork`创建并分离父子进程
  int id = fork();
  if (id == -1) {
    puts ("Failed to create child process.");
    exit(EXIT_FAILURE);
  }

  // 如果id == 0就是子进程.
  if (!id) {
    sleep(1);
    puts("Child process exit.");
    exit(EXIT_SUCCESS);
  }

  // 可以使用已定义的宏来获取默认的事件循环, 当然你也可以根据自己的需求创建指定的.
  struct ev_loop *loop = EV_DEFAULT;

  ev_child child;
  ev_child_init(&child, child_cb, 0, 0);
  ev_child_start(loop, &child);

  // 开始运行事件循环
  ev_run (loop, 0);

  // 如果事件循环退出, 那将会执行到这里.
  return 0;
}
