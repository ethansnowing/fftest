//
// Created by 余德怀 on 2020/1/8.
//
extern "C"
{
#include <libswresample/swresample.h>
#include <libavcodec/avcodec.h>
}

#include "FFResample.h"
#include "XLog.h"

bool FFResample::Open(XParameter in ,XParameter out)
{
    //音频重采样上下文初始化
    actx = swr_alloc();
    actx = swr_alloc_set_opts(actx,
                              av_get_default_channel_layout(out.channels ),
                              AV_SAMPLE_FMT_S16, out.sample_rate,
                              av_get_default_channel_layout(in.para->channels),
                              (AVSampleFormat)in.para->format, in.para->sample_rate,
                              0,0);
    int re = swr_init(actx);
    if(re != 0){
        LOGE("ydh--swr_init failed!");
        return false;
    }else{
        LOGI("ydh--swr_init success!");
    }
    outChannels = in.para->channels;
    outFormat = AV_SAMPLE_FMT_S16;
    return true;

}

XData FFResample::Resample(XData indata)
{
    if(indata.size<=0 || !indata.data) return XData();
    if(!actx) return XData();
    AVFrame *frame = (AVFrame *)indata.data;
//    LOGE("indata size is %d", indata.size);
    //输出空间的分配
    XData out;
    //通道数* 单通道样本数* 样本字节大小
    int outsize = outChannels * frame->nb_samples * av_get_bytes_per_sample((AVSampleFormat)outFormat);
    if(outsize <=0) return XData();
    out.Alloc(outsize);
    uint8_t *outArr[2] = {0};
    outArr[0] = out.data;
    int len = swr_convert(actx,outArr,frame->nb_samples,(const uint8_t **)frame->data,frame->nb_samples);
    if(len<=0)
    {
        out.Drop();
        return XData();
    }
    out.pts = indata.pts;       //将从decode获取的pts传递给audioPlay
//    LOGI("swr_convert success = %d", len);
    return out;
}
