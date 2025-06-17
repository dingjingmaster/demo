CC      = gcc
CFLAGS  = -Wall -Wextra -g -std=c99
LDFLAGS = -ldl
curDir  = $(shell pwd)

src 	= $(strip $(subst $(curDir), ., $(wildcard $(curDir)/*.c)))
target 	= $(strip $(patsubst %.c, %.run, $(src)))

.PHONY: all clean

all: $(target)

$(target): $(src)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(target)

# 显示帮助信息
help:
	@echo "可用目标:"
	@echo "  clean  - 清理生成的文件"
	@echo "  help   - 显示此帮助信息" 
