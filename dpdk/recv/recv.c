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
#include <arpa/inet.h>
#include <rte_errno.h>
#include <sys/queue.h>
#include <rte_lcore.h>
#include <rte_debug.h>
#include <rte_memory.h>
#include <rte_ethdev.h>
#include <rte_launch.h>
#include <rte_per_lcore.h>


#define NUM_MBUFS 	4096
#define BURST_SIZE	1024

int gDpdkPortId = 0;

static const struct rte_eth_conf gsPortConfDefault = {
	.rxmode = {
		.max_rx_pkt_len = RTE_ETHER_MAX_LEN
	}
};

int main (int argc, char* argv[])
{
	// 环境检测
	if (rte_eal_init(argc, argv) < 0) {
		printf("Error with EAL init\n");
		return -1;
	}

	// socket id
	int socketId = rte_socket_id();
	if (socketId < 0) {
		printf ("rte_socket_id %d, %s\n", rte_errno, strerror(rte_errno));
		return -1;
	}

	// 初始化内存池
	struct rte_mempool* mbufPool = rte_pktmbuf_pool_create("mbuf-pool", NUM_MBUFS, 128, 0, RTE_MBUF_DEFAULT_BUF_SIZE, socketId);
	if (NULL == mbufPool) {
		printf ("error code %d, %s\n", rte_errno, strerror(rte_errno));
		return -1;
	}

	// 初始化端口
	{
		uint16_t nbSysPorts = rte_eth_dev_count_avail();
		if (0 == nbSysPorts) {
			printf("No supported eth found\n");
			return -1;
		}

		struct rte_eth_dev_info devInfo;
		rte_eth_dev_info_get(gDpdkPortId, &devInfo);

		const int numRxQueues = 1;
		const int numTxQueues = 0;
		struct rte_eth_conf portConf = gsPortConfDefault;
		rte_eth_dev_configure(gDpdkPortId, numRxQueues, numTxQueues, &portConf);

		if (rte_eth_rx_queue_setup(gDpdkPortId, 0, 128, rte_eth_dev_socket_id(gDpdkPortId), NULL, mbufPool) < 0) {
			printf("Could not setup RX queue\n");
			return -1;
		}

		if (rte_eth_dev_start(gDpdkPortId) < 0) {
			printf("Could not start\n");
			return -1;
		}
	}

	while (1) {
		struct rte_mbuf* mbufs[BURST_SIZE] = {0};
		unsigned numRecv = rte_eth_rx_burst (gDpdkPortId, 0, mbufs, BURST_SIZE);
		if (numRecv > BURST_SIZE) {
			printf ("Error receiving from eth\n");
			return -1;
		}

		unsigned int i = 0;
		for (i = 0; i < numRecv; i++) {
			struct rte_ether_hdr* ehdr = rte_pktmbuf_mtod(mbufs[i], struct rte_ether_hdr*);
			if (rte_cpu_to_be_16(RTE_ETHER_TYPE_IPV4) != ehdr->ether_type) {
				continue;
			}

			struct rte_ipv4_hdr* iphdr = rte_pktmbuf_mtod_offset(mbufs[i], struct rte_ipv4_hdr*, sizeof(struct rte_ether_hdr));
			if (IPPROTO_UDP == iphdr->next_proto_id) {
				struct rte_udp_hdr* udphdr = (struct rte_udp_hdr*) (iphdr + 1);
				uint16_t length = ntohs(udphdr->dgram_len);
				*((char*) udphdr + length) = '\0';

				struct in_addr addr;
				addr.s_addr = iphdr->src_addr;
				printf ("src: %s:%d, ", inet_ntoa(addr), udphdr->src_port);

				addr.s_addr = iphdr->dst_addr;
				printf ("dst: %s:%d, %s\n", inet_ntoa(addr), udphdr->src_port, (char*)(udphdr + 1));
				rte_pktmbuf_free(mbufs[i]);
			}
		}
	}

	return 0;
}
