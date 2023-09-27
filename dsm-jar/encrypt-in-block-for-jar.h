#ifndef _ENCRYPT_IN_BLOCK_FOR_JAR_H_
#define _ENCRYPT_IN_BLOCK_FOR_JAR_H_
#include <glib.h>

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

guint64 encryptinblockforjr (
            const gchar* fileid,/*in*/
            guchar filetype,
            guint64 filesize,
            guchar* block, /*in/out*/
            guint64 blocklen,
            guint64 fileoffset,
            guchar* cryptkey,
            gushort cryptkeylen,
            guchar* infilehead,
            guchar* outfilehead, /* out */
            gushort* outfileheadsize, /* out */
            guchar* datakey,
            gushort* datakeylength,
            guchar* headextenddata,
            gushort headextenddatalen,
            gchar* szfilename,
            gchar* sztid,
            gchar* szuserid,
            gchar* szusername,
            guint64 szfilemaketime 
            );
#endif
