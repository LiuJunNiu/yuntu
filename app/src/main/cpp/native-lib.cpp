#include <jni.h>
#include <string>
#include <android/log.h>

#include <pthread.h>
#include <unistd.h>

#include "qcloud_iot_export.h"
#include "qcloud_iot_import.h"
#include "cJSON.h"
#include "android_wrapper.h"


extern "C" JNIEXPORT jstring JNICALL
Java_com_example_yuntunative_MainActivity_startYuntu(
        JNIEnv* env,
        jobject /* this */,
        jstring clsName) {
    std::string hello;
    Log_d("%s_%d", __func__, __LINE__);

    const char *className = env->GetStringUTFChars(clsName, 0);
    char tmp_classname[128] = {0};
    const char *p = className;
    int i = 0;

    for (i = 0; *p != 0 && i < 128; i++, p++) {
        if (*p == '.') tmp_classname[i] = '/';
        else
        tmp_classname[i] = *p;
    }
    Log_d("%s_%d classname %s", __func__, __LINE__, tmp_classname);
    int ret = YuntuStart(env, tmp_classname);

    if (ret) hello = "Start Error";
    else hello = "YunTu is running";

    return env->NewStringUTF(hello.c_str());
}
