CXX := g++
CXXFLAGS := -O3 -m64 -Wall -I../../include/ -I../../source/ -I./ -ggdb

CXX_STANDALONE := $(CXX) $(CXXFLAGS)

all: example

EXAMPLE_OBJECTS := chomp.o

example: $(EXAMPLE_OBJECTS)
	$(CXX_STANDALONE) $(EXAMPLE_OBJECTS) -o chomp

chomp.o: 

# Cleanup

.PHONY: clean
clean:
	rm *.o
