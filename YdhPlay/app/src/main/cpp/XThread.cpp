//
// Created by 余德怀 on 2020/1/5.
//

#include "XThread.h"
#include "XLog.h"

#include <thread>
using namespace std;

//启动线程
void XThread::Start()
{
    thread th(&XThread::ThreadMain, this);
    th.detach();
}

void XThread::ThreadMain()
{
    LOGI("线程函数进入");
    Main();
    LOGI("线程函数退出");
}
//通过控制isExit安全停止线程(不一定成功)
void XThread::stop()
{

}
