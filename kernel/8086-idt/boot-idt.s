;
; 尝试修改 中断描述符向量
;   8086 中断向量表的地址是无法修改的
; !!! 失败
;

section .text
org 0x100

start:
    cli
    MOV AX, 0
    MOV ES, AX                          ; 将 ES 寄存器设置为 0，以使用默认段地址

    MOV AX, CS
    MOV BX, NEW_ISR                     ; NEW_ISR 标签偏移量加载到 BX
    SUB BX, AX                          ; BX - AX, BX中包含 NEW_ISR 相对于代码段起始地址的偏移量
    MOV WORD [0x21 * 4], CS             ; 将 AX 中的值存储到中断向量表中向量号 0x21 的段地址位置
    MOV WORD [0x21 * 4 + 2], BX         ; 将 BX 中的值存储到中断向量表中向量号 0x21 的偏移位置

    sti

    ; 执行中断
    mov AH, 0x21
    int 0x21

; $     : 表示当前行的汇编地址
; $$    : 表示第一行的汇编地址
; $-$$  : 表示得到前面代码占用的字节数
    JMP $                               ; 无限循环


NEW_ISR:
    ; 此处是自定义的中断服务程序
    mov ax,0xb800
    mov es,ax

    ; 显示字符串 Hello World!
    mov byte [es:0x00],'H'
    mov byte [es:0x01],0x07
    mov byte [es:0x02],'e'
    mov byte [es:0x03],0x07
    mov byte [es:0x04],'l'
    mov byte [es:0x05],0x07
    mov byte [es:0x06],'l'
    mov byte [es:0x07],0x07
    mov byte [es:0x08],'o'
    mov byte [es:0x09],0x07
    mov byte [es:0x0a],','
    mov byte [es:0x0b],0x07
    mov byte [es:0x0c],'W'
    mov byte [es:0x0d],0x07
    mov byte [es:0x0e],'o'
    mov byte [es:0x0f],0x07
    mov byte [es:0x10],'r'
    mov byte [es:0x11],0x07
    mov byte [es:0x12],'l'
    mov byte [es:0x13],0x07
    mov byte [es:0x14],'d'
    mov byte [es:0x15],0x07
    mov byte [es:0x16],'!'
    mov byte [es:0x17],0x07
    IRET



; 主引导扇区总长度必须是 512 字节，用0占位。
times (512 - 2 - ($-$$)) db 0

; 主引导扇区最后两个字节必须是: 0x55aa
db 0x55,0xaa

