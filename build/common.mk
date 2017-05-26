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
CCPP := clang++
CPPFLAGS = \
    -fPIC \
    -std=c++14 \
    -fno-stack-protector \
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
LD := clang++
LDFLAGS = $(LDFLAGS_OS) $(LDFLAGS_EXTRA)
PANDOC := pandoc --from markdown_github --to html --standalone -c $(ABS_PATH)/doc/github-pandoc.css
RM := rm

# CPPFLAGS += -fsanitize=address
# LDFLAGS += -fsanitize=address

#
# Create a list of object files from source files.
#
OBJS := $(patsubst %.cpp,%.o,$(filter %.cpp,$(SRCS)))

