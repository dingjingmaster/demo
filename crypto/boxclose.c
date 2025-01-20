#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "voltool.h"

int main(int argc ,char **argv){
	if (geteuid()) {
		printf("Requires super user privileges.\n");
		return -1;
	}

	if(argc !=2){
		printf("Usage: boxclose dev \n");
		return -1;
	}

	close_volume(argv[1]);

	return 0;
}
