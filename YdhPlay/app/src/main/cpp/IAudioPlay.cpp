//
// Created by 余德怀 on 2020/1/10.
//

#include "IAudioPlay.h"
#include "XLog.h"

void IAudioPlay::Update(XData data)
{
    LOGI("IAudioPlay::Update size is %d", data.size);
    //压入缓冲队列
    if(data.size<=0 || !data.data) return;
    while(!isExit)
    {
        framesMutex.lock();
        if(frames.size() >= maxFrame)
        {
            framesMutex.unlock();
            XSleep(1);
            continue;
        }
        frames.push_back(data);
        framesMutex.unlock();
        break;
    }
}
