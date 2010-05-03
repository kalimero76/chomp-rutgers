CXX := g++
CXXFLAGS := -O3 -m64 -Wall -I../../include/ -I../../source/ -I../../../capd/include/ -I./

CXX_STANDALONE := $(CXX) $(CXXFLAGS)

all: example

EXAMPLE_OBJECTS := example.o

example: $(EXAMPLE_OBJECTS)
	$(CXX_STANDALONE) $(EXAMPLE_OBJECTS) -o example

example.o: 

# Cleanup

.PHONY: clean
clean:
	rm *.o
