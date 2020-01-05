//
// Created by 余德怀 on 2020/1/5.
//

#include "IDemux.h"
#include "XLog.h"

void IDemux::Main() {
    while(!isExit)
    {
        XData d = Read();
        if(d.size > 0)
        {
            Notify(d);
        }
//        LOGI("IDemux Read data size is %d", d.size);
//        if(d.size<=0) break;
    }
}