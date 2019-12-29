#include <jni.h>
#include <string>
#include <android/log.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOG_TAG "jnidemo"

static SLObjectItf  engineSL = NULL;
SLEngineItf CreateSL(){
    SLresult re;
    SLEngineItf en;
    re = slCreateEngine(&engineSL,0,0,0,0,0);
    if(re!= SL_RESULT_SUCCESS) return NULL;
    re = (*engineSL)->Realize(engineSL, SL_BOOLEAN_FALSE);
    if(re!= SL_RESULT_SUCCESS) return NULL;
    re = (*engineSL)->GetInterface(engineSL, SL_IID_ENGINE, &en);
    if(re!= SL_RESULT_SUCCESS) return NULL;
    return en;
}

void PcmCall(SLAndroidSimpleBufferQueueItf bf, void *contex){
    LOGE("ydh--PcmCall");
    static FILE *fp = NULL;
    static char *buf = NULL;
    if(!buf){
        buf = new char[1024*1024];
    }
    if(!fp){
        fp = fopen("/storage/emulated/0/test.pcm", "rb");
    }
    if(!fp) {
        LOGE("ydh--open test.pcm failed!");
        return;
    }
    if(feof(fp) ==0){
        int len = fread(buf, 1, 1024, fp);
        if(len > 0)
            (*bf)->Enqueue(bf, buf, len);
    }
}

extern "C" JNIEXPORT jstring JNICALL
Java_aplay_testopensl_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    //1、创建引擎
    SLEngineItf eng = CreateSL();
    if (eng){
        LOGE("ydh--CreateSL success!");
    }else{
        LOGE("ydh--CreateSL failed!");
    }

    //2、创建混音器
    SLObjectItf mix = NULL;
    SLresult re = 0;
    re = (*eng)->CreateOutputMix(eng, &mix, 0, 0, 0);
    if(re!= SL_RESULT_SUCCESS){
        LOGE("ydh--(*eng)->CreateOutputMix failed!");
    }
    re = (*mix)->Realize(mix, SL_BOOLEAN_FALSE);
    if(re!= SL_RESULT_SUCCESS){
        LOGE("ydh--(*mix)->Realize failed!");
    }
    SLDataLocator_OutputMix outmix = {SL_DATALOCATOR_OUTPUTMIX, mix};
    SLDataSink audioSink = {&outmix, 0};

    //3 配置音频信息
    SLDataLocator_AndroidSimpleBufferQueue que = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 10};
    //音频格式
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,
            2, //声道数
            SL_SAMPLINGRATE_44_1,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
            SL_BYTEORDER_LITTLEENDIAN //字节序，小端
    };
    SLDataSource ds = {&que, &pcm};

    //4 创建播放器
    SLObjectItf player = NULL;
    SLPlayItf iplayer = NULL;
    SLAndroidSimpleBufferQueueItf pcmQue = NULL;
    const SLInterfaceID ids[] = {SL_IID_BUFFERQUEUE};
    const SLboolean req[] = {SL_BOOLEAN_TRUE};
    re = (*eng)->CreateAudioPlayer(eng,&player,&ds,&audioSink,sizeof(ids)/sizeof(SLInterfaceID),ids,req);
    if (re!=SL_RESULT_SUCCESS){
        LOGE("ydh--CreateAudioPlayer failed!");
    }else{
        LOGE("ydh--CreateAudioPlayer success!");
    }
    (*player)->Realize(player, SL_BOOLEAN_FALSE);
    //获取player接口
    re = (*player)->GetInterface(player, SL_IID_PLAY, &iplayer);
    if (re!=SL_RESULT_SUCCESS){
        LOGE("ydh--GetInterface failed!");
    }else{
        LOGE("ydh--GetInterface success!");
    }
    re = (*player)->GetInterface(player, SL_IID_BUFFERQUEUE, &pcmQue);
    if (re!=SL_RESULT_SUCCESS){
        LOGE("ydh--GetInterface SL_IID_BUFFERQUEUE failed!");
    }else{
        LOGE("ydh--GetInterface SL_IID_BUFFERQUEUE success!");
    }

    //设置回调函数，播放队列为空时调用
    (*pcmQue)->RegisterCallback(pcmQue, PcmCall, 0);
    //设置为播放状态
    (*iplayer)->SetPlayState(iplayer, SL_PLAYSTATE_PLAYING);
    //启动队列回调
    (*pcmQue)->Enqueue(pcmQue, " ", 1);

    return env->NewStringUTF(hello.c_str());
}
