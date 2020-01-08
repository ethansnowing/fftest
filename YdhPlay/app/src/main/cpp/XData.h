//
// Created by 余德怀 on 2020/1/5.
//

#ifndef YDHPLAY_XDATA_H
#define YDHPLAY_XDATA_H


struct XData {
    unsigned char *data = 0;
    unsigned char *datas[8] = {0};
    int size = 0;
    int width = 0;
    int height = 0;
    bool isAudio = false;
    void Drop();

};


#endif //YDHPLAY_XDATA_H
