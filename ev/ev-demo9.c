/*************************************************************************
> FileName: ev-demo9.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 11 Dec 2023 02:06:33 PM CST
 ************************************************************************/
// 只需导入单个头文件
#include <ev.h>
#include <stdio.h>

// 当发现`v.log`文件被创建之后退出.
static void stat_cb (struct ev_loop *loop, ev_stat *w, int revents)
{
  puts ("Bye.");
  ev_break(loop, EVBREAK_ALL);
}


int main (void)
{
  // 可以使用已定义的宏来获取默认的事件循环, 当然你也可以根据自己的需求创建指定的.
  struct ev_loop *loop = EV_DEFAULT;

  ev_stat estat;
  ev_stat_init(&estat, stat_cb, "v.log", 0.);
  ev_stat_start(loop, &estat);

  // 开始运行事件循环
  ev_run (loop, 0);

  // 如果事件循环退出, 那将会执行到这里.
  return 0;
}
