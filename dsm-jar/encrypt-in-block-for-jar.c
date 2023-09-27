#include "encrypt-in-block-for-jar.h"


#define CDG_TPL_MAKE_KEY_LEN                (16)
#define MAX_USER_NAME_LEN                   (128)
#define MIN_USER_PWD_LEN                    (0)
#define MAX_USER_PWD_LEN                    (256)
#define CDG_FILE_HEAD_ID_DSM                (0X12345668)  // 新CDG文件头
#define CDG_FILE_HEAD_PASS                  "IOSDFKKLJJKDSDDS"


typedef struct _CdgFileHeader               CdgFileHeader;


struct _CdgFileHeader
{
    unsigned int                myID;                       // 标识 CDG_FILEHEAD_ID CDG_FILEHEAD_ID_NEW
    unsigned int                version;                    // 版本号
    unsigned int                encType;                    // 头加密算法 0:AES 1:des 2:rc4

    // 加密内容
    char                        szFileId[64];               // 文件ID
    char                        szFileName[256];            // 文件名
    char                        szPassword[20];             // 部分文件加密密钥

    unsigned int                dwenleng;                   // 实际文件的加密长度 0 - 200
    unsigned int                dwfileleng;                 // 文件长度

    char                        szTemplateId[64];           // 模板ID
    unsigned int                dwTemplateType;

    // 模板类型：TP模板，普通模板，非模板自由授权，放入文件头中用于确定打开文件时该从哪获取权限信息
    char                        szUserName[MAX_USER_NAME_LEN];
    char                        szUserId[MAX_USER_NAME_LEN];
    char                        szSecInfo[MAX_USER_NAME_LEN];

    // 增加DSM文件的制作时间、创建时间、修改时间、有效期（天）
    char                        szFileMakeTime[50];         // 文档制作时间
    char                        szFileCreateTime[50];       // 文档创建时间
    char                        szFileUpdateTime[50];       // 文档修改时间
    char                        szFileAccessTime[50];       // 文档最后访问时间
    unsigned int                dwValidateDate;             // DSM文件有效期（天）

    // 注意：保留字段要严格控制大小！
    char                        szabout[564];               // 保留字段
};

/**
 * @brief
 *  加密，在 libfilelock.so 中定义
 */
extern void EncodeAES(guchar* pBuffer, guint64 nLength, guchar* key);

/**
 * @brief
 *  解密，在libfilelock.so 中定义
 */
extern void DecodeAES(guchar* pBuffer, guint64 nLength, guchar* key);


static CdgFileHeader* initCdgHeader(guint id, guint version, guint encType, guchar* szFileId, guchar* szFileName, guchar* szPassword, guint dwenleng, guint dwfileLen, gchar* szTemplateId, guint dwTemplateType, gchar* szUserName, gchar* szUserId, gchar* szSecInfo, gchar* szFileMakeTime, gchar* szFileCreateTime, gchar* szFileUpdateTime, gchar* szFileAccessTime, guint dwValidateDate);


unsigned long encryptinblockforjr (const gchar* fileid,/*in*/ guchar filetype, guint64 filesize, guchar* block, /*in/out*/ guint64 blocklen, guint64 fileoffset, guchar* cryptkey, gushort cryptkeylen, guchar* infilehead, guchar* outfilehead, /* out */ gushort* outfileheadsize, /* out */ guchar* datakey, gushort* datakeylength, guchar* headextenddata, gushort headextenddatalen, gchar* szfilename, gchar* sztid, gchar* szuserid, gchar* szusername, guint64 szfilemaketime)
{
    guint64 retVal = TCA_UNKNOW_ERROR;

    if (!fileid || !cryptkey || !block || !cryptkey || !cryptkeylen) {
        retVal = TCA_INVALID_PARAM;
        return retVal;
    }

    // 判断文件类型是否支持
#if 0
    if (TCA_FILE_DRM != filetype && TCA_FILE_TP != filetype && TPTemplate != filetype && Customize != filetype) {
        retVal = TCA_NOT_SUPPORT_TYPE;
        break;
    }
#endif
    // dwenleng

    CdgFileHeader* header = NULL;
    if (NULL == infilehead) {
        // 生成文件头
        //header = initCdgHeader();
    }
    else {
        // 更新文件头
    }

    // 加密数据调用 EncodeAes()

    return retVal;
}

static CdgFileHeader* initCdgHeader(guint id, guint version, guint encType, guchar* szFileId, guchar* szFileName, guchar* szPassword, guint dwenleng, guint dwfileLen, gchar* szTemplateId, guint dwTemplateType, gchar* szUserName, gchar* szUserId, gchar* szSecInfo, gchar* szFileMakeTime, gchar* szFileCreateTime, gchar* szFileUpdateTime, gchar* szFileAccessTime, guint dwValidateDate)
{
    if (!szFileId || !szFileName || !szPassword || !szTemplateId || !szUserName || !szUserId || !szSecInfo || !szFileMakeTime || !szFileCreateTime || !szFileUpdateTime || !szFileAccessTime) return NULL;

    g_autofree CdgFileHeader* header = (CdgFileHeader*) g_malloc0 (sizeof(CdgFileHeader));
    if (!header) return NULL;

    header->myID = CDG_FILE_HEAD_ID_DSM;
    header->version = 2;
    header->encType = 0;

    strncpy (header->szFileId, szFileId, sizeof (header->szFileId) - 1);
    strncpy (header->szFileName, szFileName, sizeof (header->szFileName) - 1);
    strncpy (header->szUserName, szUserName, sizeof (header->szUserName) - 1);
    strncpy (header->szUserId, szUserId, sizeof (header->szUserId) - 1);
    strncpy (header->szTemplateId, szTemplateId, sizeof (header->szTemplateId) - 1);

    //g_autofree char* uuid = g_uuid_string_random();
    strncpy(header->szPassword, szPassword, sizeof (header->szPassword));

    // 增加DSM文件制作时间、创建时间、文档最 后访问时间、有效期(天)
    strncpy (header->szFileMakeTime,   szFileMakeTime,   sizeof(header->szFileMakeTime) - 1);
    strncpy (header->szFileCreateTime, szFileCreateTime, sizeof(header->szFileCreateTime) - 1);
    strncpy (header->szFileUpdateTime, szFileUpdateTime, sizeof(header->szFileUpdateTime) - 1);
    strncpy (header->szFileAccessTime, szFileAccessTime, sizeof(header->szFileAccessTime) - 1);

    header->dwValidateDate = dwValidateDate;
    if (dwenleng < 200) {
        header->dwenleng = dwenleng;
    }
    else {
        header->dwenleng = 100;
        if (header->dwenleng < 8) {
            header->dwenleng = 8;
        }
    }

    return header;
}

