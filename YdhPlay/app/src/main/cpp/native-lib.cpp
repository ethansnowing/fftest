#include <jni.h>
#include <string>
#include <android/native_window_jni.h>

#include "FFDemux.h"
#include "XLog.h"
#include "FFDecode.h"
#include "XEGL.h"
#include "XShader.h"
#include "IVideoView.h"
#include "GLVideoView.h"
#include "IResample.h"
#include "FFResample.h"
#include "IAudioPlay.h"
#include "SLAudioPlay.h"
#include "IPlayer.h"

class TestObs:public IObserver
{
public:
    void Update(XData d)
    {
        LOGI("TestObs Update data size is %d", d.size);
    }
};

IVideoView *view = NULL;
extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *res)
{
    /*  //添加IPlayer接口，将代码重写
    FFDecode::InitHard(vm);
    //////////////////测试用代码
    TestObs *tobs = new TestObs();
    IDemux *de = new FFDemux();
//    de->AddObs(tobs);
    de->Open("/sdcard/1280x536.mp4");

    IDecode *vdecode = new FFDecode();
    vdecode->Open(de->GetVPara(), true);

    IDecode *adecode = new FFDecode();
    adecode->Open(de->GetAPara());
    de->AddObs(vdecode);
    de->AddObs(adecode);

    //渲染
    view = new GLVideoView();
    vdecode->AddObs(view);

    //音频重采样
    IResample *resample = new FFResample();
    XParameter outPara = de->GetAPara();
    resample->Open(de->GetAPara(), outPara);
    adecode->AddObs(resample);

    IAudioPlay *audioPlay = new SLAudioPlay();
    audioPlay->StartPlay(outPara);
    resample->AddObs(audioPlay);

    de->Start();
    vdecode->Start();
    adecode->Start();
    return JNI_VERSION_1_4;
     */

    FFDecode::InitHard(vm);
    //////////////////测试用代码
    TestObs *tobs = new TestObs();
    IDemux *de = new FFDemux();
//    de->Open("/sdcard/1280x536.mp4");

    IDecode *vdecode = new FFDecode();
//    vdecode->Open(de->GetVPara(), true);

    IDecode *adecode = new FFDecode();
//    adecode->Open(de->GetAPara());
    de->AddObs(vdecode);
    de->AddObs(adecode);

    //渲染
    view = new GLVideoView();
    vdecode->AddObs(view);

    //音频重采样
    IResample *resample = new FFResample();
//    XParameter outPara = de->GetAPara();
//    resample->Open(de->GetAPara(), outPara);
    adecode->AddObs(resample);

    IAudioPlay *audioPlay = new SLAudioPlay();
//    audioPlay->StartPlay(outPara);
    resample->AddObs(audioPlay);

    IPlayer::Get()->demux = de;
    IPlayer::Get()->adecode = adecode;
    IPlayer::Get()->vdecode = vdecode;
    IPlayer::Get()->videoView = view;
    IPlayer::Get()->resample = resample;
    IPlayer::Get()->audioPlay = audioPlay;


    IPlayer::Get()->Open("/sdcard/1280x536.mp4");

//    de->Start();
//    vdecode->Start();
//    adecode->Start();
    return JNI_VERSION_1_4;
}


extern "C" JNIEXPORT jstring JNICALL
Java_com_yudehuai_ydhplay_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    //原本运行代码都写在这里，但是因为声音有两遍，所以写到JNI_OnLoad函数
//    XSleep(3000);
//    de->Stop();
    /*for(;;)
    {
        XData d = de->Read();
        LOGI("Read data size is %d",d.size);
    }*/

    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_yudehuai_ydhplay_YdhPlay_InitView(JNIEnv *env, jobject instance, jobject surface) {

    // TODO
    ANativeWindow *win = ANativeWindow_fromSurface(env,surface);
    view->SetRender(win);
//    XEGL::Get()->Init(win);
//    XShader shader;
//    shader.Init();

}