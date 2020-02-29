//
// Created by 余德怀 on 2020/1/8.
//

#ifndef YDHPLAY_FFRESAMPLE_H
#define YDHPLAY_FFRESAMPLE_H


#include "IResample.h"
struct SwrContext;

class FFResample: public IResample{
public:
    virtual bool Open(XParameter in ,XParameter out=XParameter());
    virtual void Close();
    virtual XData Resample(XData indata);

protected:
    SwrContext *actx = 0;
    std::mutex mux;
};


#endif //YDHPLAY_FFRESAMPLE_H
