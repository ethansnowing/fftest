//
// Created by 余德怀 on 2020/1/12.
//

#ifndef YDHPLAY_IPLAYERPORXY_H
#define YDHPLAY_IPLAYERPORXY_H


#include <mutex>
#include "IPlayer.h"

class IPlayerPorxy: public IPlayer
{public:
    static IPlayerPorxy *Get()
    {
        static IPlayerPorxy px;
        return &px;
    }
    void Init(void *vm=0);
    virtual bool Open(const char *path);
    virtual void Close();
    virtual bool Start();
    virtual void InitView(void *win);
    virtual double PlayPos();       //获取当前的播放进度 0.0-1.0
    virtual bool Seek(double pos);

protected:
    IPlayerPorxy(){}
    IPlayer *player = 0;
    std::mutex mux;
};


#endif //YDHPLAY_IPLAYERPORXY_H
