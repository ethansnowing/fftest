#include <jni.h>
#include <string>

#include "FFDemux.h"
#include "XLog.h"
#include "FFDecode.h"

class TestObs:public IObserver
{
public:
    void Update(XData d)
    {
        LOGI("TestObs Update data size is %d", d.size);
    }
};

extern "C" JNIEXPORT jstring JNICALL
Java_com_yudehuai_ydhplay_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";


    //////////////////测试用代码
    TestObs *tobs = new TestObs();
    IDemux *de = new FFDemux();
    de->AddObs(tobs);
    de->Open("/sdcard/test.mp4");

    IDecode *vdecode = new FFDecode();
    vdecode->Open(de->GetVPara());

    IDecode *adecode = new FFDecode();
    vdecode->Open(de->GetAPara());
    de->Start();
//    XSleep(3000);
//    de->Stop();
    /*for(;;)
    {
        XData d = de->Read();
        LOGI("Read data size is %d",d.size);
    }*/

    return env->NewStringUTF(hello.c_str());
}
