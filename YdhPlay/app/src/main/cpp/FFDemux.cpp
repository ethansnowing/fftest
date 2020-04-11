//
// Created by 余德怀 on 2020/1/5.
//

#include "FFDemux.h"
#include "XLog.h"
extern "C"{
#include <libavformat/avformat.h>
}

//分数转为浮点数
static double r2d(AVRational r)
{
    return r.num==0 || r.den==0 ? 0.:(double)r.num/(double)r.den;
}

void FFDemux::Close()
{
    mux.lock();
    if(ic)
    {
        avformat_close_input(&ic);
    }
    mux.unlock();
}

bool FFDemux::Seek(double pos)
{
    if(pos<0 || pos>1)
    {
        LOGE("seek pos must in 0.0-1.0");
        return false;
    }
    bool re = false;
    mux.lock();
    if(!ic)
    {
        mux.unlock();
        return false;
    }
    //清理读取的缓存
    avformat_flush(ic);
    long long seekPts = 0;
    seekPts = ic->streams[videoStream]->duration * pos;

    //往过去时间跳转到关键帧
    re = av_seek_frame(ic, videoStream, seekPts,AVSEEK_FLAG_FRAME|AVSEEK_FLAG_BACKWARD);
    mux.unlock();

    return re;
}

//打开文件，或者流媒体 rtmp http rtsp
bool FFDemux::Open(const char *url)
{
    LOGI("Open file %s begin", url);
    Close();
    mux.lock();
    int re = avformat_open_input(&ic, url, 0, 0);
    if(re != 0)
    {
        mux.unlock();
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
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf));
        LOGE("FFDemux avformat_find_stream_info %s failed!", url);
        return false;
    }
    this->totalMs = ic->duration/(AV_TIME_BASE/1000);
    LOGI("total ms = %d", totalMs);
    mux.unlock();
    GetVPara();
    GetAPara();
    return true;
}

//获取视频参数
XParameter FFDemux::GetVPara()
{
    mux.lock();
    if(!ic)
    {
        mux.unlock();
        LOGE("GetVPara failed! ic is NULL!");
        return XParameter();
    }
    //获取视频流索引
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, 0,0);
    if(re<0)
    {
        mux.unlock();
        LOGE("av_find_best_stream failed!");
        return XParameter();
    }
    videoStream = re;
    XParameter para;
    para.para = ic->streams[re]->codecpar;
    mux.unlock();
    return para;
}

//获取音频参数
XParameter FFDemux::GetAPara()
{
    mux.lock();
    if(!ic)
    {
        mux.unlock();
        LOGE("GetVPara failed! ic is NULL!");
        return XParameter();
    }
    //获取音频流索引
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, 0,0);
    if(re<0)
    {
        mux.unlock();
        LOGE("av_find_best_stream failed!");
        return XParameter();
    }
    audioStream = re;
    XParameter para;
    para.para = ic->streams[re]->codecpar;
    para.channels = ic->streams[re]->codecpar->channels;
    para.sample_rate = ic->streams[re]->codecpar->sample_rate;
    mux.unlock();
    return para;
}

//读取一帧数据，数据有调用者清理
XData FFDemux::Read()
{
    mux.lock();
    if(!ic)
    {
        mux.unlock();
        return XData();
    }
    XData d;
    AVPacket *pkt = av_packet_alloc();
    int re = av_read_frame(ic, pkt);
    if(re != 0)
    {
        mux.unlock();
        av_packet_free(&pkt);
        return XData();
    }
//    LOGI("pack size is %d, pts is %lld", pkt->size, pkt->pts);
    d.data = (unsigned char *)pkt;
    d.size = pkt->size;
    if(pkt->stream_index == audioStream)
    {
        d.isAudio = true;
    }else if(pkt->stream_index == videoStream){
        d.isAudio = false;
    }else{
        av_packet_free(&pkt);
        mux.unlock();
        return XData();
    }
    //转换pts
    pkt->pts = pkt->pts * (1000 * r2d(ic->streams[pkt->stream_index]->time_base));
    pkt->dts = pkt->dts * (1000 * r2d(ic->streams[pkt->stream_index]->time_base));
    d.pts = (int)pkt->pts;
//    LOGE("demux pts %d", d.pts);

    mux.unlock();
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
