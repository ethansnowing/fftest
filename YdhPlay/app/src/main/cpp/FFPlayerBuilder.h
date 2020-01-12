//
// Created by 余德怀 on 2020/1/12.
//

#ifndef YDHPLAY_FFPLAYERBUILDER_H
#define YDHPLAY_FFPLAYERBUILDER_H

#include "IPlayerBuilder.h"

class FFPlayerBuilder :public IPlayerBuilder{
public:
    static void InitHard(void *vm);
    static FFPlayerBuilder *Get()
    {
        static FFPlayerBuilder ff;
        return &ff;
    }
protected:
    FFPlayerBuilder(){};
    virtual IDemux *CreateDemux();
    virtual IDecode *CreateDecode();
    virtual IResample *CreateResample();
    virtual IVideoView *CreateVideoView();
    virtual  IAudioPlay *CreateAudioPlay();
    virtual IPlayer *CreatePlayer(unsigned char index=0);

};


#endif //YDHPLAY_FFPLAYERBUILDER_H
