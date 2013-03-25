BASE_DIR = .
APPS	:= math gmath hello mathlib thread

include common.mk

SUBDIRS := $(APPS)

all:
	@echo "sync include files..."
	@cp -f math/*.h include/
	@for dir in $(SUBDIRS); do echo "<< Building $$dir"; (make -C $$dir) || exit 1;done

clean:
	@for dir in $(SUBDIRS); do echo "<< Cleaning $$dir"; make -C $$dir clean; done
	@rm -f $(LIB_DIR)/*
	@rm -rf $(TARGET_DIR)/*
