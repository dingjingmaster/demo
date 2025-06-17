CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99
LDFLAGS = -ldl

TARGET = link_map_example
SOURCE = link_map_example.c

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

# 显示帮助信息
help:
	@echo "可用目标:"
	@echo "  all    - 编译程序"
	@echo "  run    - 编译并运行程序"
	@echo "  clean  - 清理生成的文件"
	@echo "  help   - 显示此帮助信息" 