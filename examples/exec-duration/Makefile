# Absolute path to directory containing this Makefile
root := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

bin := $(notdir $(root))
obj := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

CFLAGS += -I../../src -g -Wall
CXXFLAGS += $(CFLAGS) -std=gnu++17

.PHONY: all clean
all: $(bin)

clean:
	@rm -rvf $(obj) $(bin)

$(bin): $(obj)
	$(CXX) $(LDFLAGS) -o $@ $^
