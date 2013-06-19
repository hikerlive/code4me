# compile hellojni.cpp

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE	:= hellocpp
LOCAL_SRC_FILES	:= hellocpp.cpp
LOCAL_LDLIBS	:= -llog

include $(BUILD_SHARED_LIBRARY)