# UTF-8 字符验证器 Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -O2
DEBUG_FLAGS = -g -DDEBUG
TARGET = utf8_example
LIB_TARGET = libutf8validator.a
SOURCES = utf8_validator.c
HEADERS = utf8_validator.h
EXAMPLE_SOURCE = utf8_example.c
OBJECTS = $(SOURCES:.c=.o)

# 默认目标
all: $(TARGET) $(LIB_TARGET)

# 编译可执行文件
$(TARGET): $(EXAMPLE_SOURCE) $(LIB_TARGET)
	$(CC) $(CFLAGS) -o $@ $(EXAMPLE_SOURCE) -L. -lutf8validator

# 编译静态库
$(LIB_TARGET): $(OBJECTS)
	ar rcs $@ $^

# 编译目标文件
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# 调试版本
debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

# 运行示例
run: $(TARGET)
	./$(TARGET)

# 运行内存检查（需要安装 valgrind）
memcheck: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

# 安装头文件和库（需要 root 权限）
install: $(LIB_TARGET) $(HEADERS)
	sudo cp $(HEADERS) /usr/local/include/
	sudo cp $(LIB_TARGET) /usr/local/lib/
	sudo ldconfig

# 卸载
uninstall:
	sudo rm -f /usr/local/include/utf8_validator.h
	sudo rm -f /usr/local/lib/libutf8validator.a
	sudo ldconfig

# 清理
clean:
	rm -f $(OBJECTS) $(TARGET) $(LIB_TARGET)

# 显示帮助
help:
	@echo "可用的 make 目标:"
	@echo "  all       - 编译所有目标（默认）"
	@echo "  $(TARGET) - 编译示例程序"
	@echo "  $(LIB_TARGET) - 编译静态库"
	@echo "  debug     - 编译调试版本"
	@echo "  run       - 运行示例程序"
	@echo "  memcheck  - 使用 valgrind 检查内存"
	@echo "  install   - 安装库和头文件到系统"
	@echo "  uninstall - 从系统卸载库和头文件"
	@echo "  clean     - 清理编译文件"
	@echo "  help      - 显示此帮助信息"

.PHONY: all debug run memcheck install uninstall clean help 