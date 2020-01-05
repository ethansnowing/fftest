//
// Created by 余德怀 on 2020/1/5.
//

#ifndef YDHPLAY_FFDEMUX_H
#define YDHPLAY_FFDEMUX_H


#include "IDemux.h"
struct AVFormatContext;

class FFDemux: public IDemux {
public:
    //打开文件，或者流媒体 rtmp http rtsp
    virtual bool Open(const char *url);

    //获取视频参数
    virtual XParameter GetVPara();

    //读取一帧数据，数据有调用者清理
    virtual XData Read();

    FFDemux();

private:
    AVFormatContext *ic = 0;

};


#endif //YDHPLAY_FFDEMUX_H
