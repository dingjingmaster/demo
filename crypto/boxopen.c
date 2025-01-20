#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "voltool.h"


int main(int argc ,char **argv){
	char devmap[512];

	if(argc !=4){
		printf("Usage: boxopen file passwd mntpath\n");
		return -1;
	}
	return active_volume(argv[1],argv[2],argv[3],devmap);
}

