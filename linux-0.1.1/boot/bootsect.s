!
! SYS_SIZE is the number of clicks (16 bytes) to be loaded.
! 0x3000 is 0x30000 bytes = 196kB, more than enough for current
! versions of linux
!
SYSSIZE = 0x3000                ! 操作系统大小
!
!    bootsect.s        (C) 1991 Linus Torvalds
!
! bootsect.s is loaded at 0x7c00 by the bios-startup routines, and moves
! iself out of the way to address 0x90000, and jumps there.
!
! It then loads 'setup' directly after itself (0x90200), and the system
! at 0x10000, using BIOS interrupts. 
!
! NOTE! currently system is at most 8*65536 bytes long. This should be no
! problem, even in the future. I want to keep it simple. This 512 kB
! kernel size should be enough, especially as this doesn't contain the
! buffer cache as in minix
!
! The loader has been made as simple as possible, and continuos
! read errors will result in a unbreakable loop. Reboot by hand. It
! loads pretty fast by getting whole sectors at a time whenever possible.

.globl begtext, begdata, begbss, endtext, enddata, endbss
.text
begtext:
.data
begdata:
.bss
begbss:
.text

SETUPLEN = 4                    ! 要加载的 setup 程序扇区数目
BOOTSEG  = 0x07c0               ! 引导扇区起始地址，引导扇区 512B
INITSEG  = 0x9000               ! 启动扇区要被移动到的位置
SETUPSEG = 0x9020               ! setup 代码起始位置
SYSSEG   = 0x1000               ! 系统起始位置 0x10000 (65536).
ENDSEG   = SYSSEG + SYSSIZE     ! 操作系统结束的位置

! ROOT_DEV:    0x000 - same type of floppy as boot.
!       0x301 - first partition on first drive etc
ROOT_DEV = 0x306                ! 根文件系统的设备号(第二个硬盘第一分区)

entry _start
_start:
    mov ax,#BOOTSEG
    mov ds,ax
    mov ax,#INITSEG
    mov es,ax
    mov cx,#256                 ! 要复制的字数(一个字为两个字节，正好512字节)
    sub si,si                   ! ds(0x7c00) + si(0) 表示源地址
    sub di,di                   ! es(0x9000) + di(0) 表示复制到的目标地址
    rep
    movw                        ! 从 _start 开始到这里，把自身从 0x7C00 复制到 0x9000 处
    jmpi go,INITSEG             ! 执行完jmpi之后，CS的值变为 0x9000，IP 的值变为从 0x9000 到 go 这一行对应的指令偏移（即：下一条要执行的代码是 go: mov ax,cs）；到新位置继续执行下一条指令
go: mov ax,cs                   ! 由于代码从 0x7C00 复制到了 0x9000，代码位置发生了变化，要执行新位置的代码就要对寄存器进行调整
    mov ds,ax
    mov es,ax
! put stack at 0x9ff00.
    mov ss,ax                   ! 栈基址寄存器ss 位置调整 0x9000
    mov sp,#0xFF00              ! 栈顶指针位于0xFF00，(0x9000 ~ 0xFF00 为栈内存，栈的增长方向是由：0xFF00 到 0x9000)

! 至此，bootsect 的第一步操作完成，这一步主要做了两件事：
!   1. 将自己从 0x7C00 复制到 0x9000 
!   2. 规划栈的位置和大小
! 接下来 bootsect 将开始执行第二步：将 setup 程序加载到内存中

! load the setup-sectors directly after the bootblock.
! Note that 'es' is already set up.

! 此时 DS: 0x9000 
!      ES: 0x9000
!
! 0x13 中断读取磁盘使用方式:
!       DL:     存储驱动器号码
!       DH:     用于存储磁头号
!       CX:     低8位存储扇区数，CL 高2为存储磁道号，CL 低6为存储读取的起始扇区号
!       ES:BX   设置要读取到的内存位置
!       AH:     0x02 表示0x13要执行读取功能
!       AL:     要读取的扇区数
!   读取完成之后检测标志寄存器的CF位来确定是否成功，如果 CF为0表示成功，否则表示读取失败。
load_setup:
    mov dx,#0x0000              ! 0驱动器, 0磁头号
    mov cx,#0x0002              ! sector 2, track 0
    mov bx,#0x0200              ! 要读取的位置：ES:BX(0x9000:0x0200) = 0x90200
    mov ax,#0x0200+SETUPLEN     ! 0x0200 + 0x0004，表示要读取4个扇区, 1个扇区 512B，4个扇区 2KB
    int 0x13                    ! 借用BIOS的中断读取磁盘信息
    jnc ok_load_setup           ! 进位标志位CF为0则执行跳转，说明读取 setup 成功
    mov dx,#0x0000
    mov ax,#0x0000              ! 读取失败则重置磁盘
    int 0x13
    j load_setup

! 至此，操作系统已经从软盘中加载了 5 个扇区的代码，bootsect 占用1个扇区 + setup 占用4个扇区


! 0x13 中断读取磁盘信息的使用方式：
!       DL:     存储驱动器号
!       AH:     0x08 表示获取磁盘信息
!   如果执行成功则CF=0，反之亦反
!   如果成功，则结果存储在 ES:BX 指向的内存地址处
! 返回参数获取软盘驱动器信息：
!   bl,[0]  -- 物理驱动器号
!   cl,[1]  -- 磁头数
!   ch,[2]  -- 磁道数
!   dh,[3]  -- 扇区数
!   al,[4]  -- 每扇区字节数
!
ok_load_setup:
! Get disk drive parameters, specifically nr of sectors/track
    mov dl,#0x00                ! 驱动器0
    mov ax,#0x0800              ! AH=8 表示获取磁盘信息
    int 0x13                    ! 获取 驱动器0 的信息，结果存在 0x9000:0x0200
    mov ch,#0x00                ! 
    seg cs                      ! NOTE://
    mov sectors,cx              ! NOTE://
    mov ax,#INITSEG             ! 0x9000
    mov es,ax                   ! 0x9000

! 设置光标位置，为屏幕上打印输出字符串做准备
    mov ah,#0x03                ! 
    xor bh,bh
    int 0x10                    ! 根据AH的值执行对应的光标操作, BH值表示要操作第几页的光标; 0x03 表示设置光标的位置

! 显示字符串
    mov cx,#24                  ! 要显示的字符的重复次数
    mov bx,#0x0007              ! 要显示的页面和颜色属性，第0页，正常显示
    mov bp,#msg1                ! 要显示的字符串(其中添加了属性，用来控制前景色和背景色)
    mov ax,#0x1301              ! 显示字符串的功能号和显示方式：AH:0x13表示字符串写到屏幕, 0x01表示文本模式显示
    int 0x10                    ! 显示字符串

! ok, we've written the message, now
! we want to load the system (at 0x10000)
! 至此完成字符串显示
! 开始加载系统
    mov ax,#SYSSEG              ! 0x1000 -> ax
    mov es,ax                   ! 0x1000 -> es, 使用时候会左移4位，变成：0x10000
    call read_it
    call kill_motor

! After that we check which root-device to use. If the device is
! defined (!= 0), nothing is done and the given device is used.
! Otherwise, either /dev/PS0 (2,28) or /dev/at0 (2,8), depending
! on the number of sectors that the BIOS reports currently.

    seg cs
    mov ax,root_dev
    cmp ax,#0
    jne root_defined
    seg cs
    mov bx,sectors
    mov ax,#0x0208              ! /dev/ps0 - 1.2Mb
    cmp bx,#15
    je  root_defined
    mov ax,#0x021c              ! /dev/PS0 - 1.44Mb
    cmp bx,#18
    je root_defined
undef_root:
    jmp undef_root
root_defined:
    seg cs
    mov root_dev,ax

! after that (everyting loaded), we jump to
! the setup-routine loaded directly after
! the bootblock:

    jmpi 0,SETUPSEG             ! 跳转到 setup 开始执行

! This routine loads the system at address 0x10000, making sure
! no 64kB boundaries are crossed. We try to load it as fast as
! possible, loading whole tracks whenever we can.
!
! in: es - starting address segment (normally 0x1000)
!
sread: .word 1+SETUPLEN         ! sectors read of current track
head: .word 0                   ! current head
track: .word 0                  ! current track

read_it:
    mov ax,es                   ! EX: 0x1000
    test ax,#0x0fff             ! 按位测试 AX 与 #0x0FFF，最终结果写入零标志位；运算结果为0，则ZF被设置为1，否则ZF被清零。
die: jne die                    ! FIXME:// 此处没理解。上条指令后 ZF 应该被设置为 0， must be at 64kB boundary
    xor bx,bx                   ! bx is starting address within segment
rp_read:
    mov ax,es
    cmp ax,#ENDSEG              ! have we loaded all yet? #ENDSEG - AX, 如果两个数相等，则：ZF=1，CF=0。OF(溢出位) SF(符号位)
    jb ok1_read                 ! 无符号数比较的跳转指令。如果 CF 为1，则跳转到指定位置
    ret
ok1_read:
    seg cs                      ! FIXME:// ？？？
    mov ax,sectors              ! FIXME:// ???
    sub ax,sread                ! sread = 1+0x4
    mov cx,ax
    shl cx,#9
    add cx,bx
    jnc ok2_read
    je ok2_read
    xor ax,ax
    sub ax,bx
    shr ax,#9
ok2_read:
    call read_track
    mov cx,ax
    add ax,sread
    seg cs
    cmp ax,sectors
    jne ok3_read
    mov ax,#1
    sub ax,head
    jne ok4_read
    inc track
ok4_read:
    mov head,ax
    xor ax,ax
ok3_read:
    mov sread,ax
    shl cx,#9
    add bx,cx
    jnc rp_read
    mov ax,es
    add ax,#0x1000
    mov es,ax
    xor bx,bx
    jmp rp_read

read_track:
    push ax
    push bx
    push cx
    push dx
    mov dx,track
    mov cx,sread
    inc cx
    mov ch,dl
    mov dx,head
    mov dh,dl
    mov dl,#0
    and dx,#0x0100
    mov ah,#2
    int 0x13
    jc bad_rt
    pop dx
    pop cx
    pop bx
    pop ax
    ret
bad_rt:    mov ax,#0
    mov dx,#0
    int 0x13
    pop dx
    pop cx
    pop bx
    pop ax
    jmp read_track

!/*
! * This procedure turns off the floppy drive motor, so
! * that we enter the kernel in a known state, and
! * don't have to worry about it later.
! */
kill_motor:
    push dx
    mov dx,#0x3f2
    mov al,#0
    outb
    pop dx
    ret

sectors:
    .word 0

msg1:
    .byte 13,10
    .ascii "Loading system ..."
    .byte 13,10,13,10

.org 508
root_dev:
    .word ROOT_DEV
boot_flag:
    .word 0xAA55

.text
endtext:
.data
enddata:
.bss
endbss:
