#ifndef __JNILOG_H
#define __JNILOG_H

#include <android/log.h>

#define LOG_TAG "libffmpegplayer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#endif /* __JNILOG_H */
