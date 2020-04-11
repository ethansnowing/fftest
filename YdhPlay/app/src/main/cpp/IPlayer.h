//
// Created by 余德怀 on 2020/1/12.
//

#ifndef YDHPLAY_IPLAYER_H
#define YDHPLAY_IPLAYER_H


#include <mutex>
#include "XThread.h"
#include "XParameter.h"

class IDemux;
class IDecode;
class IResample;
class IVideoView;
class IAudioPlay;

class IPlayer: public XThread {
public:
    static IPlayer *Get(unsigned char index=0);
    virtual bool Open(const char *path);
    virtual void Close();
    virtual bool Start();
    virtual void InitView(void *win);
    virtual double PlayPos();       //获取当前的播放进度 0.0-1.0
    virtual bool Seek(double pos);
    bool isHardDecode = true;   //由外部用户控制，是否启用硬解码
    XParameter outPara;     //由外部用户控制，是否传入音频播放参数

    IDemux *demux = 0;
    IDecode *vdecode = 0;
    IDecode *adecode = 0;
    IResample *resample = 0;
    IVideoView *videoView = 0;
    IAudioPlay *audioPlay = 0;

protected:
    //用作音视频同步
    void Main();
    std::mutex mux;
    IPlayer(){};

};


#endif //YDHPLAY_IPLAYER_H
