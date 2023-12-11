/*************************************************************************
> FileName: ev-demo5.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 11 Dec 2023 01:38:47 PM CST
 ************************************************************************/
// 只需导入单个头文件
#include <ev.h>
#include <stdio.h>
#include <unistd.h>

// 当超时时间到达, 这个回调将会被触发.
static void timeout_cb (struct ev_loop *loop, ev_timer *w, int revents)
{
  puts ("timeout");
}


int main (void)
{
  // 可以使用已定义的宏来获取默认的事件循环, 当然你也可以根据自己的需求创建指定的.
  struct ev_loop *loop = EV_DEFAULT;

  ev_timer timeout_watcher;
  ev_timer_init (&timeout_watcher, timeout_cb, 3., 1.);
  ev_timer_start (loop, &timeout_watcher);

  // 开始运行事件循环
  ev_run (loop, 0);

  // 如果事件循环退出, 那将会执行到这里.
  return 0;
}
