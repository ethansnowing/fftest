//
// Created by 余德怀 on 2020/1/8.
//

#include "IResample.h"
#include "XLog.h"

void IResample::Update(XData data)
{
    XData d = this->Resample(data);
//    LOGI("IResample::Update data size is %d",d.size);
    if(d.size > 0)
    {
        this->Notify(d);
    }
}
