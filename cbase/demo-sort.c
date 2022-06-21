/*************************************************************************
> FileName: demo-sort.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Tue 21 Jun 2022 02:07:23 PM CST
 ************************************************************************/
#include <stdio.h>

void print_arr (int arr[], int len);

// 冒泡排序
void bubble_sort(int arr[], int len);

// 选择排序
void selection_sort(int a[], int len);

// 插入排序
void insertion_sort(int arr[], int len);

// 希尔排序
void shell_sort(int arr[], int len);

// 归并排序
void merge_sort(int arr[], int len);
void merge_sort_recurive(int arr[], const int len);

// 快排
void quick_sort_recursive (int arr[], int len);


int main (void)
{
    int arr [] = {1, 30, 28, 2, 38, 98, 76, 23, 46, 89, 77, 22, 11, 44};

    int len = sizeof (arr) / sizeof (arr[0]);

    // 冒泡排序
    bubble_sort (arr, len);

    // 选择排序
    selection_sort (arr, len);

    // 插入排序
    insertion_sort(arr, len);

    // 希尔排序
    shell_sort(arr, len);

    // 归并排序
    merge_sort(arr, len);


    print_arr (arr, len);

    return 0;
}

void print_arr (int arr[], int len)
{
    for (int i = 0; i < len; ++i) {
        printf ("%d ", arr[i]);
    }
    printf ("\n");
}

void bubble_sort(int arr[], int len) 
{
    int i, j, temp;

    for (i = 0; i < len - 1; i++) {
        for (j = 0; j < len - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void selection_sort(int a[], int len) 
{
    int i, j, temp;
 
    for (i = 0; i < len - 1 ; i++) {
        // 记录最小值，第一个元素默认最小
        int min = i;                  
        for (j = i + 1; j < len; j++) {
            // 访问未排序的元素
            if (a[j] < a[min]) {
                // 找到目前最小值, 记录最小值
                min = j;    
            }
        }
        if(min != i) {
            temp = a[min];  // 交换两个变量
            a[min] = a[i];
            a[i] = temp;
        }
    }
}

void insertion_sort(int arr[], int len)
{
    int i, j, temp;
    
    for (i = 1; i < len; i++) {
        temp = arr[i];
        for (j = i; j > 0 && arr[j - 1] > temp; j--) {
            arr[j] = arr[j-1];
        }
        arr[j] = temp;
    }
}


void shell_sort(int arr[], int len) 
{
    int gap, i, j;
    int temp;
    
    for (gap = len >> 1; gap > 0; gap = gap >> 1) {
        for (i = gap; i < len; i++) {
            temp = arr[i];
            for (j = i - gap; j >= 0 && arr[j] > temp; j -= gap) {
                arr[j + gap] = arr[j];
            }
            arr[j + gap] = temp;
        }
    }
}

int min(int x, int y) 
{
    return x < y ? x : y;
}

void merge_sort(int arr[], int len) 
{
    int* a = arr;
    int* b = (int*) malloc(len * sizeof(int));
    int seg, start;
    
    for (seg = 1; seg < len; seg += seg) {
        for (start = 0; start < len; start += seg + seg) {
            int low = start, mid = min(start + seg, len), high = min(start + seg + seg, len);
            int k = low;
            int start1 = low, end1 = mid;
            int start2 = mid, end2 = high;
            while (start1 < end1 && start2 < end2) {
                b[k++] = a[start1] < a[start2] ? a[start1++] : a[start2++];
            }
            
            while (start1 < end1) {
                b[k++] = a[start1++];
            }
            
            while (start2 < end2) {
                b[k++] = a[start2++];
            }
        }

        int* temp = a;
        a = b;
        b = temp;
    }

    if (a != arr) {
        int i;
        for (i = 0; i < len; i++) {
            b[i] = a[i];
        }
        b = a;
    }

    free(b);
}


void merge_sort_recursive1(int arr[], int reg[], int start, int end) 
{
    if (start >= end) {
        return;
    }

    int len = end - start, mid = (len >> 1) + start;
    int start1 = start, end1 = mid;
    int start2 = mid + 1, end2 = end;
    merge_sort_recursive1(arr, reg, start1, end1);
    merge_sort_recursive1(arr, reg, start2, end2);
    int k = start;

    while (start1 <= end1 && start2 <= end2) {
        reg[k++] = arr[start1] < arr[start2] ? arr[start1++] : arr[start2++];
    }
    
    while (start1 <= end1) {
        reg[k++] = arr[start1++];
    }
    
    while (start2 <= end2) {
        reg[k++] = arr[start2++];
    }

    for (k = start; k <= end; k++) {
        arr[k] = reg[k];
    }
}
void merge_sort_recurive(int arr[], const int len) 
{
    int reg[len];
    merge_sort_recursive1(arr, reg, 0, len - 1);
}


void swap(int *x, int *y) 
{
    int t = *x;
    *x = *y;
    *y = t;
}

void quick_sort_recursive1(int arr[], int start, int end) {
    if (start >= end)
        return;
    int mid = arr[end];
    int left = start, right = end - 1;
    while (left < right) {
        while (arr[left] < mid && left < right) {
            left++;
        }

        while (arr[right] >= mid && left < right) {
            right--;
        }

        swap(&arr[left], &arr[right]);
    }

    if (arr[left] >= arr[end]) {
        swap(&arr[left], &arr[end]);
    } else {
        left++;
    }
    
    if (left) {
        quick_sort_recursive1(arr, start, left - 1);
    }

    quick_sort_recursive1(arr, left + 1, end);
}

void quick_sort_recursive (int arr[], int len)
{
    quick_sort_recursive1(arr, 0, len - 1);
}

