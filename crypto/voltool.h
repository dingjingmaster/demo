#ifndef SRC_VOLTOOL_H_
#define SRC_VOLTOOL_H_

#ifdef __cplusplus
extern "C"{
#endif

int active_volume(const char *file, const char *passphase,const char *mntpath,char *devmap);

void close_volume(const char *device_name);

int create_volume_path(const char *outfile, const char *passphase, const char *inpath,unsigned long volsize);

#ifdef __cplusplus
}
#endif



#endif /* SRC_VOLTOOL_H_ */
