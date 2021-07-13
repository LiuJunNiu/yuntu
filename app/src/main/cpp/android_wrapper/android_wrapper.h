#ifndef __ANDROID_WRAPPER_H_
#define __ANDROID_WRAPPER_H_

#include <jni.h>
#include <string>
#include <android/log.h>

#include <pthread.h>
#include <unistd.h>

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved);
int YuntuStart(JNIEnv *env, const char* cls_name);

#endif
