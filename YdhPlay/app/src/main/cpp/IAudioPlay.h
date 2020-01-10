//
// Created by 余德怀 on 2020/1/10.
//

#ifndef YDHPLAY_IAUDIOPLAY_H
#define YDHPLAY_IAUDIOPLAY_H


#include "IObserver.h"
#include "XParameter.h"

class IAudioPlay: public IObserver {
public:
    virtual void Update(XData data);
    virtual bool StartPlay(XParameter out) = 0;

};


#endif //YDHPLAY_IAUDIOPLAY_H
