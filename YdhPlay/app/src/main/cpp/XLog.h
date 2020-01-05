//
// Created by 余德怀 on 2020/1/5.
//

#ifndef YDHPLAY_XLOG_H
#define YDHPLAY_XLOG_H



class XLog {

};
#ifdef ANDROID
#include <android/log.h>
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOG_TAG "ydhplay"
#else
#define  LOGD(...)  printf(LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  printf(LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  printf(LOG_TAG, __VA_ARGS__)
#define LOG_TAG "ydhplay"
#endif



#endif //YDHPLAY_XLOG_H
