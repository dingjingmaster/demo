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
