#
# Common make variables.
#
ABS_PATH := $(realpath $(shell pwd)/$(BASE_PATH))
INCLUDE_PATH := $(ABS_PATH)/include
LIB_PATH := $(ABS_PATH)/lib

#
# Host-specific options
#
UNAME := $(shell uname -s)

AR := ar
ARFLAGS = rc
#CCPP := g++
CCPP := clang++-3.6
CPPFLAGS = \
    -fPIC \
    -std=c++14 \
    -fno-omit-frame-pointer \
    -g \
    -O2 \
    -Werror \
    -Wall \
    -Wtype-limits \
    -Wconversion \
    -I/usr/local/include \
    -I$(INCLUDE_PATH) \
    $(CPPFLAGS_EXTRA)
INSTALL := install
INSTALLFLAGS =
#LD := g++
LD := clang++-3.6
LDFLAGS = $(LDFLAGS_OS) $(LDFLAGS_EXTRA)
RM := rm

# CPPFLAGS += -fsanitize=address
# LDFLAGS += -fsanitize=address

#
# Create a list of object files from source files.
#
OBJS := $(patsubst %.cpp,%.o,$(filter %.cpp,$(SRCS)))

