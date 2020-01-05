//
// Created by 余德怀 on 2020/1/5.
//

#include "IDemux.h"
#include "XLog.h"

void IDemux::Main() {
    for(;;)
    {
        XData d = Read();
        LOGI("IDemux Read data size is %d", d.size);
        if(d.size<=0) break;
    }
}