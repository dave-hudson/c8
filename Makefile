#
# Path to the root of the development system.
#
BASE_PATH := .

#
# Define our subdirectories.
#
SUBDIRS := \
	src \
    test

#
# Dependencies
#
test: src

#
# Actual heavy lifting for the make.
#
include $(BASE_PATH)/build/dir.mk

#
# Special top-level distclean target
#
.PHONY: disclean

distclean: realclean
	$(RM) -f $(BASE_PATH)/lib/*
	$(RM) -f $(BASE_PATH)/include/*

