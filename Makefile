TMPDIR=/Volumes/RaidRamDisk/
GMOCK_LIB=./libgmock.a ./libgmock_main.a ./libgtest.a
GMOCK_INCLUDE=-I$(GMOCK_PATH)/include -I$(GMOCK_PATH)/gtest/include
CPP11_FLAGS=-g -O0 -std=c++11 -fno-inline -falign-functions=32
SRC=$(wildcard *.cpp ./*.cpp)
TARGETS=$(patsubst %.cpp, %, $(SRC))
TARGETS_PATH=$(patsubst %.cpp, $(TMPDIR)/%.out, $(SRC))
RECENT=hello_world
RECENT_PATH=$(patsubst %, $(TMPDIR)/%.out, $(RECENT))
# All
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
