/*************************************************************************
> FileName: archive-zip.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2025年04月16日 星期三 08时34分56秒
 ************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <archive.h>
#include <sys/stat.h>
#include <archive_entry.h>


int main (int argc, char* argv[])
{
	if (argc <= 1) {
		printf("请输入要压缩的文件列表\n");
		return -1;
	}

	struct archive* a = archive_write_new ();
	if (!a) {
		printf("archive new error!\n");
		return -1;
	}

	archive_write_set_format_pax_restricted(a);
	archive_write_add_filter_gzip(a);

	if (archive_write_open_filename(a, "output.tar.gz") != ARCHIVE_OK) {
		printf ("Error open file: %s\n", archive_error_string(a));
		return -1;
	}

	char buffer[81920] = {0};
	printf ("start archive...\n");
	for (int i = 1; i < argc; ++i) {
		printf ("file: %s\n", argv[i]);
		if (0 != access(argv[i], F_OK)) {
			printf("file: %s, not exist!\n", argv[i]);
			continue;
		}

		int flen = strlen(argv[i]);
		char* fileName = NULL;

		for (int j = flen - 1; j >= 0; j--) {
			if ('/' == argv[i][j]) {
				fileName = &argv[i][j + 1];
				break;
			}
		}

		if (NULL == fileName) {
			fileName = argv[i];
		}

		if (fileName == NULL) {
			printf("file name is null!\n");
			continue;
		}

		printf("Add file: '%s'\n", fileName ? fileName : "<null>");

		struct stat st;
		if (0 != stat(argv[i], &st)) {
			printf("stat error!\n");
			continue;
		}

		struct archive_entry* entry = archive_entry_new();
		if (entry) {
			archive_entry_set_pathname(entry, fileName);
			archive_entry_set_size(entry, st.st_size);
			archive_entry_set_filetype(entry, AE_IFREG);
			archive_entry_set_perm(entry, 0666);

			int ret = archive_write_header(a, entry);
			if (ARCHIVE_OK != ret) {
				printf("write header error: %s\n", archive_error_string(a));
				archive_entry_free(entry);
				continue;
			}

			int fr = open(argv[i], O_RDONLY);
			if (fr < 0) {
				printf("open file: %s error\n", argv[i]);
				archive_entry_free(entry);
				continue;
			}

			ssize_t len = 0;
			do {
				len = read(fr, buffer, sizeof(buffer));
				if (len < 0) {
					printf("read error!\n");
					break;
				}

				if (len == 0) {
					printf("read finished!\n");
					break;
				}

				if (archive_write_data(a, buffer, len) != len) {
					printf ("archive write data error!\n");
					break;
				}
			} while (1);
			close (fr);
			archive_entry_free(entry);
		}
	}

	archive_write_close(a);
	archive_write_free(a);

	return 0;
}

