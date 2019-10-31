#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>


static char* user_num_tmp = "/tmp/user_num";

int main(int argc, char* argv[]) {
    char buf[32] = {0};

    int fd = open(user_num_tmp, O_RDWR | O_CREAT);
    if (fd < 0) {
        puts("error1");
        return -1;
    }
    system("who | awk '{print $1}' | sort | uniq | wc -l > /tmp/user_num");
    int iret = flock(fd, LOCK_EX | LOCK_NB);
    if (iret < 0) {
        puts("error2");
        return -1;
    }

    if (read(fd, buf, sizeof buf) < 0) {
        puts("error3");
        return -1;
    }

    iret = flock(fd, LOCK_UN);
    if (iret < 0) {
        puts("error2");
        return -1;
    }
    remove(user_num_tmp);

    printf ("%s", buf);

    return 0;
}
