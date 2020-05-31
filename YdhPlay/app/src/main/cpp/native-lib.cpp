#include <jni.h>
#include <string>
#include <android/native_window_jni.h>

#include "IPlayerPorxy.h"

extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *res)
{
    IPlayerPorxy::Get()->Init(vm);
//    IPlayerPorxy::Get()->Open("/sdcard/1280x720A.mp4");
//    IPlayerPorxy::Get()->Start();
//    IPlayerPorxy::Get()->Open("/sdcard/1280x536.mp4");
//    IPlayerPorxy::Get()->Start();
    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_yudehuai_ydhplay_YdhPlay_InitView(JNIEnv *env, jobject instance, jobject surface) {

    // TODO
    ANativeWindow *win = ANativeWindow_fromSurface(env,surface);
    IPlayerPorxy::Get()->InitView(win);


}


//extern "C" JNIEXPORT jstring JNICALL
//Java_com_yudehuai_ydhplay_MainActivity_stringFromJNI(
//        JNIEnv *env,
//        jobject /* this */) {
//    std::string hello = "Hello from C++";
//    return env->NewStringUTF(hello.c_str());
//}
extern "C"
JNIEXPORT void JNICALL
Java_com_yudehuai_ydhplay_OpenUrl_Open(JNIEnv *env, jobject thiz, jstring url_) {
    // TODO: implement Open()
    const char *url = env->GetStringUTFChars(url_,0);
    IPlayerPorxy::Get()->Open(url);
    IPlayerPorxy::Get()->Start();
//    IPlayerPorxy::Get()->Seek(0.5);
    env->ReleaseStringUTFChars(url_,url);
}extern "C"
JNIEXPORT jdouble JNICALL
Java_com_yudehuai_ydhplay_MainActivity_PlayPos(JNIEnv *env, jobject thiz) {
    // TODO: implement PlayPos()
    return IPlayerPorxy::Get()->PlayPos();
}extern "C"
JNIEXPORT jboolean JNICALL
Java_com_yudehuai_ydhplay_MainActivity_Seek(JNIEnv *env, jobject thiz, jdouble pos) {
    // TODO: implement Seek()
    return IPlayerPorxy::Get()->Seek(pos);
}extern "C"
JNIEXPORT void JNICALL
Java_com_yudehuai_ydhplay_YdhPlay_playOrPause(JNIEnv *env, jobject thiz) {
    IPlayerPorxy::Get()->SetPause(!IPlayerPorxy::Get()->IsPause());
}