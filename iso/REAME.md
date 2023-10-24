# 创建镜像

## 工具
- mkisofs       <制作ISO>
- genisoimage   <制作ISO>
- mksquashfs

## genisoimage

```shell
# -b                指定了引导加载程序的位置
# -c                指定了引导目录的位置
# -no-emul-boot     表示不使用光盘的软盘仿真方式引导
# -boot-load-size   指定了引导加载程序的大小
# -boot-info-table  用于添加引导信息表
genisoimage -o <xxx.iso> -b isolinux/isolinux.bin -c isolinux/boot.cat -no-emul-boot -boot-load-size 4 -boot-info-table <文件系统路径>
```

## mkisofs

```shell
mkisofs -o <xxx.iso> [选项] [文件夹]

mkisofs -o <xxx.iso> -b isolinux/isolinux.bin -c isolinux/boot.cat -no-emul-boot -boot-load-size 4 -boot-info-table <文件系统路径>
```

- `-o` 指定生成的ISO的输出路径和名字
- `[option]`参数:
    - `-b boot_image`: 指定引导加载程序的位置和文件名
    - `-no-emul-boot`: 禁用光盘的软盘仿真方式引导
    - `-boot-load-size load_size`: 指定引导加载程序的加载大小
    - `-r`: 以 Rock Ridge 扩展格式创建IOS文件，以支持更长的文件名和更多的文件属性
    - `-J`: 以Joliet扩展格式创建ISO文件，以支持 Windows 系统的文件名和属性
    - `-l`: 生成包含连接的ISO文件
    - `-V volume-id`: 指定 ISO 文件的卷标
