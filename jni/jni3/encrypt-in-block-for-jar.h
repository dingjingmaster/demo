#ifndef _ENCRYPT_IN_BLOCK_FOR_JAR_H_
#define _ENCRYPT_IN_BLOCK_FOR_JAR_H_

#include <stdint.h>
#include <string.h>
#include <stdbool.h>


#define TCA_SUCCESS                 0L           // 成功
#define TCA_UNKNOW_ERROR            1L           // 未知错误
#define TCA_INVALID_PARAM           2L           // 参数无效
#define TCA_FILE_NOT_FIND           3L           // 指定的文件路径不存在
#define TCA_FILE_NOT_OPEN           4L           // 不能打开文件
#define TCA_INVALID_HEADER          5L           // 无效的数据,文件头格式不对
#define TCA_BAD_LENGTH              6L           // 长度错误，文件头长度错误
#define TCA_NOT_SUPPORT_TYPE        7L           // 不支持的权限文件类型
#define TCA_FILE_IS_ENCRYPTED       8L           // 文件已经是加密文件
#define TCA_FILE_NOT_ENCRYPTED      9L           // 不是加密文件
#define TCA_FILE_ENCRYPT_ERROR      10L          // 加密函数内部错误
#define TCA_OUT_OF_MEMORY           11L          // 内存分配失败
#define TCA_KEY_ERROR               12L          // 密钥错误
#define TCA_BUFFER_TO_SMALL         13L          // 缓存区内容太少
#define TCA_FILE_COMPRESS_ERROR     14L          // 透明加密文件压缩失败
#define TCA_FILE_OFFSET_ERROR       15L          // 文件偏移错误
#define TCA_BLOCK_SIZE_ERROR        16L          // 分块长度错误

#define	TCA_TA_FILE				    1L           // TA文件
#define	TCA_DSIP_FILE				2L           // DSIP文件
#define	TCA_OTHER_FILE				3L           // 非权限文件

#define TCA_FILE_NONE               0x00
#define TCA_FILE_DRM                0x10
#define TCA_FILE_TP                 0x11

enum ESTCdgType{	
	TPTemplate = 1,
	NormalTemplate,
	Customize,	
};

#define TA_FILE_ID_LENGTH           32
#define TA_FILE_KEY_LENGTH          16
#define TA_FILE_DATA_KEY_LENGTH		32
#define TA_FILE_HEADER_LENGTH       1024
#define TA_BLOCK_SIZE               1024
#define TA_MAX_HEAD_EXTEND_LENGTH   30
#define TA_DSIP_FILE_HEADER_LENGTH      1580
#define DSIP_FILE_HEADER_LENGTH         4096

#define ARITH_DES	0  //DES加密算法
#define ARITH_AES	1  //AES加密算法
#define ARITH_RC4	2  //RC4加密算法--》动态加解密目前只支持RC4算法，以后可能会扩展

#define MAX_KEY_LEN        16 
/*
功能: 权限文件分块加密接口
参数:
	fileid                          :权限文件唯一标识ID, 32 个字节
    filetype                        :权限文件类型，DRM、TP文件类型（TCA_FILE_XXXX）
    filesize                        :待制作权限文件大小，如果不确定文件大小可设置为 0
    block                           :文件分块缓冲区
    blocklen                        :文件分块长度，1024 个字节的整倍数，最后一个分区为实际长度
    fileoffset                      :分块缓冲区对应的文件偏移
    cryptkey                        :权限文件加密密钥
    cryptkeylen                     :权限文档加密密钥长度，当前是16 个字节128 位
    infilehead                      :加密第一个文件块传 NULL 并返回 outFileHead，生成加密头后传入返回的 outFileHead 内容
    outfilehead                     :返回加密文件头
    outfileheadsize                 :权限文件加密文件头大小，当前文件头大小为 1024 字节（TA_FILE_HEADER_LENGTH）
    datakey                         :加密文件时产生的随机数据密钥
    datakeylength                   :加密文件时产生的随机数据密钥的长度
    headextendver                   :固定输入--1
    headextenddata                  :文件头中附加扩展字段缓存区内容，最大支持 30 个字节（TA_MAX_HEAD_EXTEND_LENGTH）
    headextenddatalen               :文件头中附加扩展字段实际长度，小于、等于 30 （TA_MAX_HEAD_EXTEND_LENGTH）
    szfilename                      :文件名（如：123.txt ，只要文件名和扩展名，不要路径）长度上限：256
    sztid                           :模板ID 长度上限：64
    szuserid                        :用户ID 长度上限：128
    szusername                      :用户名 长度上限：128
    nValidateDate                   :有效期（单位：天；1-36000天，0为永久）
    szfilemaketime                  :时间参数,选择有效期截止日期问题。文档制作时间、文档创建时间、文档修改时间、文档最后访问时间
返回值:
	TCA_SUCCESS : 成功
	其它错误...
*/
uint64_t encryptinblockforjr(const char* fileid,
                            /*in*/ unsigned char filetype,
                            uint64_t filesize,
                            unsigned char* block,
                            /*in/out*/ uint32_t blocklen,
                            uint64_t fileoffset,
                            unsigned char* cryptkey,
                            unsigned short cryptkeylen,
                            unsigned char* infilehead,
                            unsigned char* outfilehead,
                            /*out*/ unsigned short* outfileheadsize,
                            /*out*/ unsigned char* datakey,
                            unsigned short* datakeylength,
                            unsigned short headextendver,
                            unsigned char* headextenddata,
                            unsigned short headextenddatalen,
                            char* szfilename,
                            char* sztid,
                            char* szuserid,
                            char* szusername,
                            uint32_t nValidateDate,
                            const char* szfilemaketime);
/*
功能: 通过文件分块解密授权文件，分块最小单元为 1024 字节
参数:
	pBlock            文件分块缓冲区
	blockLength       文件分块长度，1024 个字节的整倍数，最后一个分区为实际长度
	fileOffset        分块缓冲区对应的文件偏移,忽略权限文件头 1024 字节, 从 0 开始计算
	fileHead          权限文件前 1024 个字节
	cryptKey          权限文件加密密钥
	cryptKeyLength    权限文档加密密钥长度，当前是16 个字节128 位
返回值:
	TCA_SUCCESS : 成功
	其它错误...
*/                                
uint64_t DecryptInBlock(
    unsigned char* pBlock,
    size_t blockLength,
    uint64_t fileOffset,
    unsigned char* fileHead,
    unsigned char* cryptKey,
    size_t cryptKeyLength
);



/*
功能: 通过缓冲区内容判断加密文件类型
参数:
	inBuffer          加密缓冲区内容
	inBufferLength    加密缓冲区长度，当前版本只要传前面 1024 个字节
	fileId            返回权限文件唯一标识ID, 32 个字节
	fileType          返回权限文件类型，DRM、TP文件类型（TCA_FILE_XXXX）
返回值:
	TCA_SUCCESS : 成功
	通过 fileType 返回值判断加密文件类型（DRM、TP）
*/
uint64_t GetEncryptTypeInMemory(
    const unsigned char* inBuffer,
    size_t inBufferLength,
    char* fileId,
    unsigned char* fileType
);
/*
功能: 通过缓冲区内容判断加密文件类型
参数:
	inBuffer          加密缓冲区内容
	inBufferLength    加密缓冲区长度，当前版本只要传前面 1024 个字节
返回值:
	0 : 失败
	其他：返回文件类型
*/
unsigned int GetEncryptTypeInMemoryEx(const unsigned char* inBuffer, size_t inBufferLength);

#endif
