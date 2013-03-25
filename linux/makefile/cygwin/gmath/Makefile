BASE_DIR = ..

include $(BASE_DIR)/common.mk

OBJS= gmath.o
BIN	= gmath

CFLAGS 	+= -Wno-unused -Wno-write-strings
LDFLAGS += -L$(LIB_DIR)
LIBS 	:= -lmath

all:$(BIN)

include $(BASE_DIR)/rules.mk


