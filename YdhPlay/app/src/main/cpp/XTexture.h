//
// Created by 余德怀 on 2020/1/7.
//

#ifndef YDHPLAY_XTEXTURE_H
#define YDHPLAY_XTEXTURE_H

enum XTextureType
{
    XTEXTURE_YUV420P = 0,       //y 4 u 1 v 1
    XTEXTURE_NV12 = 25,     //y4 uv 1
    XTEXTURE_NV21 = 26      //y4 vu 1
};

class XTexture {
public:
    static XTexture *Create();
    virtual bool Init(void *win, XTextureType type=XTEXTURE_YUV420P) = 0;
    virtual void Draw(unsigned char *data[], int width, int height) = 0;

};


#endif //YDHPLAY_XTEXTURE_H
