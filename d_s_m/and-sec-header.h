#ifndef _AND_SEC_HEADER_H_
#define _AND_SEC_HEADER_H_
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

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

#define MAX_KEY_LEN                 32
#define USERID_LENGTH               32
#define HEAD_DATA_SIZE              32
#define MAX_MACHINE_UUID            32
#define MAX_FONT_NAME_LEN           15
#define MAX_SHOWTEXT_LEN            95
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
    QUARCTRL_GLOABLE        =  0x1,
    QUARCTRL_ORG            =  0x10,
    QUARCTRL_PERSONAL       =  0x100,
    QUARCTRL_RESERVED       = 0x10000000,
} QuarCtrl;

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

#endif
