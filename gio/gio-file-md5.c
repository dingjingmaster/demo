/*************************************************************************
> FileName: gio-file-md5.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Thu Jan  9 14:23:40 2025
 ************************************************************************/
#include <stdio.h>
#include <gio/gio.h>


int main (int argc, char* argv[])
{
	if (argc < 2) {
		printf ("请输入文件绝对路径\n");
		return -1;
	}

	int bufLen = 0;
	char buf[1024] = {0};
	FILE* fr = fopen (argv[1], "r");
	if (fr) {
		GChecksum* cs = g_checksum_new (G_CHECKSUM_MD5);
		if (cs) {
			while ((bufLen = fread (buf, 1, sizeof(buf), fr)) > 0) {
				g_checksum_update (cs, buf, bufLen);
			}

			printf ("md5: %s\n", g_checksum_get_string(cs));

			g_checksum_free (cs);
		}
		fclose(fr);
	}


	return 0;
}
