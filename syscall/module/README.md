## 内核模块拦截系统调用

> 需要注意，高版本内核在编译时候可能报错 '`kallsyms_lookup_name` 函数未定义'，而使用`cat /proc/kallsyms | grep kallsyms_lookup_name` 可以看到这个调用，这是因为内内有导出这个函数，修改 `kernel/kallsyms.c` 在函数`kallsyms_lookup_name`后添加 `EXPORT_SYMBOL_GPL(kallsyms_lookup_name);` 重新编译安装内核、模块，重新生成 initramfs<br/>
> PS: `kallsyms_lookup_name` 不存在也可能是内核编译参数没打开相关配置，自行谷歌
