########################################################
# FileName     : test.S
# Author       : DingJing
# Mail         : dingjing@live.cn
# Created Time : Thu 13 Apr 2023 03:55:24 PM CST
########################################################

start:
mov     $0x48,%al
outb    %al,$0xf1
mov     $0x65,%al
outb    %al,$0xf1
mov     $0x6c,%al
outb    %al,$0xf1
mov     $0x6f,%al
outb    %al,$0xf1
mov     $0x0a,%al
outb    %al,$0xf1

hlt
