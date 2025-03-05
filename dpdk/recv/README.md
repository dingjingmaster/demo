# 从网卡接收udp数据

1. 重启机器，使用 `ip addr` 或 `ifconfig -a`，获取网卡信息并保存到文件
2. 初始化DPDK运行环境，运行`usertools/dpdk-setup.py`，执行 43（插入UIO内核模块）、47（配置大页）、48（选择一个dpdk要使用的网卡，复制其设备信息）、49（把上一步复制到的设备信息粘贴到这里，绑定到dpdk）
3. sudo ./build/demo-recv 
4. 随便取一个ip，使用arp命令把ip映射到网卡`sudo arp -s 192.168.122.125 52:54:00:80:9f:41`，一定是绑定到DPDK的网卡地址
5. 使用nc命令模拟udp发送信息：`echo 'Hello,UDP' | nc -u -w1 192.168.122.125 8888`

注意：
- 如果HugePage配置出错，可以在内核启动参数中添加 `default_hugepagesz=2M hugepages=4096 transparent_hugepage=never`，确保启动时候就预分配好

