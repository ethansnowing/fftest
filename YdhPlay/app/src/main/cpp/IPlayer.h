//
// Created by 余德怀 on 2020/1/12.
//

#ifndef YDHPLAY_IPLAYER_H
#define YDHPLAY_IPLAYER_H


#include "XThread.h"
class IDemux;
class IDecode;
class IResample;
class IVideoView;
class IAudioPlay;

class IPlayer: public XThread {
public:
    static IPlayer *Get(unsigned char index=0);
    virtual bool Open(const char *path);
    virtual bool Start();
    bool isHardDecode = true;

    IDemux *demux = 0;
    IDecode *vdecode = 0;
    IDecode *adecode = 0;
    IResample *resample = 0;
    IVideoView *videoView = 0;
    IAudioPlay *audioPlay = 0;

protected:
    IPlayer(){};

};


#endif //YDHPLAY_IPLAYER_H
