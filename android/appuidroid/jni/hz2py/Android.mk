# compile hz2py.c

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE	:= hz2py
LOCAL_SRC_FILES := hz2py.c sortkey3.cpp hz2pyjni.c
LOCAL_LDLIBS    := -llog

include $(BUILD_SHARED_LIBRARY)
