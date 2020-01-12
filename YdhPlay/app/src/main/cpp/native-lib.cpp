#include <jni.h>
#include <string>
#include <android/native_window_jni.h>

#include "FFPlayerBuilder.h"

static IPlayer *player = NULL;
extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *res)
{

    ////////////////////////////只创建这些对象，将Open交给IPlayer->Open()
    FFPlayerBuilder::InitHard(vm);
    //////////////////测试用代码
    player = FFPlayerBuilder::Get()->BuilderPlayer();


    player->Open("/sdcard/1280x536.mp4");
    player->Start();

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
//    将下面这些启动播放器的代码交给IPlayer->Start()
    if(player)
    {
        player->InitView(win);
    }
//    view->SetRender(win);
//    XEGL::Get()->Init(win);
//    XShader shader;
//    shader.Init();

}