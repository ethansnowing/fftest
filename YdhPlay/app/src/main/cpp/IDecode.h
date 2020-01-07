//
// Created by 余德怀 on 2020/1/5.
//

#ifndef YDHPLAY_IDECODE_H
#define YDHPLAY_IDECODE_H


#include <list>
#include "XParameter.h"
#include "IObserver.h"

//解码接口，支持硬解码
class IDecode:public IObserver{
public:
    //打开解码器
    virtual bool Open(XParameter para) = 0;

    //future模型 发送数据到线程解码
    virtual bool SendPacket(XData pkt) = 0;

    //从线程中获取解码结果
    virtual XData RecvFrame() = 0;

    //由主体notify的数据 阻塞
    virtual void Update(XData pkt);

    bool isAudio = false;

    //最大的队列缓冲
    int maxList = 100;

protected:
    virtual void Main();
    //读取缓冲
    std::list<XData> packs;
    std::mutex packsMutex;

};


#endif //YDHPLAY_IDECODE_H
