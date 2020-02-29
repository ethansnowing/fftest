//
// Created by 余德怀 on 2020/1/7.
//

#ifndef YDHPLAY_XEGL_H
#define YDHPLAY_XEGL_H


class XEGL {
public:
    virtual bool Init(void *win) = 0;
    virtual void Close() = 0;
    virtual void Draw() = 0;
    static XEGL *Get();

protected:
    XEGL(){}

};


#endif //YDHPLAY_XEGL_H
