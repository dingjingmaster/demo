/*
 *  linux/boot/head.s
 *
 *  (C) 1991  Linus Torvalds
 */

/*
 *  head.s contains the 32-bit startup code.
 *
 * NOTE!!! Startup happens at absolute address 0x00000000, which is also where
 * the page directory will exist. The startup code will be overwritten by
 * the page directory.
 */

/**
 * @brief
 *  header.s 这块代码编译后是嵌入到操作系统里的。
 *  +---------------------------------------+
 *  |  header.s (0x00000) 25KB + 184B       |
 *  +---------------------------------------+
 *  |  在(0x064B8) 之后属于 Linux 内核      |
 *  +---------------------------------------+
 *
 */

.text
.globl idt,gdt,pg_dir,tmp_floppy_area
!-------------------------- start
pg_dir:                         ! pg_dir: 用于标识内核分页机制完成后的内核起始位置，也就是物理内存的起始位置：0x000000，head程序将要在此建立也目录表，为分页机制做准备。
.globl startup_32               ! 实模式下CS是代码段基址。在保护模式下，CS是代码段选择符。
startup_32:
    movl $0x10,%eax             ! 将寄存器从实模式转为保护模式，0x10 应该看成二进制的 '0001 0000'
    mov %ax,%ds                 !   最后两位的 00 表示内核特权级；第三位的 0 表示 GDT 表，第 4、5 两位的 10 表示 GDT 表的 2 项，也就是第三项。
    mov %ax,%es                 !   也就是说，4个寄存器用的是同一个全局描述符，它们的段基址、段限长和特权级都是相同的
    mov %ax,%fs                 ! ..
    mov %ax,%gs                 ! ..
!-------------------------- end
    lss stack_start,%esp
    call setup_idt
    call setup_gdt
    movl $0x10,%eax             ! reload all the segment registers
    mov %ax,%ds                 ! after changing gdt. CS was already
    mov %ax,%es                 ! reloaded in 'setup_gdt'
    mov %ax,%fs                 !
    mov %ax,%gs                 !
    lss stack_start,%esp        ! SS 也要转变为栈段选择符，栈顶指针也成为了32位的ESP

!-------------------------- start 检查 A20 是否打开
    xorl %eax,%eax              ! A20如果没有打开，则计算机处于实模式下，超过 0xFFFFF 寻址必然回滚。0x100000 就会回滚为 0x000000。也就是这两个地方的数据是一致的，检测数据是否一致就可以确定 A20 是否开启
1:  incl %eax                   ! check that A20 really IS enabled
    movl %eax,0x000000          ! loop forever if it isn't
    cmpl %eax,0x100000
    je 1b
!-------------------------- end

/*
 * NOTE! 486 should set bit 16, to check for write-protect in supervisor
 * mode. Then it would be unnecessary with the "verify_area()"-calls.
 * 486 users probably want to set the NE (#5) bit also, so as to use
 * int 16 for math errors.
 */
! 检测数学协处理器是否存在
! x87 协处理器是在 1980 年推出的，旨在弥补 x86 系列在进行浮点运算时候的不足，那时的 x87 是外置的、可选芯片
! 1989 年，Intel发布了 486 处理器，自此之后，CPU一般都内置了协处理器。
! 对于 486 之前的计算机而言，操作系统检验 x87 协处理器是否存在就会非常必要。
!-------------------------- start 检查数字协处理器
    movl %cr0,%eax              ! 检查数学协处理器是否存在
    andl $0x80000011,%eax       ! Save PG,PE,ET
/* "orl $0x10020,%eax" here for 486 might be good */
    orl $2,%eax                 ! set MP
    movl %eax,%cr0
    call check_x87              ! 检查数字协处理器
    jmp after_page_tables       !! NOTE:// 执行 main 之前的最后一步

/*
 * We depend on ET to be correct. This checks for 287/387.
 */
check_x87:
    fninit
    fstsw %ax
    cmpb $0,%al
    je 1f                       ! no coprocessor: have to set bits */
    movl %cr0,%eax
    xorl $6,%eax                ! reset MP, set EM 
    movl %eax,%cr0
    ret
!-------------------------- end

.align 2
1:  .byte 0xDB,0xE4             ! fsetpm for 287, ignored by 387 */
    ret

/*
 *  setup_idt
 *
 *  sets up a idt with 256 entries pointing to
 *  ignore_int, interrupt gates. It then loads
 *  idt. Everything that wants to install itself
 *  in the idt-table may do so themselves. Interrupts
 *  are enabled elsewhere, when we can be relatively
 *  sure everything is ok. This routine will be over-
 *  written by the page tables.
 */
setup_idt:
    lea ignore_int,%edx
    movl $0x00080000,%eax
    movw %dx,%ax                ! selector = 0x0008 = cs */
    movw $0x8E00,%dx            ! interrupt gate - dpl=0, present 

    lea idt,%edi
    mov $256,%ecx
rp_sidt:                        !
    movl %eax,(%edi)
    movl %edx,4(%edi)
    addl $8,%edi
    dec %ecx
    jne rp_sidt
    lidt idt_descr
    ret

/*
 *  setup_gdt
 *
 *  This routines sets up a new gdt and loads it.
 *  Only two entries are currently built, the same
 *  ones that were built in init.s. The routine
 *  is VERY complicated at two whole lines, so this
 *  rather long comment is certainly needed :-).
 *  This routine will beoverwritten by the page tables.
 */
setup_gdt:                      !
    lgdt gdt_descr
    ret

/*
 * I put the kernel page tables right after the page directory,
 * using 4 of them to span 16 Mb of physical memory. People with
 * more than 16MB will have to expand this.
 */
.org 0x1000
pg0:

.org 0x2000
pg1:

.org 0x3000
pg2:

.org 0x4000
pg3:

.org 0x5000
/*
 * tmp_floppy_area is used by the floppy-driver when DMA cannot
 * reach to a buffer-block. It needs to be aligned, so that it isn't
 * on a 64kB border.
 */
tmp_floppy_area:                !
    .fill 1024,1,0

after_page_tables:
    pushl $0                    ! These are the parameters to main :-)
    pushl $0
    pushl $0
    pushl $L6                   ! main 函数推出，就会返回到这里的标号 L6 处继续执行下去，并产生死循环.
    pushl $main
    jmp setup_paging            ! NOTE:// 创建分页机制，执行完跳入 main 函数执行。
L6:
    jmp L6                      ! main should never return here, but
                                ! just in case, we know what happens.




/* This is the default interrupt "handler" :-) */
int_msg:
    .asciz "Unknown interrupt\n\r"
.align 2
ignore_int:                     !
    pushl %eax
    pushl %ecx
    pushl %edx
    push %ds
    push %es
    push %fs
    movl $0x10,%eax
    mov %ax,%ds
    mov %ax,%es
    mov %ax,%fs
    pushl $int_msg
    call printk
    popl %eax
    pop %fs
    pop %es
    pop %ds
    popl %edx
    popl %ecx
    popl %eax
    iret


/*
 * Setup_paging
 *
 * This routine sets up paging by setting the page bit
 * in cr0. The page tables are set up, identity-mapping
 * the first 16MB. The pager assumes that no illegal
 * addresses are produced (ie >4Mb on a 4Mb machine).
 *
 * NOTE! Although all physical memory should be identity
 * mapped by this routine, only the kernel page functions
 * use the >1Mb addresses directly. All "normal" functions
 * use just the lower 1Mb, or the local data space, which
 * will be mapped to some other place - mm keeps track of
 * that.
 *
 * For those with more memory than 16 Mb - tough luck. I've
 * not got it, why should you :-) The source is here. Change
 * it. (Seriously - it shouldn't be too difficult. Mostly
 * change some constants etc. I left it at 16Mb, as my machine
 * even cannot be extended past that (ok, but it was cheap :-)
 * I've tried to show which constants to change by having
 * some kind of marker at them (search for "16Mb"), but I
 * won't guarantee that's all :-( )
 */
.align 2
! 0x0000 ~ 0x4FFF,20K 页目录以及4个页表
! +--------------------+---------------------------+----------------------------+---------------------------------+---------------------------------+
! | 0x0000(_pg_dir)    | 0x1000(pag0)              | 0x2000(pag1)               | 0x3000(pag2)                    | 0x4000(pag3)                    | (0x4FFF)
! +--------------------+---------------------------+----------------------------+---------------------------------+---------------------------------+
! 这4个页表是内核专属的页表，将来每个用户进程都有它们专属的页表
!
! 分页之后内存分布示意图：
!
!
!
!
!
!
!
!
!
!
!
setup_paging:
    movl $1024*5,%ecx           ! 将页目录表和 4 个也表放在物理内存的起始位置(这是操作系统能够掌控全局、掌控进程在内存中安全运行的基石之一...) 5 pages - pg_dir+4 page tables */
    xorl %eax,%eax
    xorl %edi,%edi              ! pg_dir is at 0x000
    cld;rep;stosl
    movl $pg0+7,pg_dir          ! set present bit/user r/w
    movl $pg1+7,pg_dir+4        ! --------- " " --------- 
    movl $pg2+7,pg_dir+8        ! --------- " " --------- 
    movl $pg3+7,pg_dir+12       ! --------- " " --------- 
    movl $pg3+4092,%edi
    movl $0xfff007,%eax         ! 16Mb - 4096 + 7 (r/w user,p)
    std
1:  stosl                       ! fill pages backwards - more efficient :-)
    subl $0x1000,%eax
    jge 1b
!------------------------------- start 将CR3指向页目录表，意味着操作系统认定 0x0000 这个位置就是页目录表的起始位置
    xorl %eax,%eax              ! pg_dir is at 0x0000
    movl %eax,%cr3              ! cr3 - page directory start
!------------------------------- end

!------------------------------- start 启动分页机制开关 PG 标志位，以启动分页寻址模式
    movl %cr0,%eax
    orl $0x80000000,%eax
    movl %eax,%cr0              ! set paging (PG) bit
!------------------------------- end
    ret                         ! this also flushes prefetch-queue

.align 2
.word 0
idt_descr:
    .word 256*8-1       # idt contains 256 entries
    .long idt
.align 2
.word 0
gdt_descr:
    .word 256*8-1       # so does gdt (not that that's any
    .long gdt       # magic number, but it works for me :^)

    .align 8
idt:    .fill 256,8,0       # idt is uninitialized

gdt:    .quad 0x0000000000000000    /* NULL descriptor */
    .quad 0x00c09a0000000fff    /* 16Mb */
    .quad 0x00c0920000000fff    /* 16Mb */
    .quad 0x0000000000000000    /* TEMPORARY - don't use */
    .fill 252,8,0           /* space for LDT's and TSS's etc */
