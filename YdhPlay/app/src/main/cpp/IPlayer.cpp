//
// Created by 余德怀 on 2020/1/12.
//

#include "IPlayer.h"
#include "IDemux.h"
#include "IDecode.h"
#include "IResample.h"
#include "IAudioPlay.h"
#include "IVideoView.h"
#include "XLog.h"

IPlayer *IPlayer::Get(unsigned char index) {
    static IPlayer p[256];
    return &p[index];
}

//用作音视频同步
void IPlayer::Main()
{
    while(!isExit)
    {
        mux.lock();
        if(!audioPlay || !vdecode)
        {
            mux.unlock();
            XSleep(2);
            continue;
        }

        //同步
        //获取音频的pts,告诉视频
        int apts = audioPlay->pts;
//        LOGE("apts = %d",apts);
        vdecode->synPts = apts;     //将音频pts传递给vdecode

        mux.unlock();
        XSleep(2);
    }
}

bool IPlayer::Open(const char *path)
{
    mux.lock();
    //解封装
    if(!demux || !demux->Open(path))
    {
        mux.unlock();
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
    if(outPara.sample_rate <= 0)
    {
        outPara = demux->GetAPara();
    }
    //重采样，有可能不需要，解码后或者解封装后可能就是原始播放数据
    if(!resample || !resample->Open(demux->GetAPara(),outPara))
    {
        LOGE("resample->Open %s failed!", path);
//        return false;
    }

    mux.unlock();
    return true;
}

bool IPlayer::Start()
{
    mux.lock();
    if(!demux || !demux->Start())
    {
        mux.unlock();
        LOGE("demux->Start() failed!");
        return false;
    }
    //先启动音频解码
    if(adecode)
    {
        adecode->Start();
    }
    if(audioPlay)
    {
        audioPlay->StartPlay(outPara);
    }
    if(vdecode)
    {
        vdecode->Start();
    }

    XThread::Start();
    mux.unlock();
    return true;
}

void IPlayer::InitView(void *win)
{
    if(videoView)
    {
        videoView->SetRender(win);
    }
}

