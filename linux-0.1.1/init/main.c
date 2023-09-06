/*
 *  linux/init/main.c
 *
 *  (C) 1991  Linus Torvalds
 */

#define __LIBRARY__
#include <unistd.h>
#include <time.h>

/*
 * we need this inline - forking from kernel space will result
 * in NO COPY ON WRITE (!!!), until an execve is executed. This
 * is no problem, but for the stack. This is handled by not letting
 * main() use the stack at all after fork(). Thus, no function
 * calls - which means inline code for fork too, as otherwise we
 * would use the stack upon exit from 'fork()'.
 *
 * Actually only pause and fork are needed inline, so that there
 * won't be any messing with the stack from main(), but we define
 * some others too.
 */
static inline _syscall0(int,fork)
static inline _syscall0(int,pause)
static inline _syscall1(int,setup,void *,BIOS)
static inline _syscall0(int,sync)

#include <linux/tty.h>
#include <linux/sched.h>
#include <linux/head.h>
#include <asm/system.h>
#include <asm/io.h>

#include <stddef.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#include <linux/fs.h>

static char printbuf[1024];

extern int vsprintf();
extern void init(void);
extern void blk_dev_init(void);
extern void chr_dev_init(void);
extern void hd_init(void);
extern void floppy_init(void);
extern void mem_init(long start, long end);
extern long rd_init(long mem_start, int length);
extern long kernel_mktime(struct tm * tm);
extern long startup_time;

/*
 * This is set up by the setup-routine at boot-time
 */
#define EXT_MEM_K (*(unsigned short *)0x90002)
#define DRIVE_INFO (*(struct drive_info *)0x90080)
#define ORIG_ROOT_DEV (*(unsigned short *)0x901FC)

/*
 * Yeah, yeah, it's ugly, but I cannot find how to do this correctly
 * and this seems to work. I anybody has more info on the real-time
 * clock I'd be interested. Most of this was trial and error, and some
 * bios-listing reading. Urghh.
 */

#define CMOS_READ(addr) ({ \
outb_p(0x80|addr,0x70); \
inb_p(0x71); \
})

#define BCD_TO_BIN(val) ((val)=((val)&15) + ((val)>>4)*10)

static void time_init(void)
{
    struct tm time;

    do {
        time.tm_sec = CMOS_READ(0);
        time.tm_min = CMOS_READ(2);
        time.tm_hour = CMOS_READ(4);
        time.tm_mday = CMOS_READ(7);
        time.tm_mon = CMOS_READ(8);
        time.tm_year = CMOS_READ(9);
    } while (time.tm_sec != CMOS_READ(0));
    BCD_TO_BIN(time.tm_sec);
    BCD_TO_BIN(time.tm_min);
    BCD_TO_BIN(time.tm_hour);
    BCD_TO_BIN(time.tm_mday);
    BCD_TO_BIN(time.tm_mon);
    BCD_TO_BIN(time.tm_year);
    time.tm_mon--;
    startup_time = kernel_mktime(&time);
}

static long memory_end = 0;
static long buffer_memory_end = 0;
static long main_memory_start = 0;

struct drive_info { char dummy[32]; } drive_info;

void main(void)                                 // This really IS void, no error here.
{                                               // The startup routine assumes (well, ...) this
/*
 * Interrupts are still disabled. Do necessary setups, then
 * enable them
 */
    ROOT_DEV = ORIG_ROOT_DEV;                   // FIXME:// 备份 ROOT_DEV，这个变量值保存在哪？
    drive_info = DRIVE_INFO;                    // 备份硬盘参数表, char[32]，每个硬盘参数表为 16 字节
    /**
     * @brief
     *  进程 0 在主机中的运算主要是通过CPU和内存相互配合工作而得以实现的，这里系统将对主机中物理内存的使用及管理进行规划，为进程0具备运算能力打下基础。
     *  具体操作是这样的：
     *      除了1MB以内的内核区之外，其余物理内存要完成的工作是不同的，“主内存区”主要用来承载进程的相关信息，包括进程管理结构、进程对应的程序等
     *      “缓存区”主要作为主机与外设进行数据交互的中转站：“虚拟盘区”是一个可选的区域，如果选择使用虚拟盘，就可以将外设上的数据先复制到虚拟盘区，然后再使用。提高了系统执行效率。
     *  针对内存条大小进行规划(其中 memory_end 表示“主内存区的末端位置”，“main_memory_start”为主内存区的起始位置；“buffer_memory_start” 与 “buffer_memory_end” 分别表示缓存区的起始位置和缓存区的末端位置)
     *            ? <=  6MB:
     *      6MB < ? <= 12MB:
     *     12MB < ? <= 16MB:
     *     16MB < ?        :
     *  +--------+--------------------------------------+
     *  | 缓冲区 |      物理内存                        |
     *  +--------+--------------------------------------+
     */
    memory_end = (1<<20) + (EXT_MEM_K<<10);
    memory_end &= 0xfffff000;
    if (memory_end > 16*1024*1024) {
        memory_end = 16*1024*1024;
    }

    if (memory_end > 12*1024*1024) {
        buffer_memory_end = 4*1024*1024;
    }
    else if (memory_end > 6*1024*1024) {
        buffer_memory_end = 2*1024*1024;
    }
    else {
        buffer_memory_end = 1*1024*1024;
    }

    main_memory_start = buffer_memory_end;                          // 缓存区的末端位置就是主内存区的末端位置
#ifdef RAMDISK
    main_memory_start += rd_init(main_memory_start, RAMDISK*1024);  // rd_init 表示对虚拟盘进行初始化，将 main_memory_start 开始，RAMDISK*1024 内存初始化为'0'，然后返回长度
#endif
    // 至此，确定了 主内存区 的大小和 缓存区 大小确定。
    // 开始对主内存的管理结构进行设置
    mem_init(main_memory_start, memory_end);                        // mem/memory.c，根据内存开始位置+结束位置 对内存分页，可用页的状态置为0（空闲），其它为100（非空闲）
    trap_init();                                                    // kernel/traps.c，将异常处理的中断服务程序与中断描述符表进行挂接，开始逐步重建中断服务体系，一次支撑进程0在主机中的运算
                                                                    // 设置 IDT
    blk_dev_init();                                                 // kernel/blk_dev/ll_rw_block.c 中
                                                                    // 内核要想与块设备进行沟通，就要依托系统为此构建的工作环境。主机内存的缓存区与外设中的块设备（如：软盘和硬盘）通过 `struct request` 建立关系的
                                                                    // 请求项管理是块设备与缓存区打交道的唯一中转站，系统会对缓存块操作与否进行权衡，并把需要的缓存块登记在这个请求的账本上。块设备得到操作指令后，值根据请求项中的记录来决定当前要处理哪个设备的哪个逻辑块。
                                                                    // 另外，用户对读的请求更加关注，因此读占据更多的空间，request[32]中前2/3主要用于读写共享，后1/3主要用于读操作
    chr_dev_init();                                                 // kernel/chr_dev/tty_io.c 空实现
    tty_init();                                                     // kernel/chr_dev/tty_io.c 串口通讯使用，外设串口（键鼠）、显示器
    time_init();                                                    // 读取 CMOS 时间
    sched_init();                                                   //
    buffer_init(buffer_memory_end);                                 // 缓存区初始化
    hd_init();                                                      // 硬盘中断
    floppy_init();                                                  // 软盘中断
    sti();                                                          // 打开中断
    move_to_user_mode();
    if (!fork()) {                                                  // we count on this going ok
        init();
    }
/*
 *   NOTE!!   For any other task 'pause()' would mean we have to get a
 * signal to awaken, but task0 is the sole exception (see 'schedule()')
 * as task 0 gets activated at every idle moment (when no other tasks
 * can run). For task0 'pause()' just means we go check if some other
 * task can run, and if not we return here.
 */
    for(;;) pause();
}

static int printf(const char *fmt, ...)
{
    va_list args;
    int i;

    va_start(args, fmt);
    write(1,printbuf,i=vsprintf(printbuf, fmt, args));
    va_end(args);
    return i;
}

static char * argv_rc[] = { "/bin/sh", NULL };
static char * envp_rc[] = { "HOME=/", NULL };

static char * argv[] = { "-/bin/sh",NULL };
static char * envp[] = { "HOME=/usr/root", NULL };

void init(void)
{
    int pid,i;

    setup((void *) &drive_info);
    (void) open("/dev/tty0",O_RDWR,0);
    (void) dup(0);
    (void) dup(0);
    printf("%d buffers = %d bytes buffer space\n\r",NR_BUFFERS,
        NR_BUFFERS*BLOCK_SIZE);
    printf("Free mem: %d bytes\n\r",memory_end-main_memory_start);
    if (!(pid=fork())) {
        close(0);
        if (open("/etc/rc",O_RDONLY,0))
            _exit(1);
        execve("/bin/sh",argv_rc,envp_rc);
        _exit(2);
    }
    if (pid>0)
        while (pid != wait(&i))
            /* nothing */;
    while (1) {
        if ((pid=fork())<0) {
            printf("Fork failed in init\r\n");
            continue;
        }
        if (!pid) {
            close(0);close(1);close(2);
            setsid();
            (void) open("/dev/tty0",O_RDWR,0);
            (void) dup(0);
            (void) dup(0);
            _exit(execve("/bin/sh",argv,envp));
        }
        while (1)
            if (pid == wait(&i))
                break;
        printf("\n\rchild %d died with code %04x\n\r",pid,i);
        sync();
    }
    _exit(0);   /* NOTE! _exit, not exit() */
}
