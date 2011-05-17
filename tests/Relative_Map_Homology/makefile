CXX := g++
CXXFLAGS := -O3 -Wall -I../../include/ -I../../source/ -I../../../capd/include/ -I../../../conley-morse-database/include/ -I./ -ggdb -lX11 -L/usr/X11/lib/

CXX_STANDALONE := $(CXX) $(CXXFLAGS)

all: test

test: test.o
	$(CXX_STANDALONE) test.o -o test

test.o:

# Cleanup

.PHONY: clean
clean:
	rm *.o
