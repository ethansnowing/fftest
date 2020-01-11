//
// Created by 余德怀 on 2020/1/7.
//

#ifndef YDHPLAY_XSHADER_H
#define YDHPLAY_XSHADER_H


enum XShaderType
{
    XSHADER_YUV420P = 0,       //y 4 u 1 v 1   手机软解码和虚拟机
    XSHADER_NV12 = 25,     //y4 uv 1   手机硬解码
    XSHADER_NV21 = 26      //y4 vu 1
};

class XShader {
public:
    virtual bool Init(XShaderType type=XSHADER_YUV420P);
    //获取材质并映射到内存
    virtual void GetTexture(unsigned int index, int width, int height, unsigned char *buf, bool isa=false);
    virtual void Draw();

protected:
    unsigned int vsh = 0;
    unsigned int fsh = 0;
    unsigned int program = 0;
    unsigned int texts[100] = {0};

};


#endif //YDHPLAY_XSHADER_H
