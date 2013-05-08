#ifndef __JNI_UTIL_H
#define __JNI_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

int jniRegisterNativeMethods(JNIEnv* env,
		const char* className,
		const JNINativeMethod* gMethods,
		int numMethods);

int jniThrowException(JNIEnv* env,
		const char* className,
		const char* msg);

JNIEnv* getJNIEnv();

#ifdef __cplusplus
}
#endif

#endif /* __JNI_UTIL_H */
