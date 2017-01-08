#
# Common command rules.
#
include $(BASE_PATH)/build/common.mk

#
# Define the "installed" targets that we want to make.
#
INSTALL_TARGET_LIB := $(LIB_PATH)/$(TARGET_LIB)

#
# all rule.
#
.PHONY: all

all: $(INSTALL_TARGET_LIB)

$(INSTALL_TARGET_LIB): $(TARGET_LIB)
	$(INSTALL) $(INSTALLFLAGS) $(patsubst $(LIB_PATH)/%,%,$@) $(LIB_PATH)

$(TARGET_LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

-include $(OBJS:.o=.d)

%.o : %.cpp
	$(CCPP) $(CPPFLAGS) -MD -c $< -o $@

#
# clean rule.
#
.PHONY: clean

clean:
	$(RM) -f $(TARGET_LIB)
	$(RM) -f $(INSTALL_TARGET_LIB)
	$(RM) -f *.ii *.s *.o *.d

#
# realclean rule.
#
.PHONY: realclean

realclean: clean
	$(RM) -f *~

