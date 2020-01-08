//
// Created by 余德怀 on 2020/1/7.
//

#ifndef YDHPLAY_GLVIDEOVIEW_H
#define YDHPLAY_GLVIDEOVIEW_H


#include "XData.h"
#include "IVideoView.h"

class XTexture;

class GLVideoView:public IVideoView {
public:
    virtual void SetRender(void *win);
    virtual void Render(XData data);

protected:
    void *view = 0;
    XTexture *txt = 0;

};


#endif //YDHPLAY_GLVIDEOVIEW_H
