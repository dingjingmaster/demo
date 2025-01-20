#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include <unistd.h>

static int test(int argc, char **argv)
{
	char *dirc, *basec, *bname, *dname;
	const char *path = "/usr/lib";

	dirc = strdup(path);
	basec = strdup(path);
	dname = dirname(dirc);
	bname = basename(basec);
	printf("dirname=%s, basename=%s\n", dname, bname);
	free(dirc);
	free(basec);

	return 0;
}

#define BUFFER_SIZE (128 * 1024)
int copy_file(const char* source,const char* target){
	char buffer[BUFFER_SIZE];
	FILE *fin,*fout;
	int rszie;
	int result = 0;
	if ((fin=fopen(source,"r")) ==NULL)  {
		printf("Open source file failed %s\n",source);
		return -1;
	}
	if ((fout=fopen(target,"w")) ==NULL)  {
		printf("Open target file failed %s\n",target);
		fclose(fin);
		return -1;
	}
	while( (rszie = fread (buffer,1,BUFFER_SIZE,fin)) > 0){
		if(fwrite(buffer,1,rszie,fout) != rszie){
			break;
		}
	}
	if(ferror(fin) || ferror(fout)){
		perror("Error  copy  single file\n");
		result = -1;
	}
   // printf("chmod %s\n",target);
	fclose(fin);
	fclose(fout);
	return result;

}

int copy_dir(const char *src_dir, char *dest_dir )
{
	int r = 0;
	DIR *src_dp;
	struct dirent *file_info;
	char dest_file[4096];
	char src_file[4096];

	if(access(dest_dir,0)){
		if(mkdir(dest_dir, 0777)){
			perror("Create dir failed\n");
			return -1;
		}
	}
    chmod(dest_dir, 0777);

	src_dp = opendir( src_dir );
	if( src_dp == NULL )
	{
		printf("Open source dir failed %s\n",src_dir);
		return -1;
	}

	while( (file_info = readdir( src_dp )) )
	{
		switch( file_info->d_type )
		{
			case DT_REG:
				sprintf( dest_file, "%s/%s", dest_dir, file_info->d_name );
				sprintf( src_file, "%s/%s", src_dir, file_info->d_name );
				if((r = copy_file( src_file, dest_file)))goto out;
				chmod(dest_file,0777);
				break;
			case DT_DIR:
				if( strcmp( file_info->d_name, ".") == 0 ||
						strcmp(file_info->d_name, "..")  == 0 )break;
				sprintf( dest_file, "%s/%s", dest_dir, file_info->d_name );
				sprintf( src_file, "%s/%s", src_dir, file_info->d_name );
				if((r = copy_dir( src_file, dest_file)))goto out;
				break;
//			case DT_BLK:
//			case DT_CHR:
//			case DT_FIFO:
//			case DT_LNK:
//			case DT_SOCK:
//			case DT_UNKNOWN:
			default:
				break;
		}

	}
out:
	closedir( src_dp );
	return r;
}

//int copy_to_dir( char *src_dir_path, char *dest_dir_path )
//{
//	DIR *dest_dp;
//	dest_dp = opendir( dest_dir_path );
//	if( dest_dp == NULL )
//	{
//		if( mkdir( dest_dir_path, 0777 ) == -1 )
//		{
//			perror( "mkdir error!\n" );
//			return -1;
//		}
//	}
//	else
//	{
//		closedir( dest_dp );
//	}
//	return _copy_dir( src_dir_path, dest_dir_path );
//}
//int main(int argc, char *argv[])
//{
//	if(argc != 3)
//	{
//		printf("argc error!\n");
//		return -1;
//	}
//
//	copy_dir(argv[1], argv[2]);
//
//	return 0;
//}


static int file_size(const char* filename,unsigned long *psize)
{
	//printf("File path %s\n",filename);
    FILE *fp=fopen(filename,"r");
    if(!fp) {
    	printf("Open file failed %s\n",filename);
    	return -1;
    }
    fseek(fp,0L,SEEK_END);
    long size=ftell(fp);
    fclose(fp);
    *psize = size;
    return 0;
}
/*
 *display_dir:递归遍历目录，打印所有文件路径名
 *@dir_path：目录路径
 *
 */
int dir_size(const char *dir_path,unsigned long *psize)
{
	int r = 0;
	DIR *dir_ptr;//目录指针
	char sub_path[4096] = {0};//临时存放子目录名称
	struct dirent *file_info;//目录项指针，存放目录项的相关信息比如文件类型
	unsigned long total_size = 0;
	unsigned long tmp_size;
	dir_ptr = opendir( dir_path );//打开目录
	if( dir_ptr == NULL )
	{
		printf("Open dir failed for %s \n",dir_path);
		return -1;
	}

	while( (file_info = readdir( dir_ptr )) )//每次循环读取一个目录项(一个文件)
	{

		switch( file_info->d_type )//根据文件类型执行相应动作，
		{
			case DT_REG: //常规文件 .txt .bmp .jpg .c 可执行文件 等等
					sprintf( sub_path, "%s/%s", dir_path, file_info->d_name );
					if(file_size(sub_path,&tmp_size)){
						printf("Calulate file size error for %s\n",sub_path);
						r = -1;
						break;
					}
					total_size += tmp_size;
					break;
			case DT_DIR: //如果是目录就递归遍历
					if( strcmp( file_info->d_name, ".") == 0 ||
							strcmp(file_info->d_name, "..")  == 0 )break;
					sprintf( sub_path, "%s/%s", dir_path, file_info->d_name );
					if(dir_size(sub_path,&tmp_size)){
						printf("Calulate file size error for %s\n",sub_path);
						r = -1;
						break;
					}
					total_size += tmp_size;
					break;
//			case DT_BLK:
//			case DT_CHR:
//			case DT_FIFO:
//			case DT_LNK:
//			case DT_SOCK:
//			case DT_UNKNOWN:
			default:
				break;
		}

	}
	if(r == 0)*psize = total_size;
	closedir( dir_ptr );//关闭目录
	return r;
}

//int main(int argc, char *argv[])
//{
//	//main函数参数在执行时传进来，比如./hello jack  argc=2,argc[0]=./hello,argv[1]=jack
//	if(argc != 2)
//	{
//		printf("argc error!\n");
//		return -1;
//	}
//
//	display_dir( argv[1] );//调用目录打印函数
//
//	return 0;
//}
