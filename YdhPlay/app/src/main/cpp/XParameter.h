//
// Created by 余德怀 on 2020/1/5.
//

#ifndef YDHPLAY_XPARAMETER_H
#define YDHPLAY_XPARAMETER_H


struct AVCodecParameters;

class XParameter {
public:
    AVCodecParameters *para = 0;
    int channels = 2;       //音频通道数
    int sample_rate = 44100;        //音频采样率

};


#endif //YDHPLAY_XPARAMETER_H
