/*************************************************************************
> FileName: str-hashtable.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2026年03月02日 星期一 16时28分29秒
 ************************************************************************/
// SPDX-License-Identifier: GPL-2.0
/*
 * 使用RCU的字符串哈希表内核模块示例
 * 
 * 演示如何在内核模块中使用rhashtable和RCU进行线程安全的字符串键值对操作
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/rhashtable.h>
#include <linux/stringhash.h>
#include <linux/errno.h>
#include <linux/rcupdate.h>
#include <linux/rculist.h>

/* 支持RCU的数据结构 */
struct string_entry {
    /* rhashtable需要的节点 */
    struct rhash_head node;
    
    /* RCU头，用于延迟释放 */
    struct rcu_head rcu;
    
    /* 字符串键 */
    char *key;
    
    /* 值数据 */
    int value;
};

/* 自定义对象哈希函数 - 从对象中提取字符串键并哈希 */
static u32 string_obj_hashfn(const void *data, u32 len, u32 seed)
{
    const struct string_entry *obj = data;
    /* 使用full_name_hash进行字符串哈希 */
    return full_name_hash(NULL, obj->key, strlen(obj->key));
}

/* 自定义比较函数 - 比较字符串键是否匹配 */
static int string_obj_cmpfn(struct rhashtable_compare_arg *arg, const void *obj)
{
    const struct string_entry *entry = obj;
    const char *key = arg->key;
    
    /* 比较字符串键 */
    return strcmp(entry->key, key);
}

/* 字符串哈希表参数配置 */
static const struct rhashtable_params string_rhash_params = {
    /* rhash_head的偏移量 - 在string_entry结构中node字段的偏移 */
    .head_offset = offsetof(struct string_entry, node),
    
    /* 键的长度 - 对于字符串，使用0表示动态长度 */
    .key_len = 0,
    
    /* 键的偏移量 - 在string_entry结构中key字段的偏移 */
    .key_offset = offsetof(struct string_entry, key),
    
    /* 哈希函数 - 使用jhash作为默认哈希函数 */
    .hashfn = jhash,
    
    /* 对象哈希函数 - 用于从对象中提取键并哈希 */
    .obj_hashfn = string_obj_hashfn,
    
    /* 对象比较函数 - 用于比较键与对象 */
    .obj_cmpfn = string_obj_cmpfn,
    
    /* 自动扩容/缩容 */
    .automatic_shrinking = true,
    
    /* 最小表大小 */
    .min_size = 8,
    
    /* 最大表大小 */
    .max_size = 1024,
    
    /* 元素数量提示 */
    .nelem_hint = 64,
};

/* 全局哈希表 */
static struct rhashtable string_hashtable;

/* 模块初始化时插入的测试数据 */
static const struct {
    const char *key;
    int value;
} test_data[] = {
    {"apple", 10},
    {"banana", 20},
    {"cherry", 30},
    {"date", 40},
    {"elderberry", 50},
    {"fig", 60},
    {"grape", 70},
    {"honeydew", 80},
};

/* RCU回调函数，用于延迟释放字符串键 */
static void string_entry_rcu_free(struct rcu_head *rcu)
{
    struct string_entry *entry = container_of(rcu, struct string_entry, rcu);
    
    /* 释放字符串键 */
    kfree(entry->key);
    
    /* 释放条目本身 */
    kfree(entry);
}

/* 插入字符串键值对 */
static int insert_string_entry(struct rhashtable *ht, const char *key, int value)
{
    struct string_entry *entry;
    int ret;
    
    /* 分配新对象 */
    entry = kzalloc(sizeof(*entry), GFP_KERNEL);
    if (!entry)
        return -ENOMEM;
    
    /* 复制字符串键 */
    entry->key = kstrdup(key, GFP_KERNEL);
    if (!entry->key) {
        kfree(entry);
        return -ENOMEM;
    }
    
    /* 设置值 */
    entry->value = value;
    
    /* 插入哈希表 */
    ret = rhashtable_insert_fast(ht, &entry->node, string_rhash_params);
    if (ret) {
        kfree(entry->key);
        kfree(entry);
        return ret;
    }
    
    pr_info("插入: 键=%s, 值=%d\n", key, value);
    return 0;
}

/* RCU安全的查找字符串键 */
static struct string_entry *lookup_string_entry(struct rhashtable *ht, const char *key)
{
    struct string_entry *entry;
    
    rcu_read_lock();
    entry = rhashtable_lookup_fast(ht, key, string_rhash_params);
    /* 条目将在RCU读锁保护下使用 */
    rcu_read_unlock();

    if (!entry) {
        struct rhashtable_iter iter;
        rhashtable_walk_enter(ht, &iter);
        rhashtable_walk_start(&iter);
        
        while ((entry = rhashtable_walk_next(&iter)) && !IS_ERR(entry)) {
            if (strcmp(entry->key, key) == 0) {
                entry = entry;
                break;
            }
        }
        
        rhashtable_walk_stop(&iter);
        rhashtable_walk_exit(&iter);
    }
    
    return entry;
}

/* RCU安全的查找字符串键 - 返回前增加引用 */
static struct string_entry *lookup_string_entry_get(struct rhashtable *ht, const char *key)
{
    struct string_entry *entry;
    
    rcu_read_lock();
    entry = rhashtable_lookup_fast(ht, key, string_rhash_params);
    if (entry) {
        /* 如果需要，可以在这里增加引用计数 */
        /* 目前我们依赖于调用者在RCU读锁下使用条目 */
    }
    rcu_read_unlock();

    if (!entry) {
        struct rhashtable_iter iter;
        rhashtable_walk_enter(ht, &iter);
        rhashtable_walk_start(&iter);
        
        while ((entry = rhashtable_walk_next(&iter)) && !IS_ERR(entry)) {
            if (strcmp(entry->key, key) == 0) {
                entry = entry;
                break;
            }
        }
        
        rhashtable_walk_stop(&iter);
        rhashtable_walk_exit(&iter);
    }
    
    return entry;
}

/* RCU安全的删除字符串键值对 */
static int remove_string_entry(struct rhashtable *ht, const char *key)
{
    struct string_entry *entry;
    int ret = -ENOENT;
    
    rcu_read_lock();
    entry = rhashtable_lookup_fast(ht, key, string_rhash_params);
    if (entry) {
        /* 从哈希表中移除条目 */
        if (rhashtable_remove_fast(ht, &entry->node, string_rhash_params) == 0) {
            /* 使用RCU延迟释放 */
            kfree_rcu(entry, rcu);
            ret = 0;
            pr_info("删除: 键=%s\n", key);
        }
    }
    rcu_read_unlock();
    
    return ret;
}

/* 打印哈希表统计信息 */
static void print_hashtable_stats(struct rhashtable *ht)
{
    struct bucket_table *tbl;
    
    rcu_read_lock();
    tbl = rht_dereference_rcu(ht->tbl, ht);
    if (tbl) {
        pr_info("哈希表统计: 大小=%u, 元素数量=%u\n",
                tbl->size, atomic_read(&ht->nelems));
    }
    rcu_read_unlock();
}

/* RCU安全的清空哈希表 */
static void clear_hashtable_rcu(struct rhashtable *ht)
{
    struct rhashtable_iter iter;
    struct string_entry *entry;
    
    /* 使用迭代器遍历哈希表 */
    rhashtable_walk_enter(ht, &iter);
    rhashtable_walk_start(&iter);
    
    while ((entry = rhashtable_walk_next(&iter)) && !IS_ERR(entry)) {
        /* 从哈希表中移除条目 */
        if (rhashtable_remove_fast(ht, &entry->node, string_rhash_params) == 0) {
            /* 使用RCU延迟释放 */
            kfree_rcu(entry, rcu);
        }
    }
    
    rhashtable_walk_stop(&iter);
    rhashtable_walk_exit(&iter);
    
    /* 等待所有RCU回调完成 */
    synchronize_rcu();
}

/* RCU安全的遍历并打印所有条目 */
static void print_all_entries_rcu(struct rhashtable *ht)
{
    struct rhashtable_iter iter;
    struct string_entry *entry;
    
    pr_info("哈希表中的所有条目:\n");
    
    /* 使用迭代器遍历哈希表 */
    rhashtable_walk_enter(ht, &iter);
    rhashtable_walk_start(&iter);
    
    while ((entry = rhashtable_walk_next(&iter)) && !IS_ERR(entry)) {
        pr_info("  键=%s, 值=%d\n", entry->key, entry->value);
    }
    
    rhashtable_walk_stop(&iter);
    rhashtable_walk_exit(&iter);
}

/* 模块初始化函数 */
static int __init hash_example_init(void)
{
    int ret, i;
    
    pr_info("RCU字符串哈希表模块初始化\n");
    
    /* 初始化哈希表 */
    ret = rhashtable_init(&string_hashtable, &string_rhash_params);
    if (ret) {
        pr_err("哈希表初始化失败: %d\n", ret);
        return ret;
    }
    
    /* 插入测试数据 */
    for (i = 0; i < ARRAY_SIZE(test_data); i++) {
        ret = insert_string_entry(&string_hashtable, test_data[i].key, test_data[i].value);
        if (ret) {
            pr_err("插入失败: 键=%s, 错误=%d\n", test_data[i].key, ret);
            goto cleanup;
        }
    }
    
    /* 打印哈希表统计信息 */
    print_hashtable_stats(&string_hashtable);
    
    /* 打印所有条目 */
    print_all_entries_rcu(&string_hashtable);
    
    /* 测试查找 */
    for (i = 0; i < ARRAY_SIZE(test_data); i++) {
        struct string_entry *entry = lookup_string_entry(&string_hashtable, test_data[i].key);
        if (entry) {
            pr_info("查找: 键=%s, 值=%d\n", entry->key, entry->value);
        } else {
            pr_err("查找失败: 键=%s\n", test_data[i].key);
        }
    }
    
    /* 测试删除 */
    ret = remove_string_entry(&string_hashtable, "banana");
    if (ret) {
        pr_err("删除失败: 键=banana, 错误=%d\n", ret);
    }
    
    /* 再次查找已删除的项 */
    struct string_entry *entry = lookup_string_entry(&string_hashtable, "banana");
    if (entry) {
        pr_err("错误: 已删除的项仍可找到: 键=%s\n", entry->key);
    } else {
        pr_info("确认: 已删除的项无法找到: 键=banana\n");
    }
    
    /* 再次打印所有条目 */
    print_all_entries_rcu(&string_hashtable);
    
    return 0;
    
cleanup:
    clear_hashtable_rcu(&string_hashtable);
    rhashtable_destroy(&string_hashtable);
    return ret;
}

/* 模块退出函数 */
static void __exit hash_example_exit(void)
{
    pr_info("RCU字符串哈希表模块退出\n");
    
    /* 清空哈希表 */
    clear_hashtable_rcu(&string_hashtable);
    
    /* 等待所有RCU操作完成 */
    synchronize_rcu();
    
    /* 销毁哈希表 */
    rhashtable_destroy(&string_hashtable);
    
    pr_info("哈希表已销毁\n");
}

module_init(hash_example_init);
module_exit(hash_example_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("示例作者");
MODULE_DESCRIPTION("RCU字符串哈希表内核模块示例");
MODULE_VERSION("1.0");