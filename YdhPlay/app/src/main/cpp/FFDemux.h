//
// Created by 余德怀 on 2020/1/5.
//

#ifndef YDHPLAY_FFDEMUX_H
#define YDHPLAY_FFDEMUX_H


#include "IDemux.h"
#include <mutex>
struct AVFormatContext;

class FFDemux: public IDemux {
public:
    //打开文件，或者流媒体 rtmp http rtsp
    virtual bool Open(const char *url);
    virtual void Close();
    //seek操作
    bool Seek(double pos);
    //获取视频参数
    virtual XParameter GetVPara();

    //获取音频参数
    virtual XParameter GetAPara();

    //读取一帧数据，数据有调用者清理
    virtual XData Read();

    FFDemux();

private:
    AVFormatContext *ic = 0;
    std::mutex mux;
    int audioStream = 1;
    int videoStream = 0;

};


#endif //YDHPLAY_FFDEMUX_H
