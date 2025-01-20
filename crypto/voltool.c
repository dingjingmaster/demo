#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/types.h>
#include <libcryptsetup.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libgen.h>
#include <sys/syscall.h>
#include <uuid/uuid.h>
#include "sm3.h"
#ifndef SYS_gettid
#error "SYS_gettid unavailable on this system"
#endif
#define GETTID() ((pid_t)syscall(SYS_gettid))


static int generate_zero(const char *name,unsigned long size)
{
    FILE *fp;
    int r = 0;
    do{
		fp = fopen(name,"wb+");
		if(!fp){
			r = -1;
			printf("Open file %s for write failed\n",name);
			break;
		}

		r = fseek(fp, size-1, SEEK_SET);
		if(r){
			printf("Write file %s failed\n",name);
			break;
		}
		if( fputc('\0',fp) == EOF){
			r = -1;
			printf("Write file %s failed\n",name);
			break;
		}
		fclose(fp);
    }while(0);
    return r;
}


static int mount_vol(const char *device_name,const char *mnt){
	char spath[256];
	int r;
	sprintf(spath,"/dev/mapper/%s",device_name);
	if((r = mkdir(mnt,0777))){
		printf("Create dir %s failed\n",mnt);
	}
    chmod(mnt,0777);
	char cmd[512];
	sprintf(cmd,"mount -t ext3 %s %s",spath,mnt);
	return system(cmd);
	return r;
}

static int umount_vol(){
//	char spath[256];
	char tpath[256];
	int r;
	sprintf(tpath, "/tmp_box%d",GETTID());
	if((r = umount(tpath))){
		printf("umount failed\n");
		return r;
	}
	remove(tpath);
	return 0;
}

static int umount_mapper(const char *dv){
	char tpath[256];
	sprintf(tpath,"umount /dev/mapper/%s",dv);
	printf("Try to umount %s\n",tpath);
	system(tpath);
	return 0;
}

static int makefs(const char *device_name){
	const char* param[2];
	char path[256];
	sprintf(path,"/dev/mapper/%s",device_name);
	param[0] =   "mkfs.ext3";
	param[1] = path;
	char cmd[256];
	sprintf(cmd,"mkfs.ext3 %s",path);
	return system(cmd);
	//return  mkfsfat(2,(char**)param);

}



#define MAX_KEY_LEN 32
static int init_and_active(char *uuid,const char *path,const char *passphrase)
{
	struct crypt_device *cd;
	int r;
	struct crypt_params_plain pl_params = {
		.hash = "sha256",
		.offset = 0,
		.skip = 0,
		.size = 0,
	};


	r = crypt_init(&cd, path);
	if (r < 0) {
		printf("crypt_init() failed for %s.\n", path);
		return r;
	}

	printf("Context is attached to block device %s.\n", crypt_get_device_name(cd));

	r = crypt_format(cd,		/* crypt context */
			CRYPT_PLAIN,	/* LUKS2 is a new LUKS format; use CRYPT_LUKS1 for LUKS1 */
			 "aes",		/* used cipher */
			 "xts-plain64",	/* used block mode and IV */
			 NULL,		/* generate UUID */
			 NULL,		/* generate volume key from RNG */
			 512 / 8,	/* 512bit key - here AES-256 in XTS mode, size is in bytes */
			 &pl_params);		/* default parameters */

	if (r < 0) {
		printf("crypt_format() failed on device %s\n", crypt_get_device_name(cd));
		crypt_free(cd);
		return r;
	}


	unsigned char twoKeys[MAX_KEY_LEN*2];
    sm3_hash((unsigned char *)passphrase,strlen(passphrase), twoKeys);
	sm3_hash(twoKeys,MAX_KEY_LEN, twoKeys+MAX_KEY_LEN);

	r = crypt_activate_by_volume_key(cd,
		uuid,
		(char *)twoKeys,
		MAX_KEY_LEN + MAX_KEY_LEN,
		0);

	if (r < 0) {
		printf("Adding keyslot failed.\n");
		crypt_free(cd);
		return r;
	}

	printf("\tdevice actived: %s\n",uuid);

	crypt_free(cd);
	return 0;
}



static int deactivate_device(const char *device_name)
{
	struct crypt_device *cd;
	int r;
	int loop =0;

	/*
	 * crypt_init_by_name() initializes context by an active device-mapper name
	 */
	r = crypt_init_by_name(&cd, device_name);
	if (r < 0) {
		printf("crypt_init_by_name() failed for %s.\n", device_name);
		return r;
	}

	//crypt_status(cd, device_name)

	while(1){

		if (crypt_status(cd, device_name) == CRYPT_ACTIVE){
			printf("Device %s is active.\n", device_name);
			break;
		}
		else {
			printf("Something failed perhaps, device %s is not active. status %d\n", device_name,crypt_status(cd, device_name));
			sleep(1);
			if(loop++ > 5)umount_mapper(device_name);
		}
	}

	/*
	 * crypt_deactivate() is used to deactivate a device
	 */
	r = crypt_deactivate(cd, device_name);
	if (r < 0) {
		printf("crypt_deactivate() failed.\n");
		crypt_free(cd);
		return r;
	}

	printf("Device %s is now deactivated.\n", device_name);

	crypt_free(cd);
	return 0;
}



int copy_dir(const char *src_dir, char *dest_dir );
int dir_size(const char *dir_path,unsigned long *psize);
int create_volume_path(const char *outfile, const char *passphase, const char *inpath,unsigned long volsize){

	//int r;
	//char uuid[256];
	char tmp_uuid[40];
	char tpath[256];
	uuid_t tmp_uuid_bin;
	char *loop_device = NULL;
	int readonly = 0;
	//int loop_fd;
	int r;
	if (geteuid()) {
		printf("Requires super user privileges.\n");
		return -1;
	}
	if((r = generate_zero(outfile , volsize)))return r;

//	loop_fd = crypt_loop_attach(&loop_device, outfile, 0, 1, &readonly, 512);
//	if(loop_fd < 0){
//		printf("Attach loop failed.\n");
//		return -1;
//	}
	uuid_generate(tmp_uuid_bin);
	uuid_unparse(tmp_uuid_bin, tmp_uuid);

	if ( (r = init_and_active(tmp_uuid,outfile,passphase)))
		goto end;
	//if (activate_and_check_status(outfile, tmp_uuid,passphase))	return -1;
	if( (r = makefs(tmp_uuid)))goto out_deactive;
	sprintf(tpath,"/tmp/box_%d",GETTID());
	if( (r = mount_vol(tmp_uuid,tpath)))goto out_deactive;

	if( ( r = copy_dir(inpath,tpath))){
		printf("Copy dir failed.\n");
	}
	umount_vol();
out_deactive:
//	close(loop_fd);
	deactivate_device(tmp_uuid);
	return r;
end:
//	close(loop_fd);
	return r;
}


int active_volume(const char *file, const char *passphase,const char *mntpath,char *devmap){

	//int r;
	//char uuid[256];
	char tmp_uuid[40];
	//char tpath[256];
	uuid_t tmp_uuid_bin;
//	char *loop_device = NULL;
	int readonly = 0;
//	int i;
//	int loop_fd;
	int r;
	if (geteuid()) {
		printf("Requires super user privileges.\n");
		return -1;
	}
//	loop_fd = crypt_loop_attach(&loop_device, file, offset, 1, &readonly, 512);
//	if(loop_fd < 0){
//		printf("Attach loop failed.\n");
//		return -1;
//	}

	uuid_generate(tmp_uuid_bin);
	uuid_unparse(tmp_uuid_bin, tmp_uuid);

	if ( (r = init_and_active(tmp_uuid,file,passphase)))
		goto end;
	//if (activate_and_check_status(outfile, tmp_uuid,passphase))	return -1;
	//if( r = makefs(tmp_uuid))goto out_deactive;

//	sprintf(tpath,BOX_DATA_PREFIX"/%d",GETTID());
	if( (r = mount_vol(tmp_uuid,mntpath)))goto out_deactive;
	printf("Box open successfully, mount at %s\n"
			"You can use './boxclose %s' to close it !\n" ,
			mntpath,tmp_uuid);
//	if(mntpath)strcpy(mntpath,tpath);
	if(devmap)strcpy(devmap,tmp_uuid);
end:
//	close(loop_fd);
	return r;
out_deactive:
//	close(loop_fd);
	deactivate_device(tmp_uuid);
	return r;

}


void close_volume(const char *device_name){
	umount_mapper(device_name);
	deactivate_device(device_name);
}
