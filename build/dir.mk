MAKE := make
RM := rm

.DEFAULT_GOAL := all

#
# Standard global rules.
#
.PHONY: all
.PHONY: clean

all clean: $(SUBDIRS)

.PHONY: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

SUBDIRS_REALCLEAN := $(patsubst %,%_realclean,$(SUBDIRS))

.PHONY: realclean

realclean: $(SUBDIRS_REALCLEAN)
	$(RM) -f *~
	$(RM) -f perf.data*

.PHONY: $(SUBDIRS_REALCLEAN)

$(SUBDIRS_REALCLEAN):
	$(MAKE) -C $(patsubst %_realclean,%,$@) realclean

