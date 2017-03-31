#
# Common command rules.
#
include $(BASE_PATH)/build/common.mk

#
# Create a list of HTML files to create from source files.
#
DOCS := $(patsubst %.md,html/%,$(filter %.md,$(SRCS)))

#
# all rule.
#
.PHONY: all

all: $(DOCS)

html/% : %.md
	$(PANDOC) --output $@ $<

#
# clean rule.
#
.PHONY: clean

clean:
	$(RM) -f html/*

#
# realclean rule.
#
.PHONY: realclean

realclean: clean
	$(RM) -f *~

