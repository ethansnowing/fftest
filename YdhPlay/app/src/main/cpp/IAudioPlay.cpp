//
// Created by 余德怀 on 2020/1/10.
//

#include "IAudioPlay.h"
#include "XLog.h"

void IAudioPlay::Clear()
{
    framesMutex.lock();
    while(!frames.empty())
    {
        frames.front().Drop();
        frames.pop_front();
    }
    framesMutex.unlock();
}

XData IAudioPlay::GetData()
{
    XData d;

    while(!isExit)
    {
        framesMutex.lock();
        if(!frames.empty())
        {
            //有数据返回
            d = frames.front();
            frames.pop_front();
            pts = d.pts;        //将音频的pts传递出去
            framesMutex.unlock();
            return d;
        }
        framesMutex.unlock();
        XSleep(1);
    }
    //没有拿到音频数据
    return d;
}

void IAudioPlay::Update(XData data)
{
//    LOGI("IAudioPlay::Update size is %d", data.size);
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
