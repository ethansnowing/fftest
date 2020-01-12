//
// Created by 余德怀 on 2020/1/12.
//

#ifndef YDHPLAY_IPLAYERBUILDER_H
#define YDHPLAY_IPLAYERBUILDER_H

#include "IPlayer.h"


class IPlayerBuilder {
public:
    virtual IPlayer *BuilderPlayer(unsigned char index=0);

protected:
    virtual IDemux *CreateDemux() = 0;
    virtual IDecode *CreateDecode() = 0;
    virtual IResample *CreateResample() = 0;
    virtual IVideoView *CreateVideoView() =0;
    virtual  IAudioPlay *CreateAudioPlay() = 0;
    virtual IPlayer *CreatePlayer(unsigned char index=0) = 0;

};


#endif //YDHPLAY_IPLAYERBUILDER_H
