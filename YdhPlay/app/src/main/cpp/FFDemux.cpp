//
// Created by 余德怀 on 2020/1/5.
//

#include "FFDemux.h"
#include "XLog.h"

//打开文件，或者流媒体 rtmp http rtsp
bool FFDemux::Open(const char *url)
{
    LOGI("Open file %s begin", url);
    return true;
}

//读取一帧数据，数据有调用者清理
XData FFDemux::Read()
{
    XData d;
    return d;
}
