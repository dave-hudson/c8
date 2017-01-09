#
# Path to the root of the development system.
#
BASE_PATH := ..

#
# Define our subdirectories.
#
SUBDIRS := \
    integer \
    natural

#
# Ensure that we reflect the dependencies on the various libraries!
#
integer: natural

#
# Actual heavy lifting for the make.
#
include $(BASE_PATH)/build/dir.mk

