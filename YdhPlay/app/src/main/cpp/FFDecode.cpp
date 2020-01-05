//
// Created by 余德怀 on 2020/1/5.
//
extern "C"
{
#include <libavcodec/avcodec.h>
}
#include "FFDecode.h"
#include "XLog.h"

bool FFDecode::Open(XParameter para)
{
    if(!para.para) return false;
    AVCodecParameters *p = para.para;

    //1 查找解码器
    AVCodec *cd = avcodec_find_decoder(p->codec_id);
    if(!cd)
    {
        LOGE("avcodec_find_decoder %d failed!", p->codec_id);
        return false;
    }
    LOGI("avcodec_find_decoder success!");
    //2 创建解码上下文，并复制参数
    codec = avcodec_alloc_context3(cd);
    avcodec_parameters_to_context(codec, p);
    //3 打开解码器
    int re = avcodec_open2(codec, 0, 0);
    if(re !=0)
    {
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf)-1);
        LOGE("%s", buf);
        return false;
    }
    LOGI("avcodec_open2 success!");


    return true;
}
