//
// Created by 余德怀 on 2020/1/5.
//

#include "IDecode.h"
#include "XLog.h"

//由主体notify的数据
void IDecode::Update(XData pkt)
{
    if(pkt.isAudio != isAudio)
    {
        return;
    }
    while(!isExit)
    {
        packsMutex.lock();

        //阻塞
        if(packs.size() < maxList)
        {
            //生产者
            packs.push_back(pkt);
            packsMutex.unlock();
            break;
        }
        packsMutex.unlock();
        XSleep(1);
    }

}

void IDecode::Clear()
{
    packsMutex.lock();
    while(!packs.empty())
    {
        packs.front().Drop();
        packs.pop_front();
    }
    pts = 0;
    synPts = 0;
    packsMutex.unlock();
}

void IDecode::Main()
{
    while(!isExit)
    {
        packsMutex.lock();

        //判断音视频同步
        if(!isAudio && synPts>0)
        {
            if(synPts < pts)
            {
                packsMutex.unlock();
                XSleep(1);
                continue;
            }
        }


        if(packs.empty())
        {
            packsMutex.unlock();
            XSleep(1);
            continue;
        }
        //取出packet
        XData pack = packs.front();
        packs.pop_front();

        //发送数据到解码线程，一个数据包，可能解码多个结果
        if(this->SendPacket(pack))
        {
            while(!isExit)
            {
                //获得解码数据
                XData frame = RecvFrame();
                if(!frame.data) break;
//                LOGE("RecvFrame %d", frame.size);
                pts = frame.pts;
                //发送数据给观察者
                this->Notify(frame);
            }
        }
        pack.Drop();
        packsMutex.unlock();
    }

}