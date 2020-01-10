//
// Created by 余德怀 on 2020/1/10.
//

#ifndef YDHPLAY_IAUDIOPLAY_H
#define YDHPLAY_IAUDIOPLAY_H


#include <list>
#include "IObserver.h"
#include "XParameter.h"

class IAudioPlay: public IObserver {
public:
    //缓冲满以后阻塞
    virtual void Update(XData data);
    virtual bool StartPlay(XParameter out) = 0;
    //最大缓冲
    int maxFrame = 100;

protected:
    std::list <XData> frames;
    std::mutex framesMutex;

};


#endif //YDHPLAY_IAUDIOPLAY_H
