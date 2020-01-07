//
// Created by 余德怀 on 2020/1/7.
//

#include "XTexture.h"
#include "XLog.h"
#include "XEGL.h"
#include "XShader.h"

class CXTexture:public XTexture
{
public:
    XShader sh;
    virtual bool Init(void *win)
    {
        if(!win)
        {
            LOGE("XTexture Init failed win is NULL!");
            return true;
        }
        if(!XEGL::Get()->Init(win)) return false;
        sh.Init();

        return true;
    }
};

XTexture *XTexture::Create() {
    return new CXTexture();
}
