/*************************************************************************
> FileName: ev-demo6.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 11 Dec 2023 01:48:49 PM CST
 ************************************************************************/
// 只需导入单个头文件
#include <ev.h>
#include <stdio.h>

// 当使用了键盘的组合键`CTRL`+`C`之后回调会被调用.
static void sig_cb (struct ev_loop *loop, ev_signal *w, int revents)
{
  puts ("Bye.");
  ev_break(loop, EVBREAK_ALL);
}


int main (void)
{
  // 可以使用已定义的宏来获取默认的事件循环, 当然你也可以根据自己的需求创建指定的.
  struct ev_loop *loop = EV_DEFAULT;

  ev_signal sigint;
  ev_signal_init(&sigint, sig_cb, SIGINT);
  ev_signal_start(loop, &sigint);

  // 开始运行事件循环
  ev_run (loop, 0);

  // 如果事件循环退出, 那将会执行到这里.
  return 0;
}
