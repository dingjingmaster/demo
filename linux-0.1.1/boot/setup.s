!
!   setup.s     (C) 1991 Linus Torvalds
!
! setup.s is responsible for getting the system data from the BIOS,
! and putting them into the appropriate places in system memory.
! both setup.s and system has been loaded by the bootblock.
!
! This code asks the bios for memory/disk/other parameters, and
! puts them in a "safe" place: 0x90000-0x901FF, ie where the
! boot-block used to be. It is then up to the protected mode
! system to read them from there before the area is overwritten
! for buffer-blocks.
!

! NOTE! These had better be the same as in bootsect.s!

INITSEG  = 0x9000   ! we move boot here - out of the way
SYSSEG   = 0x1000   ! system loaded at 0x10000 (65536).
SETUPSEG = 0x9020   ! this is the current segment

.globl begtext, begdata, begbss, endtext, enddata, endbss
.text
begtext:
.data
begdata:
.bss
begbss:
.text

entry start
start:

! ok, the read went well so we get current cursor position and save it for
! posterity.
! 1. 利用BIOS提供的中断服务程序从设备上提取内核运行所需的机器系统数据，其中包括光标位置和显示页面等数据，并分别从中断向量 0x41 和 0x46 向量值所指的内存地址处获取硬盘参数1和银盘参数2，并放在 0x9000:0x0080 和 0x9000:0x0090 处
! setup 从 BIOS 中提取的机器数据将覆盖 bootsect 程序所在的部分区域。
! setup 占用空间为 0x90000 ~ 0x901FD，共 510 个字节，原来的 bootsect 只有 2 个字节未被覆盖。

    mov ax,#INITSEG             ! this is done in bootsect already, but...
    mov ds,ax
    mov ah,#0x03                ! read cursor pos
    xor bh,bh
    int 0x10                    ! save it in known place, con_init fetches
    mov [0],dx                  ! it from 0x90000.
! Get memory size (extended mem, kB)

    mov ah,#0x88
    int 0x15
    mov [2],ax

! Get video-card data:

    mov ah,#0x0f
    int 0x10
    mov [4],bx                  ! bh = display page
    mov [6],ax                  ! al = video mode, ah = window width

! check for EGA/VGA and some config parameters

    mov ah,#0x12
    mov bl,#0x10
    int 0x10
    mov [8],ax
    mov [10],bx
    mov [12],cx

! Get hd0 data

    mov ax,#0x0000
    mov ds,ax
    lds si,[4*0x41]
    mov ax,#INITSEG
    mov es,ax
    mov di,#0x0080
    mov cx,#0x10
    rep
    movsb

! Get hd1 data

    mov ax,#0x0000              !
    mov ds,ax
    lds si,[4*0x46]
    mov ax,#INITSEG
    mov es,ax
    mov di,#0x0090
    mov cx,#0x10
    rep
    movsb

! Check that there IS a hd1 :-)

    mov ax,#0x01500
    mov dl,#0x81
    int 0x13
    jc  no_disk1
    cmp ah,#3
    je  is_disk1
no_disk1:
    mov ax,#INITSEG
    mov es,ax
    mov di,#0x0090
    mov cx,#0x10
    mov ax,#0x00
    rep
    stosb
is_disk1:

! now we want to move to protected mode ...

    cli                         ! 关中断 no interrupts allowed !

! first we move the system to it's rightful place
! 将 system 移动到内存地址起始位置 0x00000（原来位于 0x10000 位置）
! 原来在 0x00000 位置保存的是 BIOS 建立的中断向量表以及 BIOS 数据区，此操作将 BIOS 中断向量表和BIOS数据区完全覆盖。
    mov ax,#0x0000
    cld                         ! 清除方向标志位DF的值，清除后MOVSB将递增内存指针。 'direction'=0, movs moves forward
do_move:
    mov es,ax                   ! destination segment
    add ax,#0x1000
    cmp ax,#0x9000
    jz  end_move
    mov ds,ax                   ! source segment
    sub di,di
    sub si,si
    mov cx,#0x8000
    rep
    movsw
    jmp do_move

! then we load the segment descriptors
! GDT、GDTR
! GDT 是系统中唯一存放段寄存器内容(段描述符)的数组，配合程序进行保护模式下的寻址。它可以理解为是所有进程的总目录表，其中存放者每一个任务局部描述符表(LDT)地址 和 任务状态段(TSS)地址，用于完成进程中各段的寻址、现场保护与现场恢复。
! GDTR：GDT可以存放在内存的任何位置，当程序通过段寄存器引用一个段描述符时候，需要取得 GDT 的入口，GDTR所标识的即为此入口。在操作系统对 GDT 的初始化完成后，可以利用 LGDT指令将 GDT 基地址加载到 GDTR
! IDT: 中断描述符表，保存保护模式下所有中断服务的入口地址，类似于实模式下的中断向量表
! IDTR: IDT基址寄存器，保存 IDT 的起始地址。
end_move:
    mov ax,#SETUPSEG            ! right, forgot this at first. didn't work :-)
    mov ds,ax
    lidt idt_48                 ! load idt with 0,0
    lgdt gdt_48                 ! load gdt with whatever appropriate

! that was painless, now we enable A20
! 打开A20，意味着CPU可以进行 32 位寻址，最大寻址空间变为4GB。
    call empty_8042
    mov al,#0xD1                ! command write
    out #0x64,al
    call empty_8042
    mov al,#0xDF
    out #0x60,al
    call empty_8042
! 32位的中断基址和16位的中断机制在原理上有比较大的差别:
!   最明显的是16位中断机制用的是中断向量表，中断向量表的起始位置在 0x00000 处，这个位置是固定的。
!   32位的中断机制利用的是中断描述符表 IDT，位置是不固定的，可以按设计要求灵活安排，由IDTR寄存器来锁定其位置

! well, that went ok, I hope. Now we have to reprogram the interrupts :-(
! we put them right after the intel-reserved hardware interrupts, at
! int 0x20-0x2F. There they won't mess up anything. Sadly IBM really
! messed this up with the original PC, and they haven't been able to
! rectify it afterwards. Thus the bios puts interrupts at 0x08-0x0f,
! which is used for the internal hardware interrupts as well. We just
! have to reprogram the 8259's, and it isn't fun.
! 设置 8259A 中断控制器
    mov al,#0x11        ! initialization sequence
    out #0x20,al        ! send it to 8259A-1
    .word   0x00eb,0x00eb       ! jmp $+2, jmp $+2
    out #0xA0,al        ! and to 8259A-2
    .word   0x00eb,0x00eb
    mov al,#0x20        ! start of hardware int's (0x20)
    out #0x21,al
    .word   0x00eb,0x00eb
    mov al,#0x28        ! start of hardware int's 2 (0x28)
    out #0xA1,al
    .word   0x00eb,0x00eb
    mov al,#0x04        ! 8259-1 is master
    out #0x21,al
    .word   0x00eb,0x00eb
    mov al,#0x02        ! 8259-2 is slave
    out #0xA1,al
    .word   0x00eb,0x00eb
    mov al,#0x01        ! 8086 mode for both
    out #0x21,al
    .word   0x00eb,0x00eb
    out #0xA1,al
    .word   0x00eb,0x00eb
    mov al,#0xFF        ! mask off all interrupts for now
    out #0x21,al
    .word   0x00eb,0x00eb
    out #0xA1,al

! well, that certainly wasn't fun :-(. Hopefully it works, and we don't
! need no steenking BIOS anyway (except for the initial loading :-).
! The BIOS-routine wants lots of unnecessary data, and it's less
! "interesting" anyway. This is how REAL programmers do it.
!
! Well, now's the time to actually move into protected mode. To make
! things as simple as possible, we do no register set-up or anything,
! we let the gnu-compiled 32-bit programs do that. We just jump to
! absolute address 0x00000, in 32-bit protected mode.
! 开启保护模式
! 开启保护模式之后，一个重要的特征就是要根据GDT表来决定后续将执行哪里的程序。
    mov ax,#0x0001  ! protected mode (PE) bit
    lmsw    ax      ! This is it!
    jmpi 0,8        ! 跳转到第0的内存块的0地址处开始执行 jmp offset 0 of segment 8 (cs)
                    ! 此处 0 表示段内偏移，8(0x1000)表示保护模式下的段选择符，用于选择描述符表和描述符表项以及所要求的特权级。
                    ! 此处8表示为 x01000：
                    !   1. 最后两位 00 表示内核特权级，与之对应的是用户特权级 11
                    !   2. 第三位的 0 表示 GDT 表，如果是 1，则表示 LDT。
                    !   3. 第一位的 1 表示所选的表(在此处是GDT表)的第 1 项(GDT表项号排序为：0、1、2)来确定代码段的基地址和段限长等信息。
                    ! 总之这条命令跳转到 head.s 中执行。

! This routine checks that the keyboard command queue is empty
! No timeout is used - if this hangs there is something wrong with
! the machine, and we probably couldn't proceed anyway.
empty_8042:
    .word   0x00eb,0x00eb
    in  al,#0x64    ! 8042 status port
    test    al,#2       ! is input buffer full?
    jnz empty_8042  ! yes - loop
    ret

gdt:
    .word   0,0,0,0     ! dummy

    .word   0x07FF      ! 8Mb - limit=2047 (2048*4096=8Mb)
    .word   0x0000      ! base address=0
    .word   0x9A00      ! code read/exec
    .word   0x00C0      ! granularity=4096, 386

    .word   0x07FF      ! 8Mb - limit=2047 (2048*4096=8Mb)
    .word   0x0000      ! base address=0
    .word   0x9200      ! data read/write
    .word   0x00C0      ! granularity=4096, 386

idt_48:
    .word   0           ! idt limit=0
    .word   0,0         ! idt base=0L

gdt_48:
    .word   0x800       ! gdt limit=2048, 256 GDT entries
    .word   512+gdt,0x9 ! gdt base = 0X9xxxx
    
.text
endtext:
.data
enddata:
.bss
endbss:
