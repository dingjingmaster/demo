#include <stdio.h>

// 外部函数
void outer_function(int x) {
    // 在函数内部定义内部函数
    void inner_function(int y) {
        printf("内部函数被调用，参数 y = %d\n", y);
        printf("可以访问外部函数的变量 x = %d\n", x);
    }
    
    printf("外部函数被调用，参数 x = %d\n", x);
    
    // 调用内部函数
    inner_function(x * 2);
}

// 另一个示例：使用内部函数进行回调
void process_numbers(int arr[], int size) {
    // 定义内部函数用于打印数组元素
    void print_element(int index) {
        printf("数组[%d] = %d\n", index, arr[index]);
    }
    
    // 定义内部函数用于计算总和
    int calculate_sum(void) {
        int sum = 0;
        for (int i = 0; i < size; i++) {
            sum += arr[i];
        }
        return sum;
    }
    
    printf("开始处理数组...\n");
    
    // 使用内部函数打印每个元素
    for (int i = 0; i < size; i++) {
        print_element(i);
    }
    
    // 使用内部函数计算总和
    int total = calculate_sum();
    printf("数组元素总和: %d\n", total);
}

int main() {
    // 测试第一个示例
    printf("测试嵌套函数示例1:\n");
    outer_function(10);
    
    // 测试第二个示例
    printf("\n测试嵌套函数示例2:\n");
    int numbers[] = {1, 2, 3, 4, 5};
    process_numbers(numbers, 5);
    
    return 0;
} 