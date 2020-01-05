//
// Created by 余德怀 on 2020/1/5.
//

#ifndef YDHPLAY_IDECODE_H
#define YDHPLAY_IDECODE_H


#include "XParameter.h"

//解码接口，支持硬解码
class IDecode {
public:
    //打开解码器
    virtual bool Open(XParameter para) = 0;

};


#endif //YDHPLAY_IDECODE_H
