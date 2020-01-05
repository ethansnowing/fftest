//
// Created by 余德怀 on 2020/1/5.
//

#include "FFDecode.h"

bool FFDecode::Open(XParameter para)
{
    if(!para.para) return false;
    AVCodecParameters *p = para.para;
    return true;
}
