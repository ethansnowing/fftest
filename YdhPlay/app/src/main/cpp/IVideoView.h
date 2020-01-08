//
// Created by 余德怀 on 2020/1/7.
//

#ifndef YDHPLAY_IVIDEOVIEW_H
#define YDHPLAY_IVIDEOVIEW_H


#include "XData.h"
#include "IObserver.h"

class IVideoView: public IObserver{
public:
    virtual void SetRender(void *win) = 0;
    virtual void Render(XData data) = 0;
    virtual void Update(XData data);

};


#endif //YDHPLAY_IVIDEOVIEW_H
