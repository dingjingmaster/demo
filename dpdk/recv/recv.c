/*************************************************************************
> FileName: recv.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 03 Mar 2025 07:56:12 PM CST
 ************************************************************************/
#include <stdio.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <rte_eal.h>
#include <rte_mbuf.h>
#include <rte_errno.h>
#include <sys/queue.h>
#include <rte_lcore.h>
#include <rte_debug.h>
#include <rte_memory.h>
#include <rte_ethdev.h>
#include <rte_launch.h>
#include <rte_per_lcore.h>


#define NUM_MBUFS 	4096
#define BURST_SIZE	32


int main (int argc, char* argv[])
{
	// 环境检测
	if (rte_eal_init(argc, argv) < 0) {
		rte_exit(-1, "Error with EAL init\n");
	}

	int socketId = rte_socket_id();
	if (socketId < 0) {
		printf ("rte_socket_id %d, %s\n", rte_errno, strerror(rte_errno));
		return -1;
	}

	// 初始化内存池
	struct rte_mempool* mbufPool = rte_pktmbuf_pool_create("mbuf-pool", NUM_MBUFS, 128, 0, RTE_MBUF_DEFAULT_BUF_SIZE, socketId);
	if (NULL == mbufPool) {
		printf ("error code %d, %s\n", rte_errno, strerror(rte_errno));
		rte_exit(-1, "Could not create mbuf pool\n");
	}

	//


	return 0;
}
