#include <jni.h>
#include <string>

#include "FFDemux.h"
#include "XLog.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_yudehuai_ydhplay_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";


    //////////////////测试用代码
    IDemux *de = new FFDemux();
    de->Open("/sdcard/test.mp4");
    de->Start();
    /*for(;;)
    {
        XData d = de->Read();
        LOGI("Read data size is %d",d.size);
    }*/

    return env->NewStringUTF(hello.c_str());
}
