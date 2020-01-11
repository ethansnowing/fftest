//
// Created by 余德怀 on 2020/1/10.
//

#ifndef YDHPLAY_SLAUDIOPLAY_H
#define YDHPLAY_SLAUDIOPLAY_H


#include "IAudioPlay.h"

class SLAudioPlay : public IAudioPlay{
public:
    virtual bool StartPlay(XParameter out);
    void PlayCall(void *bufq);

    SLAudioPlay();
    virtual ~SLAudioPlay();

protected:
    unsigned char *buf = 0;

};


#endif //YDHPLAY_SLAUDIOPLAY_H
