#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

int main (void)
{
	for (int i = 0; i <1000000000000000000000000; ++i) {
		key_t k = ftok("/proc/self/exe", 65);
		int id = shmget(k, 10240, IPC_CREAT | 0666);
		if (id == -1) {
			printf("shmget error:%d, %s\n", errno, strerror(errno));
		}
		else {
			printf ("1\n");
			char* d = shmat(id, NULL, 0);
			if ((char*) -1 == d) {
				printf ("shmat error:%d, %s\n", errno, strerror(errno));
			}
			printf ("2\n");
            if (shmdt(d) == -1) {
                shmctl(id, IPC_RMID, 0);
            }
		}
	}

	return 0;
}
