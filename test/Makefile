TMPDIR=/Volumes/RaidRamDisk/
PROJECT_ROOT=..
GMOCK_LIB=$(PROJECT_ROOT)/libgmock.a $(PROJECT_ROOT)/libgmock_main.a $(PROJECT_ROOT)/libgtest.a
GMOCK_INCLUDE=-I$(GMOCK_PATH)/include -I$(GMOCK_PATH)/gtest/include -I$(PROJECT_ROOT)
CPP11_FLAGS=-g -O0 -std=c++11 -fno-inline -falign-functions=32
SRC=$(wildcard *.cpp)
TARGETS=$(patsubst %.cpp, %, $(SRC))
TARGETS_PATH=$(patsubst %.cpp, $(TMPDIR)/%.out, $(SRC))
RECENT=runtime_patch_test
RECENT_PATH=$(patsubst %, $(TMPDIR)/%.out, $(RECENT))
# Example
%: %.cpp
	g++ $(CPP11_FLAGS) $^ $(GMOCK_INCLUDE) $(GMOCK_LIB) -o $(TMPDIR)/$@.out
# Default
all: $(TARGETS)
# Recent
recent: $(RECENT)
# Test
test: all
	echo $(TARGETS_PATH) | tr " " "\n" | while read test; do echo "Test: $$test"; $$test; done
# Recent test
recent-test: recent
	$(RECENT_PATH)
