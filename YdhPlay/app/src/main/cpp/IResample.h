//
// Created by 余德怀 on 2020/1/8.
//

#ifndef YDHPLAY_IRESAMPLE_H
#define YDHPLAY_IRESAMPLE_H


#include "IObserver.h"
#include "XParameter.h"

class IResample: public IObserver{
public:
    virtual bool Open(XParameter in ,XParameter out=XParameter()) = 0;

};


#endif //YDHPLAY_IRESAMPLE_H
