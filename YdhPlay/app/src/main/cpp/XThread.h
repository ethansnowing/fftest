//
// Created by 余德怀 on 2020/1/5.
//

#ifndef YDHPLAY_XTHREAD_H
#define YDHPLAY_XTHREAD_H

//sleep 毫秒
void XSleep(int mis);

//C++ 11 线程库
class XThread
{
public:
    //启动线程
    virtual void Start();

    //通过控制isExit安全停止线程(不一定成功)
    virtual void Stop();

    //入口主函数
    virtual void Main() {}

protected:
    bool isExit = false;
    bool isRuning = false;

private:
    void ThreadMain();
};


#endif //YDHPLAY_XTHREAD_H
