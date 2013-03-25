#rules.mk

.PHONY: clean

${OBJS}: %.o: %.c
	@echo "(CC) $< -> $@"
	@$(CC) $(CFLAGS) $(INCLUDES) -MD -o $@ $<

$(STATICLIB): $(OBJS)
	@echo "(AR) $@"
	@$(AR) -cr $(STATICLIB_DIR)/$(STATICLIB) $(OBJS)

$(SHAREDLIB): $(OBJS)
	@echo "(LD) $@"
	@$(CC) $(LDFLAGS) -shared -fPIC -o $(LIB_DIR)/$(SHAREDLIB) $(OBJS) $(LIBS)

$(BIN): $(OBJS)
	@echo "(LD) $@"
	@$(CC) $(LDFLAGS) -o $(TARGET_DIR)/$(BIN) $(OBJS) $(LIBS)

clean:
	@rm -f $(OBJS)
	@rm -f $(INCLUDE_DIR)/*.h
	@for dir in $(SUBDIRS); do echo "<<cleaning $$dir"; make -C $$dir clean; done
