#include <stdio.h>
#include <errno.h>
#include <string.h>

int main (int argc, char* argv[])
{
	FILE* f = popen ("bash -c 'cat /home/dingjing/a_2.txt'", "r");

	int ret = 0;
	char buf[1024] = {0};

    fgets(buf, sizeof(buf) - 1, f);
	printf ("%s", buf);

	//while (0 > (ret = fread (buf, sizeof (buf) - 1, 1, f))) {
	//	memset (buf, 0, sizeof(buf));
	//}

	printf ("%s\n", strerror (errno));

	if (f) pclose (f);

	return 0;
}
