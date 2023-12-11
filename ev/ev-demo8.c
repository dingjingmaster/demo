/*************************************************************************
> FileName: ev-demo8.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 11 Dec 2023 01:58:58 PM CST
 ************************************************************************/

// 只需导入单个头文件
#include <ev.h>
#include <stdio.h>

// 输出后退出.
static void idle_cb (struct ev_loop *loop, ev_idle *w, int revents)
{
  puts ("Hello world.");
  ev_break(loop, EVBREAK_ALL);
}

int main (void)
{
  // 可以使用已定义的宏来获取默认的事件循环, 当然你也可以根据自己的需求创建指定的.
  struct ev_loop *loop = EV_DEFAULT;

  ev_idle idle;
  ev_idle_init(&idle, idle_cb);
  ev_idle_start(loop, &idle);

  // 开始运行事件循环
  ev_run (loop, 0);

  // 如果事件循环退出, 那将会执行到这里.
  return 0;
}
