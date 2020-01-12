//
// Created by 余德怀 on 2020/1/12.
//

#include "IPlayer.h"
#include "IDemux.h"
#include "IDecode.h"
#include "IResample.h"
#include "XLog.h"

IPlayer *IPlayer::Get(unsigned char index) {
    static IPlayer p[256];
    return &p[index];
}

bool IPlayer::Open(const char *path)
{
    //解封装
    if(!demux || !demux->Open(path))
    {
        LOGE("demux->Open %s failed!", path);
        return false;
    }
    //解码，解码可能不需要，解封装之后就是原始数据
    if(!vdecode || !vdecode->Open(demux->GetVPara(), isHardDecode))
    {
        LOGE("vdecode->Open %s failed!", path);
//        return false;
    }
    if(!adecode || !adecode->Open(demux->GetAPara()))
    {
        LOGE("adecode->Open %s failed!", path);
//        return false;
    }
    //重采样，有可能不需要，解码后或者解封装后可能就是原始播放数据
    XParameter outPara = demux->GetAPara();
    if(!resample || !resample->Open(demux->GetAPara(),outPara))
    {
        LOGE("resample->Open %s failed!", path);
//        return false;
    }

    return true;
}

bool IPlayer::Start()
{

    return true;
}

