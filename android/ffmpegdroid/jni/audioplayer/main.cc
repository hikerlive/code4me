#include <stdlib.h>
#include <jni.h>
#include "jnitool.h"
#include "jnilog.h"

void ffmpeg_player_prepare(JNIEnv* env, jobject obj) {

}

void ffmpeg_player_start(JNIEnv* env, jobject obj) {

}

void ffmpeg_player_stop(JNIEnv* env, jobject obj) {

}

void ffmpeg_player_setPlayUrl(JNIEnv* env, jobject obj, jstring url) {

}

static JavaVM *sJavaVM;

/*
 * JNI registration
 */
static JNINativeMethod methods[] = {
  {"_start", "()V", (void*) ffmpeg_player_start},
  {"_stop", "()V", (void*) ffmpeg_player_stop},
  {"_prepare", "()V", (void*) ffmpeg_player_prepare},
};

JNIEnv* getJNIEnv() {
	JNIEnv* env = NULL;
	if (sJavaVM->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		LOGI("Failed to obtain JNIEnv");
		return NULL;
	}
	return env;
}

/**
 * JNI load entry
 */
jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	JNIEnv* env = NULL;
	jint result = JNI_ERR;
	sJavaVM = vm;
	int ret = -1;

	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		LOGI("GetEnv failed!");
		return result;
	}

	/* add register android native method here */
	ret = jniRegisterNativeMethods(env,
			"com/sspace/cn/media/FFmpegPlayer",
			methods,
			sizeof(methods) / sizeof(methods[0])
			);
	if (ret != 0) {
		goto end;
	}

	result = JNI_VERSION_1_4;

end:
	return result;
}
