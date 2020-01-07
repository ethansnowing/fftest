//
// Created by 余德怀 on 2020/1/7.
//

#ifndef YDHPLAY_XSHADER_H
#define YDHPLAY_XSHADER_H


class XShader {
public:
    virtual bool Init();

protected:
    unsigned int vsh = 0;
    unsigned int fsh = 0;
    unsigned int program = 0;

};


#endif //YDHPLAY_XSHADER_H
