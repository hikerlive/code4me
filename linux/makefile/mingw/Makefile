# a demo makefile for multi projects orgnization

BASE_DIR = .
APPS	:= libmath math

include common.mk

SUBDIRS := $(APPS)

all:
	@echo "sync include files..."
	@cp libmath/*.h include/
	@for dir in $(SUBDIRS); do echo "<< Building $$dir"; (make -C $$dir) || exit 1; done

clean:
	@for dir in $(SUBDIRS); do echo "<< Cleanging $$dir"; make -C $$dir clean; done
	@rm -f $(LIB_DIR)/*
	@rm -rf $(TARGET_DIR)/*


