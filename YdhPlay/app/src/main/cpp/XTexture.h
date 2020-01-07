//
// Created by 余德怀 on 2020/1/7.
//

#ifndef YDHPLAY_XTEXTURE_H
#define YDHPLAY_XTEXTURE_H


class XTexture {
public:
    static XTexture *Create();
    virtual bool Init(void *win) = 0;

};


#endif //YDHPLAY_XTEXTURE_H
