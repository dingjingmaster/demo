#include "encrypt-in-block-for-jar.h"
#include <time.h>
#include <stdio.h>
#include <syslog.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <uuid/uuid.h>


#define LOG_FILE "library_log.txt"

static uint32_t g_dwenleng = 0;
static uint32_t g_dfileleng = 0;
char g_password[20] = {0};

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
    
    // 模板类型：TP模板，普通模板，非模板自由授权，放入文件头中用于确定打开文件时该从哪获取权限信息
    unsigned int                dwTemplateType;
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
extern void EncodeAES(char *pBuffer, size_t nLength, unsigned char* key);

/**
 * @brief
 *  解密，在libfilelock.so 中定义
 */
extern void DecodeAES(char *pBuffer, size_t nLength, unsigned char* key);

/**
 * @brief
 *  加密文件内容所在的缓冲区，在 libfilelock.so 中定义
 */
extern bool EncryptLockFileBuffer(char* pBuffer, uint64_t lnOffset, uint32_t Length, unsigned char* key, int Arith);

/**
 * @brief
 *  解密文件内容所在的缓冲区，在 libfilelock.so 中定义
 */
extern bool DecryptLockFileBuffer(char* FileBuffer, uint64_t lnOffset, uint32_t Length,unsigned char* Key,int Arith);

static uint64_t makeDSIPDrmFileHeadEx(const char* fileId, unsigned char fileType, uint64_t fileSize, unsigned char* cryptKey, unsigned short cryptKeyLength, unsigned char* fileHead, unsigned short* fileHeadSize, unsigned char* dataKey, unsigned short* dataKeyLength, unsigned short headExtendVer, unsigned char* headExtendData, unsigned short headExtendDataLength, char* szFileName, char* szTID, char* szUserID, char* szUserName, uint32_t nValidateDate, const char* szFileMakeTime);

char* createGuid();

bool IsDSIPCDGHeaderV3( CdgFileHeader *pHeader );

/**
 * @brief
 *  写日志到文件
 */
void WriteLog(const char* format, ...);

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
                            const char* szfilemaketime)
{
#if 1
    syslog (LOG_ERR, "[YST] fileid: %s\n", fileid);
    syslog (LOG_ERR, "[YST] filetype: %u\n", (unsigned int)filetype);
    syslog (LOG_ERR, "[YST] filesize: %llu\n", (unsigned long long)filesize);
    syslog (LOG_ERR, "[YST] block: %s\n", block);
    syslog (LOG_ERR, "[YST] blocklen: %u\n", blocklen);
    syslog (LOG_ERR, "[YST] fileoffset: %llu\n", (unsigned long long)fileoffset);
    syslog (LOG_ERR, "[YST] cryptkey: %s\n", cryptkey);
    syslog (LOG_ERR, "[YST] cryptkeylen: %hu\n", cryptkeylen);
    syslog (LOG_ERR, "[YST] infilehead: %s\n", infilehead);
    syslog (LOG_ERR, "[YST] outfilehead: %s\n", outfilehead);
    syslog (LOG_ERR, "[YST] outfileheadsize: %hu\n", *outfileheadsize);
    syslog (LOG_ERR, "[YST] datakey: %s\n", datakey);
    syslog (LOG_ERR, "[YST] datakeylength: %hu\n", *datakeylength);
    syslog (LOG_ERR, "[YST] headextendver: %hu\n", headextendver);
    syslog (LOG_ERR, "[YST] headextenddatalen: %hu\n", headextenddatalen);
    syslog (LOG_ERR, "[YST] szfilename: %s\n", szfilename);
    syslog (LOG_ERR, "[YST] sztid: %s\n", sztid);
    syslog (LOG_ERR, "[YST] szuserid: %s\n", szuserid);
    syslog (LOG_ERR, "[YST] szusername: %s\n", szusername);
    syslog (LOG_ERR, "[YST] nValidateDate: %u\n", nValidateDate);
    syslog (LOG_ERR, "[YST] szfilemaketime: %s\n", szfilemaketime);
#endif

    uint64_t retVal = TCA_UNKNOW_ERROR;
    unsigned char mDRMHeader[TA_DSIP_FILE_HEADER_LENGTH] = { 0 };
    unsigned char retDRMHeader[TA_DSIP_FILE_HEADER_LENGTH] = { 0 };
    unsigned short wDRMHeaderSize = TA_DSIP_FILE_HEADER_LENGTH;
    char outFileId[TA_FILE_ID_LENGTH] = { 0 };
    unsigned char outFileType = TCA_FILE_NONE;
    do
    {
        if (!fileid || !cryptkey || !block || !cryptkeylen) {
            retVal = TCA_INVALID_PARAM;
            syslog (LOG_ERR, "[EST] fileid or cryptkey or block or block is error!\n");
            return retVal;
        }

        // 判断文件类型是否支持
    //#if 0
        if (TCA_FILE_DRM != filetype && TCA_FILE_TP != filetype && TPTemplate != filetype && Customize != filetype) {
            retVal = TCA_NOT_SUPPORT_TYPE;
            syslog (LOG_ERR, "[EST] This file type is not supported error is 7\n");
            break;
        }
    //#endif
        char Key[MAX_KEY_LEN + 2] = { 0x00 };
        memcpy(Key, cryptkey, TA_FILE_KEY_LENGTH);

        if (blocklen < 200) {
            g_dwenleng = blocklen;
        } else {
            // 获取当前时间戳作为随机数种子
            time_t current_time;
            time(&current_time);
            srand((unsigned int)current_time);

            // 生成随机数
            unsigned int random_seed = rand();
            unsigned int g_dwenleng = random_seed % 200;
            if (g_dwenleng < 8)
                g_dwenleng = 8;
        }

        CdgFileHeader* header = NULL;
        if (NULL == infilehead) {
            // 无文件头生成文件头
            syslog (LOG_ERR, "[EST] Decrypt: no file head!");
            retVal = makeDSIPDrmFileHeadEx(fileid, filetype, filesize, (unsigned char*)Key, TA_FILE_KEY_LENGTH,
                    (unsigned char*)&mDRMHeader, &wDRMHeaderSize, datakey, datakeylength, 
                    headextendver, headextenddata, headextenddatalen,
                    szfilename, sztid, szuserid, szusername, nValidateDate, szfilemaketime);
            if (retVal)
            {
                retVal = 111;
                syslog (LOG_ERR, "[EST] The generated file header is abnormal error\n");
                break;
            }
        }
        else {
            //有文件头更新部分内容
            uint64_t nRetVal = GetEncryptTypeInMemory(infilehead, sizeof(CdgFileHeader), outFileId, &outFileType);
            syslog (LOG_ERR, "[EST] Determine the type of the encrypted file \n");
            if (TCA_SUCCESS != nRetVal || (TCA_FILE_DRM != outFileType && TCA_FILE_TP != outFileType && TPTemplate != outFileType && Customize != outFileType))
            {
    				// 判断文件头是否是权限文件
                    retVal = nRetVal;
                    syslog (LOG_ERR, "[EST] Check that the file header is not a permission file error\n");
                    break;
            }
            CdgFileHeader* cdgFileHead = (CdgFileHeader*)infilehead;
            EncodeAES((char*)(cdgFileHead)+12, sizeof(CdgFileHeader) - 12, (unsigned char*)CDG_FILE_HEAD_PASS);
            syslog (LOG_ERR, "[EST] Encrypted file header \n");
            memcpy(&mDRMHeader, infilehead, sizeof(CdgFileHeader));
        }
        memcpy(&retDRMHeader, &mDRMHeader, sizeof(CdgFileHeader));

        //内存数据加密
        syslog (LOG_ERR, "[EST] Encrypted file content \n");
        if (!EncryptLockFileBuffer((char*)block, fileoffset, blocklen, (unsigned char*)Key, ARITH_RC4))
        {
            syslog (LOG_ERR, "[EST] Encrypted file content fail  error -1\n");
            retVal = -1;
            break;
        }
        if (NULL == infilehead)
        {
    	    //加密权限文件头
            syslog (LOG_ERR, "[EST] No file header, encrypted file header\n");
            EncodeAES((char*)block, g_dwenleng, (unsigned char*)g_password);
        }

        if (outfilehead && outfileheadsize)
        {
            syslog (LOG_ERR, "[EST] Check  file header\n");
            syslog (LOG_ERR, "[EST] Check  file header:%d", *outfileheadsize);
            if (*outfileheadsize >= TA_DSIP_FILE_HEADER_LENGTH)
            {
                memcpy(outfilehead, retDRMHeader, sizeof(CdgFileHeader));
    		    *outfileheadsize = sizeof(CdgFileHeader);
            }
            else
            {
                retVal = TCA_BUFFER_TO_SMALL;
                syslog (LOG_ERR, "[EST] File header too small error\n");
                break;
            }
        }
        syslog (LOG_ERR, "[EST] Encryption succeeded\n");
        retVal = TCA_SUCCESS;
        // if( retVal == 0)
        // {
        // //    // 获取$HOME环境变量的值
        //     char* homeDir = getenv("HOME");

        //     if (homeDir) {
        //         // 构建完整的输出文件路径
        //         char outputFilePath[256];
        //         snprintf(outputFilePath, sizeof(outputFilePath), "%s/test1.txt", homeDir);

        //         // 打开输出文件以供写入二进制数据
        //         FILE* output = fopen(outputFilePath, "wb");
        //         if (output) {
        //             // 写入outfilehead数据
        //             fwrite(outfilehead, 1, *outfileheadsize, output);
        //             // 写入block数据
        //             fwrite(block, 1, blocklen, output);
        //             fclose(output);
        //         } else {
        //             printf("加密：无法打开输出文件：%s\n", outputFilePath);
        //         }
        //     } else {
        //         printf("$HOME环境变量未设置\n");
        //     }
        //     // FILE* output = fopen("test1.txt", "wb");
        //     // syslog (LOG_ERR, "[EST] outfileheadsize: %hu\n", *outfileheadsize);
        //     // if (output) {
        //     //     fwrite(outfilehead, 1, *outfileheadsize, output);
        //     //     fwrite(block, 1, blocklen, output);
        //     //     fclose(output);
        //     // } else {
        //     //     printf("无法打开输出文件\n");
        //     // }
        // }

        // 加密数据调用 EncodeAes()
    } while (false);
    return retVal;
}

uint64_t DecryptInBlock(
    unsigned char* pBlock,
    size_t blockLength,
    uint64_t fileOffset,
    unsigned char* fileHead,
    unsigned char* cryptKey,
    size_t cryptKeyLength)
{

    // syslog (LOG_ERR, "[EST] block: %s\n", pBlock);
    // syslog (LOG_ERR, "[EST] blocklen: %u\n", blockLength);
    // syslog (LOG_ERR, "[EST] fileoffset: %llu\n", (unsigned long long)fileOffset);
    // syslog (LOG_ERR, "[EST] filehead: %s\n", fileHead);
    // syslog (LOG_ERR, "[EST] cryptkey: %s\n", cryptKey);
    // syslog (LOG_ERR, "[EST] cryptkeylen: %u\n", cryptKeyLength);

    uint64_t			retValue			= TCA_UNKNOW_ERROR;
    char				outFileId[TA_FILE_ID_LENGTH]	= {0};
    unsigned char		outFileType						= TCA_FILE_NONE;
    unsigned char		Key[32]							= {0};

    memcpy( Key, cryptKey, TA_FILE_KEY_LENGTH );

    if(retValue != TCA_SUCCESS)
    {
        do 
        {
            if( !pBlock || !blockLength || !fileHead || !cryptKey || cryptKeyLength < TA_FILE_KEY_LENGTH )
            {
                retValue = TCA_INVALID_PARAM;
                syslog (LOG_ERR, "[EST] Decrypt: Invalid input parameters (retValue: %llu)", retValue);
                break;
            }
            
            syslog (LOG_ERR, "[EST] Decrypt: pBlock size does not match blockLength (blockLength: %zu)",blockLength);


            bool RetVal = GetEncryptTypeInMemory( fileHead, sizeof(CdgFileHeader), outFileId, &outFileType );
            if( TCA_SUCCESS != RetVal || ( TCA_FILE_DRM != outFileType && TCA_FILE_TP != outFileType && TPTemplate != outFileType && Customize != outFileType) )
            {
    			// 不是加密文件
                retValue = TCA_FILE_NOT_ENCRYPTED;
                syslog (LOG_ERR, "[EST] Decrypt: Not encrypted file (retValue: %llu)", retValue);
                break;
            }
    
    		CdgFileHeader* pCDGHeader = (CdgFileHeader *)fileHead;
    		if(pCDGHeader->dwenleng > 200) // 防止数据头解密失败，保存的文件有加密数据大小超大
            {
                retValue = TCA_INVALID_HEADER;
                syslog (LOG_ERR, "[EST] Decrypt: Invalid data format header wrong (retValue: %llu)", retValue);
                break;
            }
    		if(pCDGHeader->dwenleng > blockLength && fileOffset == 0) // 首次传入数据大小 小于 加密时文件头二次加密数据大小
            {
                retValue = TCA_BUFFER_TO_SMALL;
                syslog (LOG_ERR, "[YST] Decrypt: The cache content is too small (retValue: %lu)", retValue);
                break;
            }
    
    
            if(fileOffset == 0)
            {
                syslog(LOG_ERR, "[YST] Decrypt: fileOffset = 0 ");
    			DecodeAES((char *)pBlock,pCDGHeader->dwenleng,(unsigned char *)pCDGHeader->szPassword);
            }
    
    		if(!DecryptLockFileBuffer((char *)pBlock, fileOffset, blockLength, (unsigned char*)Key, ARITH_RC4 ))
            {
    			// 不是加密文件
                retValue = TCA_OTHER_FILE;
                syslog (LOG_ERR, "[YST] Decrypt: DecryptLockFileBuffer fail (retValue: %lu)", retValue);
                break;
            }
            retValue = TCA_SUCCESS;
            syslog (LOG_ERR, "[YST] Decryption successful (retValue: %lu)", retValue);
            
            // if (retValue == 0)
            // {
            //     // 获取$HOME环境变量的值
            //     char* homeDir = getenv("HOME");

            //     if (homeDir) {
            //         // 构建完整的输出文件路径
            //         char outputFilePath[256];
            //         snprintf(outputFilePath, sizeof(outputFilePath), "%s/test2.txt", homeDir);

            //         // 打开输出文件以供写入二进制数据
            //         FILE* output = fopen(outputFilePath, "wb");
            //         if (output) {
            //             // 写入数据到输出文件
            //             fwrite(pBlock, 1, blockLength, output);
            //             fclose(output);
            //         } else {
            //             syslog (LOG_ERR, "[EST] 解密：无法打开输出文件：%s\n", outputFilePath);
            //         }
            //     } else {
            //         printf("$HOME环境变量未设置\n");
            //         syslog (LOG_ERR, "[EST] $HOME环境变量未设置\n");
            //     }
            // }

        } while (false);
    }
    return retValue;
}


static uint64_t makeDSIPDrmFileHeadEx(
    const char* fileId, 
    unsigned char fileType, 
    uint64_t fileSize, 
    unsigned char* cryptKey, 
    unsigned short cryptKeyLength, 
    unsigned char* fileHead, 
    unsigned short* fileHeadSize, 
    unsigned char* dataKey, 
    unsigned short* dataKeyLength, 
    unsigned short headExtendVer, 
    unsigned char* headExtendData, 
    unsigned short headExtendDataLength, 
    char* szFileName,     // 文件名（如：123.txt ，只要文件名和扩展名，不要路径）长度上限：256 [9/3/2022 dongziheng]
    char* szTID,          // 模板ID 长度上限：64[9/3/2022 dongziheng]
    char* szUserID,       // 用户ID 长度上限：128[9/3/2022 dongziheng]
    char* szUserName,     // 用户名 长度上限：128[9/3/2022 dongziheng]
    uint32_t nValidateDate,     // 有效期（单位：天；1-36000天，0为永久） [9/3/2022 dongziheng]
    const char* szFileMakeTime)    // 增加时间参数，满足自由选择有效期截止日期问题。文档制作时间、文档创建时间、文档修改时间、文档最后访问时间 [9/23/2023 dongziheng]
{
    syslog(LOG_ERR, "%s:%d", __func__, __LINE__);
    uint64_t retVal = TCA_UNKNOW_ERROR;
    if (!fileId || !cryptKey || !fileHead || !fileHeadSize)
	    return  TCA_INVALID_PARAM;
    syslog(LOG_ERR, "%s:%d", __func__, __LINE__);
    if (*fileHeadSize < TA_FILE_HEADER_LENGTH)
	    return  TCA_BUFFER_TO_SMALL;
    syslog(LOG_ERR, "%s:%d", __func__, __LINE__);
    if (fileType != TCA_FILE_DRM && fileType != TCA_FILE_TP && fileType != TPTemplate && fileType != Customize)
        return TCA_NOT_SUPPORT_TYPE;
    syslog(LOG_ERR, "%s:%d", __func__, __LINE__);

    CdgFileHeader  mDRMHeader = { 0 };
    CdgFileHeader* pDRMHeader = &mDRMHeader;

    // 组CDG文件头	
    syslog(LOG_ERR, "%s:%d", __func__, __LINE__);
    pDRMHeader->myID = CDG_FILE_HEAD_ID_DSM;
    pDRMHeader->version = 2;
    pDRMHeader->encType = 0;
    pDRMHeader->dwfileleng = fileSize;
    pDRMHeader->dwenleng = g_dwenleng;

    // 缺失部分信息
    syslog(LOG_ERR, "%s:%d", __func__, __LINE__);
    memset(pDRMHeader->szFileId, 0, 64);
    strncpy(pDRMHeader->szFileId, fileId, TA_FILE_ID_LENGTH);
    pDRMHeader->szFileId[TA_FILE_ID_LENGTH - 1] = '\0';

    syslog(LOG_ERR, "%s:%d", __func__, __LINE__);
    memset(pDRMHeader->szFileName, '\0', 256);
    strncpy(pDRMHeader->szFileName, szFileName, 256);
    pDRMHeader->szFileName[255] = '\0';

    syslog(LOG_ERR, "%s:%d", __func__, __LINE__);
    memset(pDRMHeader->szUserName, '\0', MAX_USER_NAME_LEN);
    strncpy(pDRMHeader->szUserName, szUserName, MAX_USER_NAME_LEN);
    pDRMHeader->szUserName[MAX_USER_NAME_LEN - 1] = '\0';

    syslog(LOG_ERR, "%s:%d", __func__, __LINE__);
    memset(pDRMHeader->szUserId, '\0', MAX_USER_NAME_LEN);
    strncpy(pDRMHeader->szUserId, szUserID, MAX_USER_NAME_LEN);
    pDRMHeader->szUserId[MAX_USER_NAME_LEN - 1] = '\0';

    syslog(LOG_ERR, "%s:%d", __func__, __LINE__);
    pDRMHeader->dwValidateDate = nValidateDate;

    syslog(LOG_ERR, "%s:%d", __func__, __LINE__);
    memset(pDRMHeader->szFileMakeTime, '\0', 50);
    strncpy(pDRMHeader->szFileMakeTime, szFileMakeTime, szFileMakeTime ? strlen(szFileMakeTime) : 0);
    pDRMHeader->szFileMakeTime[49] = '\0';

    syslog(LOG_ERR, "%s:%d", __func__, __LINE__);
    strncpy(pDRMHeader->szFileAccessTime, szFileMakeTime, szFileMakeTime ? strlen(szFileMakeTime) : 0);
    pDRMHeader->szFileAccessTime[49] = '\0';

    syslog(LOG_ERR, "%s:%d", __func__, __LINE__);
    strncpy(pDRMHeader->szFileCreateTime, szFileMakeTime, szFileMakeTime ? strlen(szFileMakeTime) : 0);
    pDRMHeader->szFileCreateTime[49] = '\0';

    syslog(LOG_ERR, "%s:%d", __func__, __LINE__);
    strncpy(pDRMHeader->szFileUpdateTime, szFileMakeTime, szFileMakeTime ? strlen(szFileMakeTime) : 0);
    pDRMHeader->szFileUpdateTime[49] = '\0';

    // 模板权限文件
    syslog(LOG_ERR, "%s:%d", __func__, __LINE__);
    pDRMHeader->dwTemplateType = fileType;
    strncpy(pDRMHeader->szTemplateId, szTID, TA_FILE_ID_LENGTH - 1);
    pDRMHeader->szTemplateId[TA_FILE_ID_LENGTH - 1] = '\0';
    
    // 产生随机密钥
    syslog(LOG_ERR, "%s:%d", __func__, __LINE__);
    char* password = createGuid();  // 假设 CreateGuid() 返回 char*
    memmove(pDRMHeader->szPassword, password, 16);
    memmove(g_password, password, TA_FILE_KEY_LENGTH);

    // 传出dataKey
    syslog(LOG_ERR, "%s:%d", __func__, __LINE__);
    *dataKeyLength = TA_FILE_KEY_LENGTH;
    memmove(dataKey, password, TA_FILE_KEY_LENGTH);

    // 加密CDG头
    syslog(LOG_ERR, "%s:%d", __func__, __LINE__);
    EncodeAES((char*)(&mDRMHeader) + 12, sizeof(CdgFileHeader) - 12, (unsigned char*)CDG_FILE_HEAD_PASS);

    // 复制文件头
    syslog(LOG_ERR, "%s:%d", __func__, __LINE__);
    memmove(fileHead, pDRMHeader, sizeof(CdgFileHeader));
    *fileHeadSize = sizeof(CdgFileHeader);

    syslog(LOG_ERR, "%s:%d", __func__, __LINE__);
    return TCA_SUCCESS;  // 返回 NULL 表示成功

}


// 生成UUID并返回字符串
char* createGuid() {
    uuid_t uuid;
    char uuidStr[37]; // UUID字符串的长度为36个字符 + 1个结束符
    
    // 生成UUID
    uuid_generate(uuid);

    // 将UUID转换为字符串
    uuid_unparse(uuid, uuidStr);

    // 创建字符串的副本，因为uuid_unparse使用内部缓冲区
    return strdup(uuidStr);
}

bool IsDSIPCDGHeaderV3( CdgFileHeader *pHeader )
{
	if (CDG_FILE_HEAD_ID_DSM != pHeader->myID)
	{
		return false;	
	}
	DecodeAES((char*)(pHeader)+12,sizeof(CdgFileHeader)-12,(unsigned char *)CDG_FILE_HEAD_PASS);  
	return true;
}

uint64_t GetEncryptTypeInMemory(const unsigned char* inBuffer,size_t inBufferLength,char* fileId,unsigned char* fileType)
{

    unsigned long RetVal = TCA_UNKNOW_ERROR;
    do 
    {   
        if( !inBuffer || !fileId || !fileType )
        {
            //RetVal = TCA_INVALID_PARAM;
            RetVal = 123;
            break;
        }
        if( inBufferLength < sizeof(CdgFileHeader) )
        {
            //RetVal = TCA_BAD_LENGTH;
            RetVal = 234;
            break;
        }
        if( !IsDSIPCDGHeaderV3( (CdgFileHeader *)inBuffer ) )
        {
            //RetVal = TCA_INVALID_HEADER;
            RetVal = 678;
            break;
        }
        *fileType = ((CdgFileHeader *)inBuffer)->dwTemplateType;
        syslog (LOG_ERR, "[YST] dwTemplateType: %u", ((CdgFileHeader *)inBuffer)->dwTemplateType);
        syslog (LOG_ERR, "[YST] fileType: %u\n", *fileType);

        RetVal = TCA_SUCCESS;
    } while ( false );
    return RetVal;
}

/* 写日志的函数 */
void WriteLog(const char* format, ...)
{
    char* homeDir = getenv("HOME");
    char logFilePath[256];
    if (homeDir) {
        // 构建完整的日志文件路径
        snprintf(logFilePath, sizeof(logFilePath), "%s/%s", homeDir, LOG_FILE);

        // 使用logFilePath作为日志文件路径
        //printf("日志文件路径: %s\n", logFilePath);
    } else {
        printf("$HOME环境变量未设置\n");
    }


    FILE* logFile = fopen(logFilePath, "a"); // "a" 表示以追加模式打开文件
    if (logFile)
    {
        time_t currentTime;
        struct tm* timeInfo;
        char timeString[50];

        va_list args;
        va_start(args, format);

        time(&currentTime);
        timeInfo = localtime(&currentTime);
        strftime(timeString, sizeof(timeString), "[%Y-%m-%d %H:%M:%S] ", timeInfo);

        fprintf(logFile, "%s", timeString);
        vfprintf(logFile, format, args);
        fprintf(logFile, "\n");

        va_end(args);
        fclose(logFile);
    }
}

unsigned int GetEncryptTypeInMemoryEx(const unsigned char* inBuffer, size_t inBufferLength) {
    unsigned int fileType = 0; // 默认值为 0，如果操作失败则保持为 0

    if (!inBuffer || inBufferLength < sizeof(CdgFileHeader) || !IsDSIPCDGHeaderV3((CdgFileHeader*)inBuffer)) {
        // 操作失败，返回默认值 0
        syslog (LOG_ERR, "[EST] Failed to determine the file type. Procedure\n");
        return fileType;
    }

    fileType = ((CdgFileHeader*)inBuffer)->dwTemplateType;
    syslog (LOG_ERR, "[EST] file type is %d\n",fileType);
    return fileType;
}



