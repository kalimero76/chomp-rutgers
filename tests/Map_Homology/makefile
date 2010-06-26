CXX := g++
CXXFLAGS := -O3 -m64 -Wall -I../../include/ -I../../source/ -I../../../capd/include/ -I../../../conley-morse-database/include/ -I./ -ggdb

CXX_STANDALONE := $(CXX) $(CXXFLAGS)

all: test test2

test: test.o
	$(CXX_STANDALONE) test.o -o test

test2: test2.o
	$(CXX_STANDALONE) test2.o -o test2

test.o:

test2.o:

# Cleanup

.PHONY: clean
clean:
	rm *.o
