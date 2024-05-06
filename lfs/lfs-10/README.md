# LFS-10

## 编译

### 跨平台工具链
1. binutils -- tools
2. gcc -- tools
```
If you ever happen to want to link against installed libraries
in a given directory, LIBDIR, you must either use libtool, and
specify the full pathname of the library, or use the `-LLIBDIR'
flag during linking and do at least one of the following:
   - add LIBDIR to the `LD_LIBRARY_PATH' environment variable
     during execution
   - add LIBDIR to the `LD_RUN_PATH' environment variable
     during linking
   - use the `-Wl,-rpath -Wl,LIBDIR' linker flag
   - have your system administrator add LIBDIR to `/etc/ld.so.conf'
```
3. linux -- fs
```
/usr/include/asm/*.h            The Linux API ASM Headers
/usr/include/asm-generic/*.h    The Linux API ASM Generic Headers
/usr/include/drm/*.h            The Linux API DRM Headers
/usr/include/linux/*.h          The Linux API Linux Headers
/usr/include/misc/*.h           The Linux API Miscellaneous Headers
/usr/include/mtd/*.h            The Linux API MTD Headers
/usr/include/rdma/*.h           The Linux API RDMA Headers
/usr/include/scsi/*.h           The Linux API SCSI Headers
/usr/include/sound/*.h          The Linux API Sound Headers
/usr/include/video/*.h          The Linux API Video Headers
/usr/include/xen/*.h            The Linux API Xen Headers
```
4. glibc -- fs
5. 从gcc中安装libstdc++ -- fs
6. m4
