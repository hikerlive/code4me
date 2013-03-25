#compile thread projects

BASE_DIR = ..
PROJECTS := thread1 thread2 thread3 thread4

include $(BASE_DIR)/common.mk

SUBDIRS := $(PROJECTS)

all:
	@echo "compile thread demo projects..."
	@for dir in $(SUBDIRS); do echo "<< Building $$dir"; (make -C $$dir) || exit 1; done

clean:
	@for dir in $(SUBDIRS); do echo "<< Cleaning $$dir"; make -C $$dir clean; done
	@rm -f $(LIB_DIR)/*
	@rm -rf $(TARGET_DIR)/*
