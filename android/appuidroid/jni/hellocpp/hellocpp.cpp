/**
 * @author gonggai
 * test vector
 */

#include <string.h>
#include <jni.h>
#include <android/log.h>

// #include <iostream>
#include <vector>
#include <string>

#define LOG_TAG		"hellcpp"
#define LOGI(...)	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...)	__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

void Java_com_example_appuidroid_JniCallerActivity_stringFromJNI(JNIEnv* env,
		jobject thiz) {
	std::vector<std::string> vec;
}


#ifdef __cplusplus
}
#endif
