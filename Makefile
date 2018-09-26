# Build for linux by default
PLATFORM := LINUX

# Compilation settings
INC     := -Iinclude
CFLAGS  := -std=gnu11 -Wall -Wextra -O3 -march=sandybridge -flto -Iinclude
LDFLAGS := -shared -Lsrc

CC     := gcc
CXX	   := g++
CFLAGS += -fPIC
CXXFLAGS = -std=c++11 -O3 -pipe -DNDEBUG -Wparentheses -Wreturn-type -Wpointer-arith -Wwrite-strings -Wno-unknown-pragmas -Wno-long-long -fPIC -DLINUX -D__USE_LONG_INTEGERS__ -D__USE_LONG_FINTS__ -DSOLVER_NONE -D__NO_COPYRIGHT__ -pthread -g
LIBS   := -Lsrc -lm -ldl -Wl,--whole-archive -lagilitycassie -Wl,--no-whole-archive
LIBOUT := libcassiemujoco.so

MAINOBJS = src/udp.o
CYGPATH_W = echo

# Default build target
all: udp_test

clean:
	rm -f udp_test
	rm -f $(MAINOBJS)
	rm -rf build/
	rm -rf test/

udp_test: $(MAINOBJS)
	bla=;\
	for file in $(MAINOBJS); do bla="$$bla `$(CYGPATH_W) $$file`"; done; \
	$(CXX) mainUDPTest.cpp $(CXXFLAGS) $(INC) -o udp_test $$bla $(LIBS)

# Virtual targets
.PHONY: all clean udp_test
