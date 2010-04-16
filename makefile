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

ARCHETYPE_OBJECTS := Cell_Complex_Archetype.o Chain_Archetype.o Toplex_Archetype.o
ALGORITHM_OBJECTS := Homology.o matrix/Smith_Normal_Form.o matrix/Sparse_Matrix.o matrix/Dense_Matrix.o
COMPLEX_OBJECTS := Abstract_Complex.o Cubical_Complex.o Morse_Complex.o Vector_Complex.o Subcomplex.o Product_Complex.o Adaptive_Complex.o
TOPLEX_OBJECTS := Cubical_Toplex.o

LIBRARY_OBJECTS := $(patsubst %.o, build/archetypes/%.o, $(ARCHETYPE_OBJECTS) )
LIBRARY_OBJECTS += $(patsubst %.o, build/algorithms/%.o, $(ALGORITHM_OBJECTS) )
LIBRARY_OBJECTS += $(patsubst %.o, build/complexes/%.o, $(COMPLEX_OBJECTS) )
LIBRARY_OBJECTS += $(patsubst %.o, build/toplexes/%.o, $(TOPLEX_OBJECTS) )

library: $(LIBRARY_OBJECTS)
	ar rcs lib/libchomp-rutgers.a $(LIBRARY_OBJECTS)

$(BUILD_DIR)/%.o : source/%.cpp include/%.h include/%.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Cleanup

.PHONY: clean
clean:
	find build -name "*.o" -delete
	find programs -name "*.o" -delete
	find . -name ".?*" -delete
	rm lib/libchomp-rutgers.a
