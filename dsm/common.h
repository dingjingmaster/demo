/*************************************************************************
> FileName: common.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 27 Mar 2023 10:10:12 AM CST
 ************************************************************************/
#ifndef _COMMON_H
#define _COMMON_H
#include <string>

#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif

enum CDG_RETVAL
{
	CDG_RET_SUCCESS			        =	0,
	CDG_RET_INVALID_FILETYPE         =	1,
	CDG_RET_OPEN_BYOTHERPROCESS		=	2,
	CDG_RET_DOC_READONLY            =   3,
	CDG_RET_COPY_FAILED             =   4,
	CDG_RET_MAKE_FAILED             =   5,
	CDG_RET_OPEN_INIT_FAILED        =   6,
	CDG_RET_OPEN_MUTEX_EXIST        =   7,
	CDG_RET_USER_NOT_LOGIN          =   8,
	CDG_RET_OPEN_NO_PERMISSION      =   9,
	CDG_RET_FILENAME_EMPTY          =   10,
	CDG_RET_OPEN_FILE_FAILED        =   11,
	CDG_RET_OPEN_NO_APPFILE         =   12,
	CDG_RET_OPEN_WITH_64EXE         =   13,
	CDG_RET_OPEN_CREATEPROCESS_FAILED = 14,
	CDG_RET_OPEN_EXCEED_MAX_FILENUM   = 15,
	CDG_RET_SUFFIX_NOT_DSM            = 16,
	CDG_RET_FILE_HEAER_FAILED         = 17,
	CDG_RET_FILE_HEAER_ID_ERR         = 18,
	CDG_RET_OCCUPY_FAILED             = 19,
	CDG_RET_NO_CDGKEY                 = 20,
	CDG_RET_DECRYPT_FAILED            = 21,
	CDG_RET_SEND_RPC_FAILED           = 22,
	CDG_RET_INVALID_TEMPLATEID        = 23,
	CDG_RET_OPEN_BEFORE_START_TIME    = 24,
	CDG_RET_OPEN_AFTER_END_TIME       = 25,
	CDG_RET_OPEN_WRONG_TIME           = 26,
};


//#define CDG_FILEHEAD_ID            	0x12345678
//#define CDG_FILEHEAD_ID_NEW			0x12345688

#define CDG_FILEHEAD_ID_DSM			0x12345668
#define CDG_FILEHEAD_PASS           "IOSDFKKLJJKDSDDS"

#define MAX_USER_NAME_LEN          	128

#define TRUE 1
#define FALSE 0

typedef unsigned int   DWORD;
typedef unsigned int   BOOL;
typedef unsigned long  ULONG;
typedef unsigned char  UCHAR;
typedef void* HANDLE;


typedef struct _CDG_FILE_HEADER
{
	DWORD dwmyid;        	// 标识：CDG_FILEHEAD_ID_DSM  ( DWORD是4个字节 )
	DWORD dwversion;        // 版本号
	DWORD dwentype;         // 头加密算法 0: AES 1: des 2: rc4

	//加密内容
	char  szFileId[64];		// 文件ID
	char  szFileName[256];	// 文件名
	char  szpassword[20];   // 部分文件加密密钥

	DWORD dwenleng;         // 实际文件的加密长度 0 - 200
	DWORD dwfileleng;       // 文件长度
	char  szTemplateId[64]; // 模板ID

	//模板类型：TP模板，普通模板，非模板自由授权，放入文件头中用于确定打开文件时该从哪获取权限信息
	DWORD dwTemplateType;
	char  szUserName[MAX_USER_NAME_LEN];
	char  szUserId[  MAX_USER_NAME_LEN];
	char  szSecInfo[ MAX_USER_NAME_LEN];


	//增加DSM文件的制作时间、创建时间、修改时间、有效期（天） [7/21/2022 fuzhanhui]
	char szFileMakeTime[50];       //文档制作时间
	char szFileCreateTime[50];     //文档创建时间
	char szFileUpdateTime[50];     //文档修改时间
	char szFileAccessTime[50];     //文档最后访问时间
	DWORD dwValidateDate;          //DSM文件有效期（天）

	//注意：保留字段要严格控制大小！
	char  szabout[564];//保留字段
	//char  szabout[896];//保留字段
	//char  szabout[1024];//保留字段

} CDG_FILE_HEADER;


bool EncryptLockFile(char* pFileName, unsigned char* key, int arith);
void DecodeAES(char* pBuffer, ULONG nLength, unsigned char* Key);
void EncodeAES(char* pBuffer, ULONG nLength, unsigned char* Key);
bool IsEncryptLockFile(const char* pFileName);
#ifdef __cplusplus
}
#endif

//BOOL IsCdgFile(  char *filename, CDG_FILE_HEADER *pCdgHead );


#endif
