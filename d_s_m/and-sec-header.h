#ifndef _AND_SEC_HEADER_H_
#define _AND_SEC_HEADER_H_
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


#ifdef __cplusplus
extern "C" 
{
#endif


#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OPTIONAL
#define OPTIONAL
#define OPT
#endif

#define HASH_ARITH_NONE             0
#define HASH_ARITH_MD5              1
#define HASH_ARITH_SHA1             2
#define HASH_ARITH_SHA2             3
#define HASH_ARITH_SM3              8

#define ENCRYPT_ARITH_NONE          0
//stream 
#define ENCRYPT_ARITH_RC4           10
#define ENCRYPT_ARITH_ENRC4         11
///
#define ENCRYPT_ARITH_RC5           20
#define ENCRYPT_ARITH_RC6           30
//block
#define ENCRYPT_ARITH_DES           40
#define ENCRYPT_ARITH_3DES          41
#define ENCRYPT_ARITH_SEAL          50
#define ENCRYPT_ARITH_TEA           60
#define ENCRYPT_ARITH_XXTEA         61
#define ENCRYPT_ARITH_AES           70
#define ENCRYPT_ARITH_SM4           80

#define MAX_KEY_LEN                 32
#define USERID_LENGTH               32
#define HEAD_DATA_SIZE              32
#define MAX_MACHINE_UUID            32
#define MAX_FONT_NAME_LEN           15
#define ENCRYPTKEY_LEN              64
#define MAX_SHOWTEXT_LEN            95
#define MAX_MOUNT_PATH              16
#define MAX_MAP_PATH                256
#define SAFE_HEAD_SIZE              1024
#define FILE_HEAD_SIZE              4096
#define FILE_HEAD_VERSION           0x02
#define SAFE_FILE_MAGIC             0x4D524418
#define SAFE_FILE_MAGICID           0xC4CEDCC3


#ifndef C_SUPPORTED_C11
#if defined(__GNUC__) && __GNUC__ >= 4 && __GNUC_MINOR__ >= 7
#define C_SUPPORTED_C11 1
#elif defined(__clang__) && __clang_major__ >= 3 && __clang_minor__ >= 0
#define C_SUPPORTED_C11 1
#else
#define C_SUPPORTED_C11 0
#endif
#endif

#ifndef C_STRUCT_OFFSET_OF
#define C_STRUCT_OFFSET_OF(structType, member)                                  offsetof(structType, member)
#endif

#ifndef C_STRUCT_SIZE_CHECK
#if C_SUPPORTED_C11
#define C_STRUCT_SIZE_CHECK(structType, expectedSize)                           static_assert(sizeof(structType) == expectedSize, "struct '"#structType"' size is wrong");
#else
#define C_STRUCT_SIZE_CHECK(structType, expectedSize)                           typedef char _macros_check_size##structType[(sizeof(structType) == expectedSize ? 1 : -1)];
#endif
#endif

#ifndef C_TYPE_SIZE_CHECK
#define C_TYPE_SIZE_CHECK(typeT, sizeT)                                         C_STRUCT_SIZE_CHECK(typeT, sizeT)
#endif

typedef struct _SafeFileHead    SafeFileHead;
typedef struct _SafeFileHead*   PSafeFileHead;
typedef struct _DrmFileHead     DrmFileHead;
typedef struct _DrmFileHead*    PDrmFileHead;
typedef struct _DrmFileHead     BoxFileHead;
typedef struct _DrmFileHead*    PBoxFileHead;

enum 
{
    ENCRYPT_MODE_ECB        = 0,
    ENCRYPT_MODE_CBC        = 1,
    ENCRYPT_MODE_CFB        = 2
};

typedef enum
{
    ENCRYPTOR_DRIVER        = 0x1,
    ENCRYPTOR_MCLIENT       = 0x10,
    ENCRYPTOR_ACLIENT       = 0x30,
    ENCRYPTOR_API           = 0x100,
    ENCRYPTOR_RESERVED      = 0x10000000,
} Encryptor;

typedef enum
{
    ENMODE_WRITE            = 0x1,
    ENMODE_OPEN             = 0x2,
    ENMODE_SEPARATE         = 0x10,
    ENMODE_RESERVED         = 0x10000000,
} EnMode;

typedef enum
{
    CIPHERTYPE_NONE         = 0x0,
    CIPHERTYPE_ENCRYPT      = 0x1,
    CIPHERTYPE_DECRYPT      = 0x2,
    CIPHERTYPE_DECRYPT_CLOUD= 0x4,
} CipherType;

typedef enum
{
    QUARCTRL_GLOABLE        =  0x1,
    QUARCTRL_ORG            =  0x10,
    QUARCTRL_PERSONAL       =  0x100,
    QUARCTRL_RESERVED       = 0x10000000,
} QuarCtrl;

typedef enum
{
    WORKMODE_NORMAL         = 0x0,
    WORKMODE_USERSELF       = 0x1,
    WORKMODE_WRITEENCRYPT   = 0x2,
} WorkMode;

typedef enum
{
    VOLUMEINFO_NORMAL       = 0x0,
    VOLUMEINFO_READONLY     = 0x01,
    VOLUMEINFO_HIDDEN       = 0x02,
    VOLUMEINFO_REMOVABLE    = 0x04,
    VOLUMEINFO_CLOUDBOX     = 0x01000,
    VOLUMEINFO_SANDBOX      = 0x02000,
    VOLUMEINFO_ULONG        = 0xFFFFFFFF
} VolumeInfo;

typedef enum
{
    DECTRL_ALLOW            = 0x1,
    DECTRL_FORBIDEN         = 0x2,
    DECTRL_RESERVED         = 0x10000000,
} DeCtrl;

typedef enum _FileType
{
//file head = 1K
    FILE_TYPE_NONE          = 0x00,
    FILE_TYPE_SIGNATURE     = 0x01,
    FILE_TYPE_SELF          = 0x02,
    FILE_TYPE_CONFIG        = 0x08,

//below file head = 4K
    FILE_TYPE_POLICY        = 0x10,

//MonSafe.sys related file
    FILE_TYPE_TDE           = 0x100,

///BoxSafe.sys related file
    FILE_TYPE_ODM           = 0x1000,
    FILE_TYPE_CLOUDBOX      = FILE_TYPE_ODM,
    FILE_TYPE_CLOUD         = 0x2000,
    FILE_TYPE_NETDISK       = FILE_TYPE_CLOUD,
    FILE_TYPE_SANDBOX       = 0x4000,
    FILE_TYPE_DATABOX       = 0x8000,
////
    FILE_TYPE_ANY           = 0xFFFF,
} FileType;

typedef enum
{
    WMTYPE_NONE             = 0x0,
    WMTYPE_BLIND            = 0x1,
//visible 
    WMTYPE_APP              = 0x10,
    WMTYPE_READ             = 0x20,
    WMTYPE_OUT              = 0x40,
//
    WMTYPE_PRINT            = 0x100,
//Charles
    WMTYPE_FILE             = 0x200,
    WMTYPE_ENCRYPT_PRINT    = 0x400,
} WMType;

typedef struct
{
    int64_t                         arith;
    int64_t                         arithMode;
    uint16_t                        keyLen;
    union {
        char                        encryptKey[ENCRYPTKEY_LEN];
        // DJ- FIXME://
        //rc4_context                 rc4Ctx;
        //aes_context                 aesCtx;
        //CSm4Ctx                     sm4Ctx;
    };
} CryptInfo, *PCryptInfo;


typedef struct __attribute__((packed))
{
    uint32_t                        nFixType;
    uint32_t                        nFixcount;
    uint32_t                        nReserved;
    char                            fixUserID[USERID_LENGTH];
    char                            fixReviewID[USERID_LENGTH];
} FixLevel;

typedef struct __attribute__((packed))
{
    uint32_t                        bytesPerSector;
    uint32_t                        sectorsPerBlock;
    uint32_t                        sectorsPerBAT;
    uint32_t                        batNums;
    uint32_t                        batStartLBA;
    uint32_t                        batStartLBA1;
    uint32_t                        allocBlocks;
    uint32_t                        reserved[3];
} BatInfo, *PCBatInfo;

typedef struct __attribute__((packed))
{
    WMType                          eWMType; 
    uint32_t                        eShowMode;
    union {
        uint32_t                    xPart;
        uint32_t                    gridNum;
    };
    union {
        uint32_t                    yPart;
        uint32_t                    reserved;
    };
    char                            showText[MAX_SHOWTEXT_LEN + 1];
    char                            fontName[MAX_FONT_NAME_LEN + 1];
    uint32_t                        fontDepth;
    uint32_t                        fontSize;
    uint32_t                        fontColor;
    uint32_t                        fontWeight;
    uint32_t                        fontOrient;
    uint32_t                        eDispSet;
    uint32_t                        reserved1;
} WaterMark, *PCWaterMark;

typedef struct __attribute__((packed))
{
    uint32_t                        eAuthMode;
    int64_t                         tmExpireTime;
    uint32_t                        nTermCount;
    uint32_t                        nOffLineHours;
    uint32_t                        reserved[2];
    char                            passwdSM3[32];
    char                            szServerUrl[128];
    uint32_t                        serverPort;
    char                            szBakServerUrl[128];
    uint32_t                        bakServerPort;
} Authority;

typedef struct __attribute__((packed))
{
    int64_t                         tmViewTimeFrom;
    int64_t                         tmViewTimeEnd;
    uint32_t                        nEditCountLimit;
    uint32_t                        nEditCount;
    uint32_t                        nPrintCountLimit;
    uint32_t                        nPrintCount;
    uint32_t                        nViewCountLimit;
    uint32_t                        nViewCount;
    uint32_t                        nClipCount;
    uint32_t                        eAuthMode;
    uint32_t                        nReserved[2];
} UserPriv, *PUserPriv;


struct __attribute__((packed)) _SafeFileHead 
{
    unsigned int                    magic;
    unsigned int                    magicID;
    unsigned short                  version;
    unsigned short                  headSize;
    unsigned short                  fileType;
    unsigned short                  headArith;
    char                            prompt[68];
    unsigned short                  reserved[2];
// off = 64 + 24 = 88
    char                            authorID[USERID_LENGTH];        // 作者ID/文件类型
    char                            departID[USERID_LENGTH];        // 作者所在部门ID/文件类型扩展
    unsigned int                    userState;                      // 当前用户读写删除权限/签名状态等
// off = 88 + 64 + 4 = 156
    unsigned short                  headItems;
    unsigned short                  fileItems;
// off = 160 = 0xA0
// begin to encrypt
    char                            dataKey[MAX_KEY_LEN];           // 
    unsigned int                    dataKeyAdler;                   //
    unsigned int                    masterKeyAdler;                 //
    unsigned short                  keyArith;                       // 加密DataKey的算法
    unsigned short                  dataArith;                      // 数据解密算法
    unsigned short                  dataMode;                       // 数据解密模式
    unsigned short                  headDataSize;                   // DataAdler size
// off = 208 = 0xD0
    unsigned int                    dataAdler;                      // 
    unsigned int                    headAdler;                      //
    unsigned int                    reserved1[2];
// 160 + 32 + 4 * 8 = 224
    char                            headData[HEAD_DATA_SIZE];
};
C_TYPE_SIZE_CHECK(SafeFileHead, 256);

struct __attribute__((packed)) _DrmFileHead
{
    SafeFileHead                    safeHead;
    unsigned int                    subType;
    unsigned int                    passwdAdler;
    unsigned int                    nAuthMode;
    unsigned int                    reserved[9];
    char                            companyID[128];                 // 公司标识
    char                            customerSN[32];                 // 客户编号，例如：分公司
    char                            fileID[32];
    short                           extName[16];                    // 源文件扩展名, 不包含 "."
    char                            userID[USERID_LENGTH];          // 用户ID，在用户进行权限查询并确认权限变化后保存
    char                            organID[USERID_LENGTH];         // 组织机构ID
    uint64_t                        createTime;                     // 文件创建时间
    uint64_t                        modifyTime;                     // 最后修改时间
    uint64_t                        fileLength;                     // 加密前的文件长度，如果位0，= fileLength - 4K
    unsigned int                    reserved1[18];                  //

// cipher begin
// off = 688 = 0x2b0, align 16
    char                            licenseSN[32];                  // License 编号
    uint32_t                        eDrmCtrl;                       // 存放内容控制，DRMCTRL
    uint32_t                        fileVersion;                    // =0; 这个版本是指源文件数据的版本，主要针对服务器管控文档，普通文档此字段为0
    Encryptor                       encryptor;                      // 谁加密该文件，驱动、模板、权限工具、流转工具
    EnMode                          encryptMode;                    // 加密类型：自动1(仅写动作加密)，自动2(只要打开强制加密)、非自动(明是明、密是密-密文读写正常加解密)
    QuarCtrl                        quarCtrl;                       // 隔离控制，来自策略，策略匹配，全局、部门、个人
    DeCtrl                          decryptCtrl;                    // 脱密控制
    unsigned int                    reserved2[14];
    Authority                       authority;                      // 授权信息
    UserPriv                        userPriv;                       // 当前使用者权限
    UserPriv                        authorPriv;                     // 作者或授权者权限
    WaterMark                       printWM;                        // 打印水印形式：0 -- 无水印, 1 --
    WaterMark                       screenWM;                       // 屏幕水印形式：0 -- 无水印, 1 --

// 标密、定密
    unsigned int                    secLevel;                       // 密级
    unsigned int                    secModelID;                     // 密级模板ID
    unsigned int                    secMonth;                       // 安全年限(按月计算)
    unsigned int                    reserved3[80];                  //
#if defined(LINUX) || defined(__GNUC__)
    union BothCommon {
#else
    union {
#endif
        struct BoxSafe {
            unsigned int            uuidNums;
            char                    machineUUID[MAX_MACHINE_UUID][16];      // 16字节一个; 512bytes
            char                    reserved4[372];
            uint64_t                diskOffset;                     // 相对整个文件或者U盘，分区镜像文件在硬盘中开始位置，非分区镜像一般为0
            uint64_t                volumeOffset;                   // 相对DiskOffset映射区间在文件头中的位置，存放Boot sector
            uint64_t                volumeSize;                     // 虚拟卷大小, byte 单位
            short                   volumeLabel[16];
            uint64_t                diskSignature;
            BatInfo                 batInfo;
            char                    reserved5[200];
            char                    lastSector[512];
        } box;

        struct MonSafe {
            FixLevel                fixLevelInfo;                   // 定密信息
            char                    secOrigOwnerName[256];          // 标密人名称，超长追加"..."
            char                    secApplicantName[256];          // 定密申请人名称，超长追加"..."
            char                    secAppDeputyName[256];          // 定密审批人列表，超长追加"..."
            uint32_t                reserved4[215];
        } mon;
#if defined(LINUX) || defined (__GNUC__)
    } monBox;
#else
    };
#endif
    uint32_t                        drmHeadAdler;                   // 文件头部到 nDrmHeadAdlper的Adler32
    char                            lastNoUse[508];                 //
};
C_TYPE_SIZE_CHECK(DrmFileHead, 4096);


extern void GenUUIDStr              (OUT uint8_t* buffer);
extern uint32_t GetStrAdler32       (IN const uint8_t* szString);
extern void InitCipher              (IN OUT CryptInfo *pCryptInfo);
extern uint64_t Adler64             (IN const uint8_t* buffer, IN uint32_t len);
extern uint32_t Adler32             (IN const uint8_t* buffer, IN uint32_t len);
extern uint32_t Adler16             (IN const uint8_t* buffer, IN uint32_t len);
extern void GenRandKey              (OUT uint8_t* buffer, IN uint32_t bufferLen);
extern bool GetDRMFileId            (IN const uint8_t* fileName, OUT int8_t fileID);
extern void MakeSafeHead            (INOUT SafeFileHead* fileHead, IN uint16_t fileType);
extern bool ReadSafeHead            (IN const char* fileName, OUT SafeFileHead* fileHead);
extern bool DecodeDRMHead           (INOUT DrmFileHead* fileHead, IN const uint8_t* masterKey);
extern void EncodeDRMHead           (INOUT DrmFileHead* fileHead, IN const uint8_t* masterKey);
extern void DecodeDES               (IN uint8_t* pBuffer,IN uint32_t nLength, IN const uint8_t* Key);
extern void EncodeDES               (IN uint8_t* pBuffer,IN uint32_t nLength, IN const uint8_t* Key);
extern void DecodeSM4               (IN uint8_t* pBuffer,IN uint32_t nLength, IN const uint8_t* Key);
extern void EncodeSM4               (IN uint8_t* pBuffer,IN uint32_t nLength, IN const uint8_t* Key);
extern void DecodeDES3              (IN uint8_t* pBuffer, IN uint32_t nLength, IN const uint8_t* Key);
extern void EncodeDES3              (IN uint8_t* pBuffer, IN uint32_t nLength, IN const uint8_t* Key);
extern void EncodeXXTEA             (IN OUT uint8_t* buffer, IN uint32_t nLen, IN const uint8_t* key);
extern void DecodeXXTEA             (IN OUT uint8_t* buffer, IN uint32_t nLen, IN const uint8_t* key);
extern void EncodeTEA               (IN OUT uint8_t* buffer, IN uint32_t nLen, IN const uint8_t* key);
extern void DecodeTEA               (IN OUT uint8_t* buffer, IN uint32_t nLen, IN const uint8_t* key);
extern void DecodeSEAL              (IN uint8_t* pBuffer, IN uint32_t nLength, IN const uint8_t* Key);
extern void EncodeSEAL              (IN uint8_t* pBuffer, IN uint32_t nLength, IN const uint8_t* Key);
extern uint32_t String2Key          (OUT char* keyArray, IN uint32_t keyArrayLen, IN uint8_t* inString);
extern void Key2String              (OUT uint8_t* outString, IN char* keyArray, IN uint32_t keyArrayLen);
extern bool CheckSafeHead           (IN SafeFileHead* fileName, IN uint16_t fileType/* = FILE_TYPE_ANY*/);
extern bool DecodeSafeHead          (INOUT SafeFileHead* fileName, IN const uint8_t* masterKey/* = NULL*/);
extern void EncodeSafeHead          (INOUT SafeFileHead* fileName, IN const uint8_t* masterKey/* = NULL*/);
extern bool GetPlainDRMDataKey      (INOUT void* pFileHead, INOUT uint8_t* dataKey, IN uint8_t dataKeyLen);
extern bool WriteDRMHead            (IN void* handle, IN DrmFileHead* pFileHead, IN const uint8_t* masterKey);
extern bool ReadDRMHead             (IN void* handle, OUT DrmFileHead* pFileHead, IN const uint8_t* masterKey);
extern bool WriteSafeHead           (IN const char* fileName, IN SafeFileHead* fileHead, IN bool update /*= false*/);
extern bool GetDRMDataKey           (INOUT DrmFileHead* fileHead, IN const uint8_t* masterKey, INOUT uint8_t* dataKey);
extern bool ChgDRMMasterKey         (INOUT DrmFileHead* fileName, IN const uint8_t* oldMasterKey, IN uint8_t* newMaster);
extern void DecodeRC4               (IN uint8_t* pBuffer, IN uint32_t nLength, IN const uint8_t* Key, IN uint32_t KeyLen);
extern void EncodeRC4               (IN uint8_t* pBuffer, IN uint32_t nLength, IN const uint8_t* Key, IN uint32_t KeyLen);
extern void DecodeENRC4             (IN uint8_t* pBuffer, IN uint32_t nLength, IN const uint8_t* Key, IN uint32_t KeyLen);
extern void EncodeENRC4             (IN uint8_t* pBuffer, IN uint32_t nLength, IN const uint8_t* Key, IN uint32_t KeyLen);
extern void ExpandKey               (IN const char* inputBuffer, IN int64_t inputLen, OUT char* outputBuffer, IN int64_t outputLen);
extern void DecryptBuffer           (IN uint8_t* buffer, IN uint32_t nLen, IN const uint8_t* key, IN uint32_t keyLen, IN int arith);
extern void EncryptBuffer           (IN uint8_t* buffer, IN uint32_t nLen, IN const uint8_t* key, IN uint32_t keyLen, IN int arith);
extern bool GetDRMMasterKey         (INOUT DrmFileHead* fileHead, IN const uint8_t* listKey, IN uint32_t listKeyNums, INOUT uint8_t* masterKey);
extern bool DecryptSafeFileBuffer   (IN OUT uint8_t* fileBuffer, IN uint32_t len, IN uint64_t lnOffset, IN const uint8_t* dataKey, IN int arith/* = ENCRYPT_ARITH_RC4*/);
extern bool EncryptSafeFileBuffer   (IN OUT uint8_t* fileBuffer, IN uint32_t len, IN uint64_t lnOffset, INOUT const uint8_t* dataKey, IN int arith/* = ENCRYPT_ARITH_RC4*/);
extern bool WriteSafeFile           (IN const char* fileName, IN const SafeFileHead* fileHead, IN void* dataBuffer, IN uint32_t dataBytes, IN int64_t offset /*= SAFE_HEAD_SIZE*/);
extern bool ReadSafeFile            (IN const char* fileName, IN const SafeFileHead* fileHead, OUT void* dataBuffer, IN uint32_t dataBytes, IN int64_t offset /* = SAFE_HEAD_SIZE*/);
extern void MakeDRMHead             (INOUT DrmFileHead* fileHead, IN uint32_t fileType, IN uint32_t subType /*= 0*/, IN uint8_t* headBuffer/* = NULL*/, IN uint32_t headBufferSize /* = 0*/);
extern void DecodeAES               (IN uint8_t* pBuffer, IN uint32_t nLength, IN const uint8_t* Key, IN uint32_t KeyLen, IN uint8_t* IV /* = NULL*/, uint32_t iMode/* = ENCRYPT_MODE_ECB*/);
extern void EncodeAES               (IN uint8_t* pBuffer, IN uint32_t nLength, IN const uint8_t* Key, IN uint32_t KeyLen, IN uint8_t* IV /* = NULL*/, uint32_t iMode/* = ENCRYPT_MODE_ECB*/);

/**
 * @brief 检查给定的文件是否为加密的文件
 */
extern bool MonIsEncryptFile        (IN const uint8_t* fileName);

/**
 * @brief 加密成符合动态加解密格式的密文
 * @param fileName: 文件名
 * @param masterKey: 加密主密钥,至少应为32字节，支持0x00-0xFF的全字符
 * @param arith：加密算法,见常量ARITH_xxxx
 *
 * EDG密钥的输入方式是(支持多密钥):
 *  1)直接传递密钥数据字符串:
 *      如有两个加密密钥：AA3456789012345678901234567890AA和BB1234567890123456789012345678BB
 *      则传递密钥为：“AA3456789012345678901234567890AABB1234567890123456789012345678BB”或“AA3456789012345678901234567890AA|BB1234567890123456789012345678BB”
 *      其中前面的方式只适用于密钥长度为32个字符的情形，如果密钥长度不足32个字符，则只能用方式2，密钥之间用'|'进行分隔，如|1233243242342ABC|EDGF34324234234|等
 *  2)通过加入|KEYFLAG_EDG|标记传递:
 *      密钥同上，传递的密钥数据字符串为：“|KEYFLAG_EDG|AA3456789012345678901234567890AA|BB1234567890123456789012345678BB|”，其中最后一个'|'分隔符也可以没有
 *
 * 兼容第三方密钥的输入方式是(（支持多密钥）:
 * 为兼容第三方，在有第三方密文的前提下，密钥的输入方式是:
 * 加密成EDG文件格式:
 *  |KEYFLAG_EDG|Monsafe的加密密钥|KEYFLAG_CDG8|第三方密钥1|第三方密钥2|...
 * 加密成第三方文件格式：
 *  |KEYFLAG_CDG8|第三方的加密密钥|第三方密钥2|第三方密钥3|...
 *
 * 其中:
 *  第一个KEYFLAG_用于标记加密的时候是加密成EDG文件格式(标记为KEYFLAG_EDG)还是第三方文件格式(标记为KEYFLAG_CDG8等第三方标记)，用于兼容第三方密文
 *  |KEYFLAG_EDG|为Monsafe的密钥标识,由调用程序自己加入，如果要加密为第三方的密文模式，则第一个密钥标记应为第三方的密钥标记
 *  |KEYFLAG_CDG8|为第三方的密钥标识，不同的第三方标识不同,由用户设置密钥时候输入
 *
 * 例如加密成EDG密文格式:
 *  MonSafe密钥为: 1234567890abcdefgh
 *  1) 没有第三方密钥: 调用程序直接传递该密钥(因Monsafe密钥为32个字符，因此传递给参数时，要存放到空间>=32字节中，不足部分用0补足)或者传递|KEYFLAG_EDG|1234567890abcdefgh| (这种格式如果密钥长度不够32个字符，则不需要补足)
 *  2) 如果用户输入第三方密钥|KEYFLAG_IPG|ABCDEFGHIJKLMNOP12345678|0987654321CEDG| 
 *  则调用程序应将这两类密钥组合为:
 *      |KEYFLAG_EDG|1234567890abcdefgh|KEYFLAG_IPG|ABCDEFGHIJKLMNOP12345678|0987654321CEDG|
 *      然后再传递组合后的密钥(Monsafe的密钥不足32字符不需要特殊处理，库文件会自动补足)
 *
 * 例如加密成CDG8密文格式(只有组合密钥，没有独立密钥):
 *  A. CDG8只有单密钥: 1234567890abcdefgh123456
 *      调用程序的组合密钥为: |KEYFLAG_CDG8|1234567890abcdefgh123456| 然后再传递组合后的密钥
 *  B. CDG8有多密钥，其中加密密钥为1234567890abcdefgh123456，其它密钥为：|ABCDEFGHIJKLMNOP12345678|0987654321CEDG|
 *      调用程序的组合密钥为:
 *          |KEYFLAG_CDG8|1234567890abcdefgh123456|ABCDEFGHIJKLMNOP12345678|0987654321CEDG|
 *      其中加密密钥1234567890abcdefgh123456必须为第一个
 *
 * @return
 *      -18: 获取密钥错误
 *      -14: 获取多密钥错误
 *      -13: 文件名太长
 *      -12: 坏文件
 *      -11: 校验不通过
 *      -10: 文件大小错误
 *      -9: 移动指针错误
 *      -8: 数据错误
 *      -7: 内存分配错误
 *      -6: 无法加密文件
 *      -5: 无法写文件
 *      -4: 无法读文件
 *      -3: 无法创建新文件
 *      -2: 无法打开源文件
 *      -1: 获取密钥错误
 *      <0: 错误码
 *      0： 加密失败
 *      1： 加密成功
 *      2： 文件已经加密（文件本身是密文）
 */
extern int EncryptSafeFile          (IN const char* fileName, IN const char* masterKey, IN int arith/* = ENCRYPT_ARITH_RC4 */);

/**
 * @brief 加密成符合动态加解密格式的密文
 * @param pPlainFileName：待加密的源文件全名
 * @param pCipherFileName: 加密后的文件全名
 * @param masterKey: 加密主密钥,至少应为32字节，支持0x00-0xFF的全字符。定义方法：UCHAR Key[32] ={0}; 可定义为32个字符，方便以后扩展到256位密钥
 * @param arith: 加密算法,见常量ENCRYPT_ARITH__xxxx
 *
 * @return
 *      -18: 获取密钥错误
 *      -14: 获取多密钥错误
 *      -13: 文件名太长
 *      -12: 坏文件
 *      -11: 校验不通过
 *      -10: 文件大小错误
 *      -9: 移动指针错误
 *      -8: 数据错误
 *      -7: 内存分配错误
 *      -6: 无法加密文件
 *      -5: 无法写文件
 *      -4: 无法读文件
 *      -3: 无法创建新文件
 *      -2: 无法打开源文件
 *      -1: 获取密钥错误
 *      <0: 错误码
 *      0： 加密失败
 *      1： 加密成功
 *      2： 文件已经加密（文件本身是密文）
 */
extern int EncryptSafeFile2         (IN const char* pPlainFileName, IN const char* pCipherFileName, IN const uint8_t* masterKey, IN int arith/* = ENCRYPT_ARITH_RC4*/);

/**
 * @return
 *      -18: 获取密钥错误
 *      -14: 获取多密钥错误
 *      -13: 文件名太长
 *      -12: 坏文件
 *      -11: 校验不通过
 *      -10: 文件大小错误
 *      -9: 移动指针错误
 *      -8: 数据错误
 *      -7: 内存分配错误
 *      -6: 无法加密文件
 *      -5: 无法写文件
 *      -4: 无法读文件
 *      -3: 无法创建新文件
 *      -2: 无法打开源文件
 *      -1: 密钥错误
 *      0： 加密失败
 *      1： 加密成功
 *      2： 文件已经加密（文件本身是密文）
 */
extern int DecryptSafeFile          (IN const char* pFileName, IN  const uint8_t* masterKey, IN int arith/* = ENCRYPT_ARITH_RC4*/);

/**
 * @return
 *      -18: 获取密钥错误
 *      -14: 获取多密钥错误
 *      -13: 文件名太长
 *      -12: 坏文件
 *      -11: 校验不通过
 *      -10: 文件大小错误
 *      -9: 移动指针错误
 *      -8: 数据错误
 *      -7: 内存分配错误
 *      -6: 无法加密文件
 *      -5: 无法写文件
 *      -4: 无法读文件
 *      -3: 无法创建新文件
 *      -2: 无法打开源文件
 *      -1: 密钥错误
 *      <0: 错误码
 *      0： 加密失败
 *      1： 加密成功
 *      2： 文件已经加密(文件本身是密文)
 */
extern int DecryptSafeFile2         (IN const char* pCipherFileName, IN const char* pPlainFileName, IN  const uint8_t* masterKey, IN int arith/* = ENCRYPT_ARITH_RC4*/);

static inline uint32_t GetSafeHeadAdler (IN SafeFileHead* fileHead)
{
    return Adler32((uint8_t*) &fileHead->magic, C_STRUCT_OFFSET_OF(SafeFileHead, headAdler));
}

/**
 * @brief 检查给定的文件头内容是否为加密的文件
 */
static inline bool MonChechEncrypt (IN SafeFileHead* fileHead)
{
    return CheckSafeHead(fileHead, FILE_TYPE_TDE);
}

static inline void print_safe_head (SafeFileHead* sh)
{
    if (!sh) { return; }

    printf("magic       : %u\n", sh->magic);
    printf("magicID     : %u\n", sh->magicID);
    printf("version     : %u\n", sh->version);
    printf("headSize    : %u\n", sh->headSize);
    printf("fileType    : %u\n", sh->fileType);
    printf("headArith   : %u\n", sh->headArith);
    printf("prompt      : %s\n", sh->prompt);
    printf("reserved    : %u %u\n", sh->reserved[0], sh->reserved[1]);
    printf("authorID    : %s\n", sh->authorID);
    printf("departID    : %s\n", sh->departID);
    printf("userState   : %u\n", sh->userState);
    printf("headItems   : %u\n", sh->headItems);
    printf("fileItems   : %u\n", sh->fileItems);
    printf("dataKey     : %s\n", sh->dataKey);
    printf("dataKeyAdler: %u\n", sh->dataKeyAdler);
    printf("keyArith    : %u\n", sh->keyArith);
    printf("dataArith   : %u\n", sh->dataArith);
    printf("dataMode    : %u\n", sh->dataMode);
    printf("headDataSize: %u\n", sh->headDataSize);
    printf("dataAdler   : %u\n", sh->dataAdler);
    printf("headAdler   : %u\n", sh->headAdler);
    printf("reserved1   : %u %u\n", sh->reserved1[0], sh->reserved1[1]);
    printf("headData    : %s\n", sh->headData);
}


#ifdef __cplusplus
}
#endif
#endif

