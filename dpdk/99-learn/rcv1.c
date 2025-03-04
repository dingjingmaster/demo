/*************************************************************************
> FileName: rcv1.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 03 Mar 2025 07:56:12 PM CST
 ************************************************************************/
#include <stdio.h>
#include <rte_eal.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/queue.h>

#include <rte_eal.h>
#include <rte_lcore.h>
#include <rte_debug.h>
#include <rte_memory.h>
#include <rte_launch.h>
#include <rte_per_lcore.h>


int main (int argc, char* argv[])
{
	if (rte_eal_init(argc, argv) < 0) {
		rte_exit(-1, "Error with EAL init\n");
	}


	return 0;
}
