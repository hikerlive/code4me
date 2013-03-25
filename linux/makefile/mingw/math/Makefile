#compile testmath project

BASE_DIR=..

include $(BASE_DIR)/common.mk

OBJS 	= test_math.o
BIN 	= test_math.exe

CFLAGS 	+= -Wno-unused -Wno-write-strings
LDFLAGS += -L$(LIB_DIR)
LIBS	:= -lmath

all:$(BIN)

include $(BASE_DIR)/rules.mk
