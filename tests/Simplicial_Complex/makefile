CXX := g++
CXXFLAGS := -O3 -m64 -Wall -I../../include/ -I../../source/  -I/opt/local/include/ -I./ -ggdb -L/opt/local/lib/ -L/usr/X11/lib/

CXX_STANDALONE := $(CXX) $(CXXFLAGS) -lgmpxx -lgmp -lX11

all: test

OBJECTS := simplicial_test.o

test: $(OBJECTS)
	$(CXX_STANDALONE) $(OBJECTS) -o test

cubical_test.o: 

# Cleanup

.PHONY: clean
clean:
	rm *.o
