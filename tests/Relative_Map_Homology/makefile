CXX := g++
CXXFLAGS := -O0 -Wall -I../../include/ -I../../source/ -I../../../capd/include/ -I../../../conley-morse-database/include/ -I./ -ggdb

CXX_STANDALONE := $(CXX) $(CXXFLAGS)

all: test

test: test.o
	$(CXX_STANDALONE) test.o -o test

test.o:

# Cleanup

.PHONY: clean
clean:
	rm *.o
