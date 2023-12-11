/*************************************************************************
> FileName: ev-demo2.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 11 Dec 2023 10:38:37 AM CST
 ************************************************************************/
// 只需导入单个头文件
#include <ev.h>
#include <stdio.h>

// I/O 观察者
ev_io stdin_watcher;

// Timer 观察者
ev_timer timeout_watcher;

// 当stdin可读的时候, 这个回调将会被触发.
static void stdin_cb (EV_P_ ev_io *w, int revents)
{
  puts ("stdin ready");

  // 如果你只需要获得一次事件, 那么需要主动停止事件观察者.
  ev_io_stop (EV_A_ w);

  // 调用此方法后, 会让ev_run停止所有事件.
  ev_break (EV_A_ EVBREAK_ALL);
}

// 当超时时间到达, 这个回调将会被触发.
static void timeout_cb (EV_P_ ev_timer *w, int revents)
{
  puts ("timeout");
  
  // 调用此方法后, 会让ev_run停止所有事件.
  ev_break (EV_A_ EVBREAK_ONE);
}

int main (void)
{
  // 可以使用已定义的宏来获取默认的事件循环, 当然你也可以根据自己的需求创建指定的.
  struct ev_loop *loop = EV_DEFAULT;

  // 在启动一个I/O观察者之前, 我们需要先初始化它.
  ev_io_init (&stdin_watcher, stdin_cb, /*STDIN_FILENO*/ 0, EV_READ);
  // 启动后意味着观察者将在`stdin`变为可读后触发.
  ev_io_start (loop, &stdin_watcher);

  // 在启动一个Timer观察者之前, 我们需要先初始化它.
  ev_timer_init (&timeout_watcher, timeout_cb, 5.5, 0.);
  // 这个一次性定时器将会从启动开始后的`5.5`秒后超时触发.
  ev_timer_start (loop, &timeout_watcher);

  // 开始运行事件循环
  ev_run (loop, 0);

  // 如果事件循环退出, 那将会执行到这里.
  return 0;
}
