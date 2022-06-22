/*************************************************************************
> FileName: test.h
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: Wed 22 Jun 2022 02:32:50 PM CST
 ************************************************************************/
#ifndef _TEST_H
#define _TEST_H
#include <jni.h>

#ifdef __cplusplus
extern "c" {
#endif


JNIEXPORT void JNICALL Java_JavaMain_show (JNIEnv* env, jobject thisObj);



#ifdef __cplusplus
}
#endif
#endif
