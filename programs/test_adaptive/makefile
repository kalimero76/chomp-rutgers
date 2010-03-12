CXX := g++
CXXFLAGS := -O3 -m64 -Wall -I../../include/ -I../../source/ -I../../../capd/include/ -I./

CXX_STANDALONE := $(CXX) $(CXXFLAGS)

all: test

TEST_OBJECTS := AdaptTest.o

test: $(TEST_OBJECTS)
	$(CXX_STANDALONE) $(TEST_OBJECTS) -o test

AdaptTest.o: 

# Cleanup

.PHONY: clean
clean:
	rm *.o
