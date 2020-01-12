//
// Created by 余德怀 on 2020/1/12.
//

#include "IPlayerBuilder.h"
#include "IVideoView.h"
#include "IResample.h"
#include "IDemux.h"
#include "IAudioPlay.h"
#include "IDecode.h"

IPlayer *IPlayerBuilder::BuilderPlayer(unsigned char index)
{
    IPlayer *player = CreatePlayer(index);
    IDemux *de = CreateDemux();     //解封装

    IDecode *vdecode = CreateDecode();      //视频解码
    IDecode *adecode = CreateDecode();      //音频解码
    de->AddObs(vdecode);        //解码器观察解封装
    de->AddObs(adecode);

    //渲染
    IVideoView *view = CreateVideoView();
    vdecode->AddObs(view);      //显示观察视频解码器

    //音频重采样
    IResample *resample = CreateResample();
    adecode->AddObs(resample);      //音频播放观察重采样

    IAudioPlay *audioPlay = CreateAudioPlay();
    resample->AddObs(audioPlay);

    player->demux = de;
    player->adecode = adecode;
    player->vdecode = vdecode;
    player->videoView = view;
    player->resample = resample;
    player->audioPlay = audioPlay;

    return player;
}
