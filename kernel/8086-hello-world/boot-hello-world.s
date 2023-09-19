;#######################################################
; FileName     : l-asm-boot1.asm
; Author       : DingJing
; Mail         : dingjing@live.cn
; Created Time : Wed 14 Jun 2023 05:09:46 PM CST
;#######################################################

; 硬盘引导扇区代码，打印字符串

; 8086 可访问内存 1MB。
; 其中:
;       0x00000 - 0x9FFFF 属于常规内存
;       0xA0000 - 0xEFFFF 这块内存是由特定外设提供，包括显卡
;       0xF0000 - 0xFFFFF 由主板芯片提供，即：ROM-BIOS
;
;   0xB8000 - 0xBFFFF 是留给显卡使用的，显示 80x25(80行，每行25字符) 的文本
;
;   ASCII 由8bit表示，其中最高位置0
;   0xB8000 - 0xBFFFF 用两个字节表示一个字符的显示，
;       高八位表示要显示的ASCII码值
;       低八位表示显示属性：前四位表示前景色、后四位表示背景色
;
;   访问内存寄存器，可以使用DS或ES(不可执行修改DS和ES的值)

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

; 取得标号number的偏移地址
;mov ax,number
;mov bx,10

; 设置数据段的基址
mov cx,cs
mov ds,cx


; 无限循环
infi: jmp near infi

; $     : 表示当前行的汇编地址
; $$    : 表示第一行的汇编地址
; $-$$  : 表示得到前面代码占用的字节数
; 主引导扇区总长度必须是 512 字节，用0占位。
times (512 - 2 - ($-$$)) db 0

; 主引导扇区最后两个字节必须是: 0x55aa
db 0x55,0xaa

