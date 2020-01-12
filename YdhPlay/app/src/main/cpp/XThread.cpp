//
// Created by 余德怀 on 2020/1/5.
//

#include "XThread.h"
#include "XLog.h"

#include <thread>
using namespace std;
void XSleep(int mis)
{
    chrono::milliseconds du(mis);
    this_thread::sleep_for(du);
}

//启动线程
bool XThread::Start()
{
    isExit = false;
    thread th(&XThread::ThreadMain, this);
    th.detach();
    return true;
}

void XThread::ThreadMain()
{
    isRuning = true;
    LOGI("线程函数进入");
    Main();
    LOGI("线程函数退出");
    isRuning = false;
}
//通过控制isExit安全停止线程(不一定成功)
void XThread::Stop()
{
    isExit = true;
    for(int i =0; i<200; i++)
    {
        if(!isRuning)
        {
            LOGI("Stop 停止线程成功！");
            return;
        }
        XSleep(1);
    }
    LOGI("Stop 停止线程超时！");

}
