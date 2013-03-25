# compile areacode project

BASE_DIR=.

# comiple common.mk
TARGET_DIR	:= $(BASE_DIR)/
LIB_DIR		:= $(BASE_DIR)/lib
INCLUDE_DIR := $(BASE_DIR)/include
STATICLIB_DIR := $(BASE_DIR)/lib

RM		:= rm -f
CC		:= gcc
CXX		:= g++
AS		:= as
LD		:= ld
CFLAGS  := -Werror -Wall -c -g
LDFLAGS := -lpthread

# project files
OBJS	= areacode.o strtest.o binary.o main.o
BIN		= areacode.exe

CFLAGS  += -Wno-unused -Wno-write-strings


all:$(BIN)

# compile rule.mk	
$(OBJS):%.o: %.c
	@echo "(CC) $< -> $@"
	@$(CC) $(CFLAGS) $(INCLUDES) -MD -o $@ $<

$(BIN): $(OBJS)
	@echo "(LD) $@"
	@$(CC) $(LDFLAGS) -o $(TARGET_DIR)/$(BIN) $(OBJS) $(LIBS)

clean:
	@rm -f $(OBJS)
	@rm -f $(INCLUDE_DIR)/*.h
	@for dir in $(SUBIDRS); do echo "<<cleaning $$dir"; make -C $$dir clean; done 
