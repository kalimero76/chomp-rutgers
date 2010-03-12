CXX := g++
CXXFLAGS := -O3 -m64 -Wall -I../../include/ -I../../source/ -I../../../capd/include/ -I./

CXX_STANDALONE := $(CXX) $(CXXFLAGS)

all: generator

GENERATOR_OBJECTS := generators.o

generator: $(GENERATOR_OBJECTS)
	$(CXX_STANDALONE) $(GENERATOR_OBJECTS) -o generator

generators.o: 

# Cleanup

.PHONY: clean
clean:
	rm *.o
