//
// Created by 余德怀 on 2020/1/5.
//

#ifndef YDHPLAY_FFDECODE_H
#define YDHPLAY_FFDECODE_H


#include "XParameter.h"
#include "IDecode.h"
struct AVCodecContext;

class FFDecode:public IDecode{
public:
    virtual bool Open(XParameter para);

protected:
    AVCodecContext *codec = 0;
};


#endif //YDHPLAY_FFDECODE_H
