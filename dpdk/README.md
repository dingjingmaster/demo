# DPDK

## 第一节

### 方向

1. 网络工程师：网络转发、数据转发的功能用DPDK来做
2. 存储网络工程师：
3. SDN虚拟网研发工程师：一台机器4个网口，可以提供大于4个子网（NFV）
4. 云基础开发、高性能网络工程师（网口定制）

### 学哪些

网络虚拟化框架、网口抓数据
1. 网络协议栈（arp、以太网、ip、icmp、udp、tcp）实现
2. DPDK内部组件如何使用？ acl（数据过滤）、kni（不需要的数据写回内核）、timer、bpf、mbuf/mempool
3. firewarll、switch、dns、gateway、nat

### DPDK框架

1. vpp（可扩展的网络数据扩展框架C/C++）
2. OVS（可开发性没有vpp强，可通过配置实现功能）
3. nff-go（golang实现）
4. snabblb（lua）
5. spdk（C、网络存储）

### DPDK源码

1. 支撑DPDK运行点：（igb_uio、vfio、kni）
2. 内存：mempool、mbuf
3. 协议：ipsec、bpf、pci、flow_classify
4. 虚拟化：vhost、virtio
5. cpu：亲和性、rcu、sched
6. 安全：srcurity（安全）、cryptodev（解解密）、compressed（压缩）

### DPDK性能

1. 性能指标：吞吐量（bps）、拆链和建链（pps，短链接）、并发（同时承载链接数量）、时延（最大、最小、平均）、丢包率
2. 测试方法：测试用例、vpp sandbox、perf3灌包、rfc2544
3. 测试工具：perf3、trex、testpmd、pktgen-dpdk

## 第二节

### Linux内核是否支持

- `CONFIG_HUGETLBFS`启用巨页文件系统、`CONFIG_HUGETLB_PAGE`启用巨页支持
- `CONFIG_NUMA`启用非统一内存访问（NUMA）支持。对于多核CPU系统，可以帮助优化内存访问
- `CONFIG_IXGBE`支持intel 82599及更高版本的网卡；`CONFIG_R8169`支持Realtek网卡；`CONFIG_MLX5_CORE`支持Mellanox网卡
- `CONFIG_FHANDLE`：启用文件句柄支持；`CONFIG_PROC_FS`；`CONFIG_DEVPTS_FS`启用devpts文件系统
- `CONFIG_NO_HZ`：启用高精度定时器，减少内核干扰；`CONFIG_PREEMPT_RT`：如果需要实时性能，考虑启用实时调度

### 环境搭建

- ubuntu 16.04 server
- dpdk-19.08

用户名：dpdk
密码：1

安装：源码目录下脚本：usertools/dpdk-setup.sh
1. 设置两个环境变量：`RTE_SDK=/源码根目录/` 和 `RTE_TARGET=x86_64-native-linux-gcc`、执行 `usertools/dpdk-setup.sh`
2. 执行`usertools/dpdk-setup.sh`选择 `43` 插入`IGB_UIO`模块（选择网卡为vmxnet3）、选择44插入VFIO模块（选择网卡为e1000）
3. 选择49绑定`igb_uio`模块：`usertools/dpdk-devbind.py --bind=igb_uio eth0`
4. 选择`53`运行testpmd

成功编译安装dpdk之后，开启大页:
1. 设置启动参数：
```
# 默认大页为1GB，共分配2个（共2GB）
# 额外分配1024个2MB的大页（共2GB）
# 禁用透明大页（THP）
default_hugepagesz=1G hugepages=2 hugepagesz=2M hugepages=1024 transparent_hugepage=never isolcpus=0-2
```
2. 自动挂载大页
```
nodev	/mnt/huge_2M	hugetlbfs	pagesize=2MB	0	0
```

|grub字段|说明|
|:-------|:---|
|`default_hugepagesz`|设置默认的HugePage大小，可能值是2M或1G，需要内核支持|
|`hugepagesz`|指定特定的大页大小，需要和hugepages配对使用，可选值为2M、1G，`hugepagesz=1G hugepages=4`分配4个1GB大页（共4GB）|
|`hugepages`|分配指定数量的HugePages，尺寸由最近的hugepagesz或default_hugepagesz决定，示例：hugepages=1024(默认2M)分配1024个2MB页(共2GB)；`hugepagesz=1G hugepages=2`分配2个1GB页|
|`transparent_hugepage`|控制透明大页(THP)的行为，THP是内核动态管理的，可能和静态HugePages冲突。可选值：`always`启用THP、`madvise`仅对明确要求的内存启用、`never`禁用THP|

#### HugePage

```shell
# cat /proc/meminfo
HugePages_Total:       0 			// 预留HugePage的总个数
HugePages_Free:        0 			// 池中尚未分配的HugePage个数，真正空闲页数等于 HugePages_Free - HugePages_Rsvd
HugePages_Rsvd:		   0			// 表示池中已经被应用程序分配但尚未使用的HugePages数量
HugePages_Surp: 	   0 			// 这个值的意思是当开始配置了20个大页，现在修改配置为16，那么这个参数就会显示为4，一般不修改配置，这个值都是0
HugePagesize: 		   0 			// 每个大页的大小
```

是一种内存管理机制，可以通过将内存页面的大小增加到常规大小的多倍（通常为2MB或1GB）来提高性能。这种机制可以减少页表的数量，提高内存访问的效率，特别是在需要大量内存的应用程序中。

用途：
1. 减少页表开销：巨页减少了需要管理的页表数量，从而降低了内存管理的开销
2. 提高了TLB命中率：由于巨页大小更大，使用巨页可以提高TLB的命中率，减少虚拟地址到物理地址转换的时间
3. 优化大内存应用：对于需要大量内存的应用（如：数据库、内存缓存等），使用巨页可以显著提高性能。

DPDK中的用途：
1. 内存性能：DPDK常在高速网络应用中使用巨页，以减少内存访问延迟和提高整体性能
2. 减少内存分配的复杂性：DPDK使用巨页来分配大块内存，简化内存管理，减少频繁的内存分配和释放
3. 优化数据包处理：在数据包处理中，巨页可以减少数据包缓存区的页表转换，从而提高数据包处理的速率
4. 提高NUMA（非统一内存访问）性能：在多核处理器系统中，巨页可以帮助更好的利用NUMA架构，提高内存访问的局部性

#### 多队列网卡

多队列网卡，能够通过多个接收队列和多个发送队列来处理网络流量。这种设计允许网卡同时处理多个数据流，提高了数据包的处理效率和网络吞吐量。

网卡接受到数据触发CPU中断。

qemu中virtio-net是常用的虚拟网卡，支持多队列；e1000、rtl8139传统网卡不支持多队列

## recv实现过程

两个端口：一个接收数据、另一个发送数据。使用前需要`DOWN`掉网口

1. 环境检测（大页）
2. 初始化内存池
3. 检测端口是否可用
