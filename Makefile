main: main.o
	g++ main.o -o main

main.o: main.cpp cbuffer.h
	g++ -c main.cpp -o main.o