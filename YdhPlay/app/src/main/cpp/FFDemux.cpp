//
// Created by 余德怀 on 2020/1/5.
//

#include "FFDemux.h"
#include "XLog.h"
extern "C"{
#include <libavformat/avformat.h>
}

//打开文件，或者流媒体 rtmp http rtsp
bool FFDemux::Open(const char *url)
{
    LOGI("Open file %s begin", url);
    int re = avformat_open_input(&ic, url, 0, 0);
    if(re != 0)
    {
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf));
        LOGE("FFDemux open %s failed!", url);
        return false;
    }
    LOGI("FFDemux open %s success!", url);

    //读取文件信息
    re = avformat_find_stream_info(ic, 0);
    if(re != 0)
    {
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf));
        LOGE("FFDemux avformat_find_stream_info %s failed!", url);
        return false;
    }
    this->totalMs = ic->duration/(AV_TIME_BASE/1000);
    LOGI("total ms = %d", totalMs);
    return true;
}

//读取一帧数据，数据有调用者清理
XData FFDemux::Read()
{
    XData d;
    
    return d;
}

FFDemux::FFDemux()
{
    static bool isFirst = true;
    if(isFirst)
    {
        isFirst = false;
        //注册所有封装器
        av_register_all();
        //注册所有的解码器
        avcodec_register_all();
        //初始化网络
        avformat_network_init();
        LOGI("register ffmpeg");
    }
}
