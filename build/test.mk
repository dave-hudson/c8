#
# Common command rules.
#
include $(BASE_PATH)/build/common.mk

#
# Host-specific options
#
ifeq ($(UNAME),Linux)
    LDFLAGS_OS := -Wl,-rpath $(LIB_PATH)
endif

#
# all rule.
#
.PHONY: all

all: $(TARGET_BIN)

LIB_DEPS1 := $(patsubst -l%,lib%.a,$(LIBS))
LIB_DEPS := $(patsubst %,$(LIB_PATH)/%,$(LIB_DEPS1))

$(TARGET_BIN): $(OBJS) $(LIB_DEPS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) -L$(LIB_PATH) -L/usr/local/lib $(LIBS) $(LIBS_EXTRA)

-include $(OBJS:.o=.d)

%.o : %.cpp
	$(CCPP) $(CPPFLAGS) -MD -c $< -o $@

#
# clean rule.
#
.PHONY: clean

clean:
	$(RM) -f $(TARGET_BIN)
	$(RM) -f *.ii *.s *.o *.d

#
# realclean rule.
#
.PHONY: realclean

realclean: clean
	$(RM) -f *~

