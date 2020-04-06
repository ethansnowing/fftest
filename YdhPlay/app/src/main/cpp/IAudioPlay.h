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
    //获取缓冲数据，如果没有则阻塞
    virtual XData GetData();
    virtual bool StartPlay(XParameter out) = 0;
    virtual void Close() = 0;
    virtual void Clear();

    int maxFrame = 100;        //最大缓冲
    int pts = 0;

protected:
    std::list <XData> frames;
    std::mutex framesMutex;

};


#endif //YDHPLAY_IAUDIOPLAY_H
