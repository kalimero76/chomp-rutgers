CXX := g++
CXXFLAGS := -O0 -m64 -Wall -I../../include/ -I../../source/ -I../../../capd/include/ -I./ -ggdb -L../../lib/ -Wfatal-errors

CXX_STANDALONE := $(CXX) $(CXXFLAGS) #-lchomp-rutgers

all: test

OBJECTS := test.o

test: $(OBJECTS)
	$(CXX_STANDALONE) $(OBJECTS) -o test

test.o: 

# Cleanup

.PHONY: clean
clean:
	rm *.o
