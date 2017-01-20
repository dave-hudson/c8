#
# Common command rules.
#
include $(BASE_PATH)/build/common.mk

#
# all rule.
#
.PHONY: all

all: $(TARGET_BIN)

#
# For our test programs we don't want to use dynamic libraries because static
# linking makes things a little more predictable.
#
LDFLAGS_EXTRA := -static

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

