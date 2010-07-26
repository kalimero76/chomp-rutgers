# makefile for CHomP project
# this makes the chomp library
# written by Shaun Harker

# CAPD LOCATION
CAPD_PATH := ../capd
BUILD_DIR := ./build

CXX := g++
CXXFLAGS := -O3 -m64 -Wall -I./include/ -I$(CAPD_PATH)/include/

# -m64 means we want to use 64 bit code
# -O3 tells the compiler to try to use its most advanced optimizations
# -Wall tells the compiler to turn on all warnings
# -pg is for the profiler. REMOVE it for a final build.

CXX_STANDALONE := $(CXX) $(CXXFLAGS)

all: library

#Cell_Complex_Archetype.o

ARCHETYPE_OBJECTS := Chain_Archetype.o
ALGORITHM_OBJECTS := 
COMPLEX_OBJECTS := Cubical_Complex.o Adaptive_Complex.o
TOPLEX_OBJECTS := 
# Adaptive_Cubical_Toplex.o

LIBRARY_OBJECTS := $(patsubst %.o, build/archetypes/%.o, $(ARCHETYPE_OBJECTS) )
LIBRARY_OBJECTS += $(patsubst %.o, build/algorithms/%.o, $(ALGORITHM_OBJECTS) )
LIBRARY_OBJECTS += $(patsubst %.o, build/complexes/%.o, $(COMPLEX_OBJECTS) )
LIBRARY_OBJECTS += $(patsubst %.o, build/toplexes/%.o, $(TOPLEX_OBJECTS) )

library: $(LIBRARY_OBJECTS)
	ar rcs lib/libchomp-rutgers.a $(LIBRARY_OBJECTS)

$(BUILD_DIR)/%.o : source/%.cpp include/%.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Cleanup

.PHONY: clean
clean:
	find build -name "*.o" -delete
	find programs -name "*.o" -delete
	find . -name ".?*" -delete
	rm lib/libchomp-rutgers.a
