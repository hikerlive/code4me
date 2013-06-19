#ifndef __JNITOOL_H
#define __JNITOOL_H

#include <jni.h>

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

#ifdef __cplusplus
}
#endif

#endif /* __JNITOOL_H */
