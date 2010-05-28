CXX := g++
CXXFLAGS := -O3 -m64 -Wall -I../../include/ -I../../source/ -I../../../capd/include/ -I./ -ggdb

CXX_STANDALONE := $(CXX) $(CXXFLAGS)

all: test

OBJECTS := test.o

test: $(OBJECTS)
	$(CXX_STANDALONE) $(OBJECTS) -o test

test.o: 

# Cleanup

.PHONY: clean
clean:
	rm *.o
