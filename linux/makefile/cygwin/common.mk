#compile for demos application
#common.mk

TARGET_DIR  := $(BASE_DIR)/bin
LIB_DIR 	:= $(BASE_DIR)/lib
INCLUDE_DIR := $(BASE_DIR)/include
STATICLIB_DIR := $(BASE_DIR)/lib

RM          := rm -f
CC          := gcc
CXX         := g++
AS          := as
LD          := ld
NM          := nm
RANDLIB     := ranlib
OBJCOPY     := objcopy
CFLAGS      := -Werror -Wall -c -g
LDFLAGS     := -lpthread
INCLUDES    := -I$(INCLUDE_DIR)
