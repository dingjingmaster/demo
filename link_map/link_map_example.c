#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <link.h>
#include <elf.h>
#include <unistd.h>

// 用于遍历符号表的辅助函数
void print_symbol_info(ElfW(Sym) *sym, const char *strtab, int i)
{
    if (sym->st_name == 0) return;
    
    const char *sym_name = strtab + sym->st_name;
    const char *sym_type = "";
    const char *sym_bind = "";
    
    // 符号类型
    switch (ELF64_ST_TYPE(sym->st_info)) {
        case STT_NOTYPE:  sym_type = "NOTYPE"; break;
        case STT_OBJECT:  sym_type = "OBJECT"; break;
        case STT_FUNC:    sym_type = "FUNC"; break;
        case STT_SECTION: sym_type = "SECTION"; break;
        case STT_FILE:    sym_type = "FILE"; break;
        case STT_COMMON:  sym_type = "COMMON"; break;
        case STT_TLS:     sym_type = "TLS"; break;
        default:          sym_type = "UNKNOWN"; break;
    }
    
    // 符号绑定
    switch (ELF64_ST_BIND(sym->st_info)) {
        case STB_LOCAL:  sym_bind = "LOCAL"; break;
        case STB_GLOBAL: sym_bind = "GLOBAL"; break;
        case STB_WEAK:   sym_bind = "WEAK"; break;
        default:         sym_bind = "UNKNOWN"; break;
    }
    
    // 只打印有意义的符号
    if (strlen(sym_name) > 0 && sym->st_value != 0) {
        printf("    符号[%d]: %s (地址: 0x%lx, 类型: %s, 绑定: %s, 大小: %lu)\n",
               i, sym_name, sym->st_value, sym_type, sym_bind, sym->st_size);
    }
}

// 解析动态段，获取符号表信息
void parse_dynamic_section(struct link_map *map)
{
    ElfW(Dyn) *dyn = map->l_ld;
    if (!dyn) return;
    
    ElfW(Sym) *symtab = NULL;
    char *strtab = NULL;
    ElfW(Word) symtab_size = 0;
    
    // 遍历动态段，查找符号表和字符串表
    while (dyn->d_tag != DT_NULL) {
        switch (dyn->d_tag) {
            case DT_SYMTAB:
                symtab = (ElfW(Sym) *)(map->l_addr + dyn->d_un.d_ptr);
                break;
            case DT_STRTAB:
                strtab = (char *)(map->l_addr + dyn->d_un.d_ptr);
                break;
            case DT_STRSZ:
                // 字符串表大小（当前示例中未使用）
                break;
            case DT_HASH:
                // 从哈希表获取符号表大小
                {
                    ElfW(Word) *hash_table = (ElfW(Word) *)(map->l_addr + dyn->d_un.d_ptr);
                    symtab_size = hash_table[1]; // nchain
                }
                break;
        }
        dyn++;
    }
    
    // 如果找到了符号表和字符串表，打印符号信息
    if (symtab && strtab && symtab_size > 0) {
        printf("  符号表信息 (共 %u 个符号):\n", symtab_size);
        
        int count = 0;
        for (ElfW(Word) i = 0; i < symtab_size && count < 20; i++) { // 限制显示前20个符号
            if (symtab[i].st_name != 0 && symtab[i].st_value != 0) {
                print_symbol_info(&symtab[i], strtab, i);
                count++;
            }
        }
        
        if (symtab_size > 20) {
            printf("    ... (还有 %u 个符号未显示)\n", symtab_size - 20);
        }
    } else {
        printf("  无法获取符号表信息\n");
    }
}

// 遍历所有加载的动态库
int iterate_loaded_libraries(struct dl_phdr_info *info, size_t size, void *data)
{
    (void)size; // 消除未使用参数警告
    (void)data; // 消除未使用参数警告
    
    printf("\n库信息:\n");
    printf("  名称: %s\n", info->dlpi_name[0] ? info->dlpi_name : "[主程序]");
    printf("  基地址: 0x%lx\n", info->dlpi_addr);
    printf("  程序头数量: %d\n", info->dlpi_phnum);
    
    // 显示程序头信息
    for (int i = 0; i < info->dlpi_phnum; i++) {
        const ElfW(Phdr) *phdr = &info->dlpi_phdr[i];
        switch (phdr->p_type) {
            case PT_LOAD:
                printf("  LOAD段[%d]: 虚拟地址=0x%lx, 文件偏移=0x%lx, 大小=%lu\n",
                       i, phdr->p_vaddr, phdr->p_offset, phdr->p_memsz);
                break;
            case PT_DYNAMIC:
                printf("  DYNAMIC段[%d]: 虚拟地址=0x%lx, 大小=%lu\n",
                       i, phdr->p_vaddr, phdr->p_memsz);
                break;
            case PT_INTERP:
                printf("  INTERP段[%d]: 解释器路径段\n", i);
                break;
        }
    }
    
    return 0; // 继续遍历
}

// 使用 link_map 结构遍历动态库
void traverse_link_map()
{
    printf("=== 使用 link_map 结构遍历动态库 ===\n");
    
    // 获取动态链接器信息
    void *handle = dlopen(NULL, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "无法获取主程序句柄: %s\n", dlerror());
        return;
    }
    
    struct link_map *map;
    if (dlinfo(handle, RTLD_DI_LINKMAP, &map) != 0) {
        fprintf(stderr, "无法获取 link_map: %s\n", dlerror());
        dlclose(handle);
        return;
    }
    
    // 遍历 link_map 链表
    struct link_map *current = map;
    int lib_count = 0;
    
    while (current) {
        lib_count++;
        printf("\n库 #%d:\n", lib_count);
        printf("  名称: %s\n", current->l_name[0] ? current->l_name : "[主程序]");
        printf("  基地址: 0x%lx\n", current->l_addr);
        printf("  动态段地址: %p\n", current->l_ld);
        
        // 解析符号表
        parse_dynamic_section(current);
        
        current = current->l_next;
    }
    
    printf("\n总共找到 %d 个动态库\n", lib_count);
    dlclose(handle);
}

// 查找特定符号
void find_symbol(const char *symbol_name)
{
    printf("\n=== 查找符号: %s ===\n", symbol_name);
    
    void *handle = dlopen(NULL, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "无法获取主程序句柄: %s\n", dlerror());
        return;
    }
    
    // 使用 dlsym 查找符号
    void *symbol_addr = dlsym(handle, symbol_name);
    if (symbol_addr) {
        printf("找到符号 '%s'，地址: %p\n", symbol_name, symbol_addr);
        
        // 获取符号信息
        Dl_info info;
        if (dladdr(symbol_addr, &info)) {
            printf("  所在库: %s\n", info.dli_fname ? info.dli_fname : "[主程序]");
            printf("  库基地址: %p\n", info.dli_fbase);
            printf("  符号名称: %s\n", info.dli_sname ? info.dli_sname : "[未知]");
            printf("  符号地址: %p\n", info.dli_saddr);
        }
    } else {
        printf("未找到符号 '%s': %s\n", symbol_name, dlerror());
    }
    
    dlclose(handle);
}

// 获取当前进程的内存映射信息
void show_memory_maps()
{
    printf("\n=== 进程内存映射信息 ===\n");
    
    FILE *maps = fopen("/proc/self/maps", "r");
    if (!maps) {
        perror("无法打开 /proc/self/maps");
        return;
    }
    
    char line[256];
    int count = 0;
    while (fgets(line, sizeof(line), maps) && count < 20) { // 限制显示前20行
        // 解析内存映射行
        unsigned long start, end;
        char perms[8], path[256];
        unsigned long offset;
        int major, minor;
        unsigned long inode;
        
        int parsed = sscanf(line, "%lx-%lx %7s %lx %x:%x %lu %255s",
                           &start, &end, perms, &offset, &major, &minor, &inode, path);
        
        if (parsed >= 7) {
            printf("0x%08lx-0x%08lx %s", start, end, perms);
            if (parsed == 8) {
                printf(" %s", path);
            }
            printf("\n");
            count++;
        }
    }
    
    if (count == 20) {
        printf("... (还有更多映射未显示)\n");
    }
    
    fclose(maps);
}

// 动态加载库并检查其符号
void dynamic_load_example()
{
    printf("\n=== 动态加载库示例 ===\n");
    
    // 尝试加载 libm (数学库)
    void *libm_handle = dlopen("libm.so.6", RTLD_LAZY);
    if (!libm_handle) {
        printf("无法加载 libm.so.6: %s\n", dlerror());
        return;
    }
    
    printf("成功加载 libm.so.6\n");
    
    // 获取 link_map
    struct link_map *libm_map;
    if (dlinfo(libm_handle, RTLD_DI_LINKMAP, &libm_map) == 0) {
        printf("libm 信息:\n");
        printf("  名称: %s\n", libm_map->l_name);
        printf("  基地址: 0x%lx\n", libm_map->l_addr);
        
        // 查找 sin 函数
        void *sin_func = dlsym(libm_handle, "sin");
        if (sin_func) {
            printf("  找到 sin 函数，地址: %p\n", sin_func);
            
            // 测试调用
            double (*sin_ptr)(double) = (double (*)(double))sin_func;
            double result = sin_ptr(3.14159/2);
            printf("  sin(π/2) = %f\n", result);
        }
    }
    
    dlclose(libm_handle);
}

int main()
{
    printf("Linux C 动态库和符号信息获取示例\n");
    printf("=====================================\n");
    
    // 1. 使用 dl_iterate_phdr 遍历程序头
    printf("\n=== 使用 dl_iterate_phdr 遍历程序头 ===\n");
    dl_iterate_phdr(iterate_loaded_libraries, NULL);
    
    // 2. 使用 link_map 结构遍历动态库
    traverse_link_map();
    
    // 3. 查找特定符号
    find_symbol("printf");
    find_symbol("malloc");
    find_symbol("main");
    
    // 4. 显示内存映射
    show_memory_maps();
    
    // 5. 动态加载库示例
    dynamic_load_example();
    
    return 0;
} 