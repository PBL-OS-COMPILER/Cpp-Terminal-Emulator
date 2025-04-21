# Makefile - Build Instructions

CXX = g++
CXXFLAGS = -Wall -std=c++17
OBJS = main.o

all: terminal

terminal: main.cpp
	$(CXX) $(CXXFLAGS) -o terminal main.cpp

clean:
	rm -f terminal *.o
