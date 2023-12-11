/*************************************************************************
> FileName: ev-demo1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 11 Dec 2023 10:24:36 AM CST
 ************************************************************************/
#include <stdio.h>
#include <ev.h>

static ev_idle idle;

static void idle_cb(struct ev_loop* loop, ev_idle *idle, int revents) 
{
  puts("idle start.");
  ev_idle_stop(loop, idle);
}

int main(int argc, char const *argv[])
{
  struct ev_loop* loop = EV_DEFAULT;

  // 注册一个空闲事件.
  ev_idle_init(&idle, idle_cb);
  ev_idle_start(loop, &idle);

  ev_run(loop, 0);
  return 0;
}
