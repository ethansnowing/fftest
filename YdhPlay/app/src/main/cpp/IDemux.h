//
// Created by 余德怀 on 2020/1/5.
//

#ifndef YDHPLAY_IDEMUX_H
#define YDHPLAY_IDEMUX_H

#include "XData.h"
#include "XThread.h"

class IDemux: public XThread {
public:
    //打开文件，或者流媒体 rtmp http rtsp
    virtual bool Open(const char *url) = 0;

    //读取一帧数据，数据有调用者清理
    virtual XData Read() = 0;

    //总时长(ms)
    int totalMs = 0;

protected:
    virtual void Main();

};


#endif //YDHPLAY_IDEMUX_H
