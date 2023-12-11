/*************************************************************************
> FileName: ev-demo3.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 11 Dec 2023 11:41:01 AM CST
 ************************************************************************/
#include <ev.h>
#include <stdio.h>
#include <unistd.h>

void stdin_cb(struct ev_loop *loop, ev_io *w, int revents) {

  char buffer[4096];
  memset(buffer, 4096, 4096);
  read(w->fd, buffer, 4096);
  puts(buffer);
  printf("等待输入: \n");
}

int main (void)
{
  // 可以使用已定义的宏来获取默认的事件循环, 当然你也可以根据自己的需求创建指定的.
  struct ev_loop *loop = EV_DEFAULT;

  ev_io stdin_watcher;

  // 在启动一个I/O观察者之前, 我们需要先初始化它.
  ev_io_init (&stdin_watcher, stdin_cb, /*STDIN_FILENO*/ 0, EV_READ);
  // 启动后意味着观察者将在`stdin`变为可读后触发.
  ev_io_start (loop, &stdin_watcher);
  printf("等待输入: \n");

  // 开始运行事件循环
  ev_run (loop, 0);

  // 如果事件循环退出, 那将会执行到这里.
  return 0;
}
