# chomp-rutgers #

(Warning: This documentation is under construction. Some of the links will be broken.)

## Introduction ##

This is software (written in C++) capable of representing chain complexes and computing their homology. We provide command line tools and also a software library for developers. To get started, we have [installation instructions](Installation.md) and [a tutorial](Tutorial.md).

There are several modes of usage for this software package. For basic usage, we offer several command line tools that accept text file input describing a complex. We also provide a lower-level C++ interface for developers. Advanced users can even extend the code by creating new types of complexes due to the generic construction of the algorithms.

## Features ##

  * Ability to represent several types of complexes, including Cubical, Simplicial, and arbitrary chain complexes given by their boundary maps.
  * Smith Normal Form based homology algorithm
  * Discrete Morse Theory based homology algorithm
  * Routines for computing the induced map on homology and Conley Index
  * Homology algorithms can return representative chains for the homology generators

## Documentation ##

We offer three levels of documentation. First, we describe installation:

[Installation](Installation.md)

Second, we describe the provided command-line tools:

[Tutorial](Tutorial.md)

Finally, we provide documentation intended for developers who wish to call chomp-rutgers methods directly in their software via the C++ interface:

[Developer Documentation](Developer.md)