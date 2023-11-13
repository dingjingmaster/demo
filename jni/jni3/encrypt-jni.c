/*************************************************************************
> FileName: encrypt-jni.c
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Mon 13 Nov 2023 01:14:43 PM CST
 ************************************************************************/
#ifndef _ENCRYPT_JNI_H
#define _ENCRYPT_JNI_H
#include "test.h"
#include <syslog.h>
#include "encrypt-in-block-for-jar.h"


static unsigned char convertJByteToChar(JNIEnv* env, jbyte byte);
static int* convertJIntArrayToIntPtr(JNIEnv* env, jintArray intArray);
static char* convertJByteArrayToCharArray(JNIEnv* env, jbyteArray byte);
static void convertCharToJByteArray(JNIEnv* env,char *array,int length,jbyteArray jba);


JNIEXPORT jint JNICALL Java_Main_encryptBlock(
        JNIEnv*         jniEnv,
        jobject         jniObj,
        jbyteArray      fileID,
        jbyte           fileType,
        jlong           fileSize,
        jbyteArray      block,
        jint            blockLen,
        jlong           fileOffset,
        jbyteArray      cryptKey,
        jint            cryptKeyLen,
        jbyteArray      inFileHead,
        jbyteArray      outFileHead,
        jintArray       outFileHeadSize,
        jbyteArray      dataKey,
        jintArray       headKeyLen,
        jint            headExtendVer,
        jbyteArray      headExtendData,
        jint            headExtendDataLen,
        jbyteArray      szFileName,
        jbyteArray      szTid,
        jbyteArray      szUserID,
        jbyteArray      szUserName,
        jint            validDate,
        jlong           fileMakeTime)
{
    char*           fileIDT             = convertJByteArrayToCharArray(jniEnv, fileID);
    unsigned char   fileTypeT           = convertJByteToChar(jniEnv, fileType);
    long            fileSizeT           = (long) fileSize;
    char*           blockT              = convertJByteArrayToCharArray(jniEnv, block);
    int*            blockLenT           = (int) blockLen;

    long            fileOffsetT         = (long) fileOffset;
    char*           cryptKeyT           = convertJByteArrayToCharArray(jniEnv, cryptKey);
    int             cryptKeyLenT        = (int) cryptKeyLen;
    char*           inFileHeadT         = convertJByteArrayToCharArray(jniEnv, inFileHead);
    char*           outFileHeadT        = convertJByteArrayToCharArray(jniEnv, outFileHead);
    int*            outFileHeadSizeT    = convertJIntArrayToIntPtr(jniEnv, outFileHeadSize);
    char*           dataKeyT            = convertJByteArrayToCharArray(jniEnv, dataKey);
    int*            headKeyLenT         = convertJIntArrayToIntPtr(jniEnv, headKeyLen);
    int             headExtendVerT      = (int) headExtendVer;
    char*           headExtendDataT     = convertJByteArrayToCharArray(jniEnv, headExtendData);
    int             headExtendDataLenT  = (int) headExtendDataLen;
    char*           szFileNameT         = convertJByteArrayToCharArray(jniEnv, szFileName);
    char*           szTidT              = convertJByteArrayToCharArray(jniEnv, szTid);
    char*           szUserIDT           = convertJByteArrayToCharArray(jniEnv, szUserID);
    char*           szUserNameT         = convertJByteArrayToCharArray(jniEnv, szUserName);
    int             validDateT          = (int) validDate;
    long            fileMakeTimeT       = (long) fileMakeTime;

    syslog(LOG_ERR, "\n \
            fileID: %s\n \
            fileType: %c\n \
            fileSize: %d\n \
            block: %s\n \
            blockLen: %d\n \
            cryptKey: %s\n \
            cryptKeyLen: %d\n \
            szFileName: %s\n \
            szTid: %s\n \
            szUserID: %s\n \
            szUserName: %s\n \
            validDate: %d\n \
            fileMakeTime: %d\n \
            ",
            (fileIDT ? fileIDT : "<null>"),
            fileTypeT,
            (fileSizeT ? fileSizeT : "<null>"),
            (blockT ? blockT : "<null>"),
            blockLenT,
            (cryptKeyT ? cryptKeyT : "<null>"),
            cryptKeyLenT, 
            (szFileNameT ? szFileNameT : "<null>"),
            (szTidT ? szTidT : "<null>"),
            (szUserIDT ? szUserIDT : "<null>"),
            (szUserNameT ? szUserNameT : "<null>"),
            validDateT, fileMakeTimeT
            );

    int ret = encryptinblockforjr(
            fileIDT,
            fileTypeT,
            fileSizeT,
            blockT,
            blockLenT,
            fileOffsetT,
            cryptKeyT,
            cryptKeyLenT,
            inFileHeadT,
            outFileHeadT,
            outFileHeadSizeT,
            dataKeyT,
            headKeyLenT,
            headExtendVerT,
            headExtendDataT,
            headExtendDataLenT,
            szFileNameT,
            szTidT,
            szUserIDT,
            szUserNameT,
            validDateT,
            fileMakeTimeT);

    convertCharToJByteArray(jniEnv, outFileHeadT, outFileHeadSizeT, outFileHead); 

    // 加入返回值输出
    syslog(LOG_ERR, "\n \
            fileID: %s\n \
            fileType: %c\n \
            fileSize: %d\n \
            block: %s\n \
            blockLen: %d\n \
            cryptKey: %s\n \
            cryptKeyLen: %d\n \
            szFileName: %s\n \
            szTid: %s\n \
            szUserID: %s\n \
            szUserName: %s\n \
            validDate: %d\n \
            fileMakeTime: %d\n \
            ",
            (fileIDT ? fileIDT : "<null>"),
            fileTypeT,
            (fileSizeT ? fileSizeT : "<null>"),
            (blockT ? blockT : "<null>"),
            blockLenT,
            (cryptKeyT ? cryptKeyT : "<null>"),
            cryptKeyLenT, 
            (szFileNameT ? szFileNameT : "<null>"),
            (szTidT ? szTidT : "<null>"),
            (szUserIDT ? szUserIDT : "<null>"),
            (szUserNameT ? szUserNameT : "<null>"),
            validDateT, fileMakeTimeT
            );

    return ret;
}

static void convertCharToJByteArray(JNIEnv* env, char *array, int length, jbyteArray jba) 
{
    (*env)->SetByteArrayRegion(env, jba, 0, length, (jbyteArray*)array);
}


static char* convertJByteArrayToCharArray(JNIEnv* env, jbyteArray byte)
{
    if (byte == NULL ){ //|| ((*env)->IsNull(env, byte) == JNI_TRUE)) {
        return NULL;
    }
    jsize len = (*env)->GetArrayLength(env, byte);
    jbyte* eles = (*env)->GetByteArrayElements(env, byte, NULL);

    char* arr = (char*) malloc (len + 1);
    if (NULL == arr) {
        return NULL;
    }
    memset(arr, 0, len + 1);
    strncpy(arr, eles, len);

    // 释放 jbyteArray 指针
    (*env)->ReleaseByteArrayElements(env, byte, eles, JNI_ABORT);

    return arr;
}

static unsigned char convertJByteToChar(JNIEnv* env, jbyte byte)
{
    return (unsigned char) (byte & 0xFF);

    (void) env;
}

static int* convertJIntArrayToIntPtr(JNIEnv* env, jintArray intArray)
{
    jint* elements = (*env)->GetIntArrayElements(env, intArray, NULL);
    jsize len = (*env)->GetArrayLength(env, intArray);

    int* newArr = (int*) malloc (sizeof(int) * len);
    memset (newArr, 0, sizeof(int) * len);

    memcpy(newArr, elements, len * sizeof(int));

    (*env)->ReleaseIntArrayElements(env, intArray, elements, JNI_ABORT);

    return newArr;
}


#endif
