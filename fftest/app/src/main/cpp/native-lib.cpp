#include <jni.h>
#include <string>
#include <android/log.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOG_TAG "jnidemo"

extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavcodec/jni.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}
//获取当前时间戳，也可以用clock,但是并不靠谱
long long GetNowMs(){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int sec = tv.tv_sec%360000;     //只取100小时内的时间
    long long t = sec*1000 + tv.tv_usec/1000;
    return t;
}

static double r2d(AVRational r){
    return r.num==0||r.den==0?0:(double)r.num/(double)r.den;
}

extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *res){
    av_jni_set_java_vm(vm, 0);
    return JNI_VERSION_1_6;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_fftest_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    hello += avcodec_configuration();
    return env->NewStringUTF(hello.c_str());
}

extern "C" jboolean JNICALL
Java_com_example_fftest_MainActivity_Open(
        JNIEnv *env,
        jobject /* this */,
        jstring url_,
        jobject handle) {
    //return true;
    jboolean is_copy = false;
    const char *url = env->GetStringUTFChars(url_,&is_copy);
    FILE *fp = fopen(url, "rb");
    if(!fp){
        LOGI("ydh--fp %s fail!", url);
    }
    else{
        LOGI("ydh--fp %s succes!", url);
    }
    env->ReleaseStringUTFChars(url_,url);
    return true;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_fftest_XPlay_Open2(JNIEnv *env, jobject instance, jstring url_, jobject surface) {
    const char *path = env->GetStringUTFChars(url_, 0);
    //初始化解封装
    av_register_all();
    //初始化网络
    avformat_network_init();
    //解码器初始化
    avcodec_register_all();
    //打开文件
    AVFormatContext *ic = NULL;
//    char path[] = "/storage/emulated/0/test.mp4";
    int re = avformat_open_input(&ic, path, 0, 0);
    if(re != 0){
        LOGI("ydh--ffmpeg open %s fail!", path);
        return;
    }
    LOGI("ydh--ffmpeg open %s  succes!", path);
    //获取流信息
    re = avformat_find_stream_info(ic,0);
    if(re != 0){
        LOGI("ydh--avformat_find_stream_info faild!");
    }
    LOGI("ydh--duration = %lld, nb_streams = %d", ic->duration, ic->nb_streams);
    int fps = 0;
    int widght = 0;
    int heigit = 0;
    int video_stream = 0;
    int audio_stream = 1;



    for(int i=0;i<ic->nb_streams; i++){
        AVStream *as = ic->streams[i];
        if(as->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
            LOGI("ydh--video data");
            video_stream = i;
            fps = r2d(as->avg_frame_rate);
            LOGI("fps = %d, width = %d, height = %d, codeid = %d, format = %d",
                 fps,
                 as->codecpar->width,
                 as->codecpar->height,
                 as->codecpar->codec_id,
                 as->codecpar->format);
        }
        else if(as->codecpar->codec_type == AVMEDIA_TYPE_AUDIO){
            LOGI("ydh--audio data");
            audio_stream = i;
            LOGI("ydh--sample_rate = %d, channels = %d, format = %d",as->codecpar->sample_rate,
                 as->codecpar->channels,
                 as->codecpar->format);
        }
    }
    //获取音频流信息
    audio_stream = av_find_best_stream(ic,AVMEDIA_TYPE_AUDIO,-1,-1,NULL,0);
    LOGI("ydh--av_find_best_stream audiostream = %d",audio_stream);

    /////////////////////////视频解码
    //软码器
    AVCodec *codec = avcodec_find_decoder(ic->streams[video_stream]->codecpar->codec_id);
    //硬解码
//    codec = avcodec_find_encoder_by_name("h264_mediacodec");
    if(!codec){
        LOGI("ydh--avcodec_find video decoder faild!");
        return;
    }
    LOGI("ydh--avcodec find video decoder success!");
    //解码器初始化
    AVCodecContext *vc = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(vc, ic->streams[video_stream]->codecpar);
    vc->thread_count = 1;

    //打开解码器
    re = avcodec_open2(vc, 0, 0);
    if(re != 0){
        LOGI("ydh--avcodec_open2 video faild!");
        return;
    }
    LOGI("ydh--avcodec open2 video success!");

    ///////////////////////音频解码
    //软码器
    AVCodec *acodec = avcodec_find_decoder(ic->streams[audio_stream]->codecpar->codec_id);
    if(!acodec){
        LOGI("ydh--avcodec_find audio decoder faild!");
        return;
    }
    LOGI("ydh--avcodec_find audio decoder success!");
    //解码器初始化
    AVCodecContext *ac = avcodec_alloc_context3(acodec);
    avcodec_parameters_to_context(ac, ic->streams[audio_stream]->codecpar);
    ac->thread_count = 1;

    //打开解码器
    re = avcodec_open2(ac, 0, 0);
    if(re != 0){
        LOGI("ydh--avcodec_open2 audio faild!");
        return;
    }
    LOGI("ydh--avcodec open2 audio success!");

    //读取帧数据
    AVPacket *pkt = av_packet_alloc();
    AVFrame *frame =av_frame_alloc();
    //初始化像素格式转化的上下文
    SwsContext *vctx = NULL;
    int outWidth = 1280;
    int outHeight = 720;
    char *rgb = new char[1920*1080*4];
    char *pcm = new char[48000*4*2];
    //音频重采样上下文初始化
    SwrContext *actx = swr_alloc();
    actx = swr_alloc_set_opts(actx,
                              av_get_default_channel_layout(2),
                              AV_SAMPLE_FMT_S16, ac->sample_rate,
                              av_get_default_channel_layout(ac->channels),
                              ac->sample_fmt, ac->sample_rate,
                              0,0);
    re = swr_init(actx);
    if(re != 0){
        LOGI("ydh--swr_init failed!");
    }else{
        LOGI("ydh--swr_init success!");
    }

    long long start = GetNowMs();
    int frameCount = 0;
    //显示窗口初始化
    ANativeWindow *nwin = ANativeWindow_fromSurface(env, surface);
    ANativeWindow_setBuffersGeometry(nwin, outWidth, outHeight, WINDOW_FORMAT_RGBA_8888);
    ANativeWindow_Buffer wbuf;

    for(;;){
        if(GetNowMs()-start >= 3000){
            LOGI("ydh--now decode fps is %d", frameCount/3);
            start = GetNowMs();
            frameCount = 0;     //3秒钟统计一次视频帧
        }
        int re = av_read_frame(ic, pkt);
        if(re != 0){
            LOGI("读取到文件末尾处！");
            int pos = 20 * r2d(ic->streams[video_stream]->time_base);
            av_seek_frame(ic, video_stream, pos, AVSEEK_FLAG_BACKWARD|AVSEEK_FLAG_FRAME);
            continue;
        }
        //先测试视频
        /*if(pkt->stream_index != video_stream){
            continue;
        }*/
//        LOGI("stream = %d, size = %d, pts = %lld, flag = %d",
//                pkt->stream_index, pkt->size, pkt->pts, pkt->flags);

        AVCodecContext *cc = vc;
        if(pkt->stream_index == audio_stream){
            cc = ac;
        }
        //发送到线程中解码
        re = avcodec_send_packet(cc, pkt);
        /////////
        av_packet_unref(pkt);
        if(re != 0){
            LOGI("ydh--avcodec_send_packet faild!");
            continue;
        }
        for(;;){
            re = avcodec_receive_frame(cc,frame);
            if(re != 0){
                //LOGI("ydh--avcodec_receive_frame faild!");
                break;
            }
//            LOGI("ydh--avcodec_receive_frame pts = %lld", frame->pts);
            if(cc == vc){   //是视频帧就加1，统计视频帧数
                frameCount++;
                vctx = sws_getCachedContext(vctx,
                                            frame->width,
                                            frame->height,
                                            (AVPixelFormat)frame->format,
                                            outWidth,
                                            outHeight,
                                            AV_PIX_FMT_RGBA,
                                            SWS_FAST_BILINEAR,
                                            0,0,0);
                if(!vctx){
                    LOGI("ydh--sws_getCachedContext failed!");
                }
                else{
                    uint8_t *data[AV_NUM_DATA_POINTERS] = {0};
                    data[0] = (uint8_t *)rgb;
                    int lines[AV_NUM_DATA_POINTERS] = {0};
                    lines[0] = outWidth*4;

                    int h = sws_scale(vctx,
                                      (const uint8_t **)frame->data,
                                      frame->linesize,
                                      0,
                                      frame->height,
                                      data,lines);
                    LOGI("ydh--sws_scale = %d",h);

                    //使用GLSurface显示画面
                    if(h>0){
                        ANativeWindow_lock(nwin, &wbuf, 0);
                        uint8_t *dst = (uint8_t *)wbuf.bits;
                        memcpy(dst, rgb, outWidth*outHeight*4);
                        ANativeWindow_unlockAndPost(nwin);

                    }
                }
            }else{//音频
                uint8_t *out[2] = {0};
                out[0] = (uint8_t *)pcm;
                //音频重采样
                int len = swr_convert(actx,out,
                                      frame->nb_samples,
                                      (const uint8_t **)frame->data,
                                      frame->nb_samples);
                LOGI("ydh--swr_convert = %d", len);
            }
        }

    }
    delete rgb;
    delete pcm;

    //关闭上下文
    avformat_close_input(&ic);

    env->ReleaseStringUTFChars(url_, path);
}