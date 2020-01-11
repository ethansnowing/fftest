//
// Created by 余德怀 on 2020/1/7.
//

#include "GLVideoView.h"
#include "XTexture.h"
#include "XLog.h"

void GLVideoView::SetRender(void *win)
{
    view = win;

}

void GLVideoView::Render(XData data)
{
    if(!view) return;
    if(!txt)
    {
        txt = XTexture::Create();
        txt->Init(view, (XTextureType)data.format);
    }
    txt->Draw(data.datas, data.width, data.height);
}
