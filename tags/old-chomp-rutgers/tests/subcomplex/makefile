CXX := g++
CXXFLAGS := -O3 -m64 -Wall -I../../include/ -I../../source/ -I../../../capd/include/ -I./

CXX_STANDALONE := $(CXX) $(CXXFLAGS)

all: test

OBJECTS := subcomplex_test.o

test: $(OBJECTS)
	$(CXX_STANDALONE) $(OBJECTS) -o test

subcomplex_test.o: 

# Cleanup

.PHONY: clean
clean:
	rm *.o
