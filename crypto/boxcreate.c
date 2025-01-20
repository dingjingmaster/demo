#include <stdio.h>
#include "voltool.h"
int main(int argc, char **argv)
{
	//int r;

  	if (argc != 4) {
		printf("usage: boxcreate outfile passphase path \n");
		return 3;
	}
	return create_volume_path(argv[1],argv[2],argv[3],1024 * 1024 * 100);

	return 0;
}
