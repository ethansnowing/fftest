//
// Created by 余德怀 on 2020/1/7.
//

#include "XEGL.h"
class CXEGL:public XEGL
{
public:
    virtual bool Init(void *win)
    {
        return true;
    }
};

XEGL *XEGL::Get()
{
    static CXEGL egl;
    return &egl;
}
