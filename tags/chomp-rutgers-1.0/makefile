
PATH_TO_BOOST_INCLUDE:=/usr/local/include/

all: chomp-cubical chomp-simplicial chomp-matrix greyscale-to-cubical

chomp-cubical:
	g++ -O3 ./source/chomp-cubical.cpp -I./include/ -I$(PATH_TO_BOOST_INCLUDE) -o ./bin/chomp-cubical

chomp-simplicial:
	g++ -O3 ./source/chomp-simplicial.cpp -I./include/ -I$(PATH_TO_BOOST_INCLUDE) -o ./bin/chomp-simplicial

chomp-matrix:
	g++ -O3 ./source/chomp-matrix.cpp -I./include/ -I$(PATH_TO_BOOST_INCLUDE) -o ./bin/chomp-matrix

greyscale-to-cubical:
	g++ ./source/greyscale-to-cubical.cpp -I./include -I$(PATH_TO_BOOST_INCLUDE) -O3 -I/usr/X11/include/ -lm -L/usr/X11R6/lib -lpthread -lX11 -o ./bin/greyscale-to-cubical
