//
// Created by 余德怀 on 2020/1/5.
//

#include "IObserver.h"

//主体函数 添加观察者
void IObserver::AddObs(IObserver *obs)
{
    if(!obs) return;       //obs为空则不添加观察者
    mux.lock();
    obss.push_back(obs);
    mux.unlock();
}

//通知所有观察者
void IObserver::Notify(XData data)
{
    mux.lock();
    for(int i=0; i<obss.size(); i++)
    {
        obss[i]->Update(data);
    }
    mux.unlock();
}
