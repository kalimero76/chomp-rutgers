CXX := g++
CXXFLAGS := -O0 -m64 -Wall -I../../include/ -I../../source/ -I../../../capd/include/ -I./ -ggdb

CXX_STANDALONE := $(CXX) $(CXXFLAGS)

all: test

OBJECTS := simplicial_test.o

test: $(OBJECTS)
	$(CXX_STANDALONE) $(OBJECTS) -o test

cubical_test.o: 

# Cleanup

.PHONY: clean
clean:
	rm *.o
