.PHONY: all clean
all: fatsim

fatsim.cpp main.cpp: fatsim.h

fatsim: fatsim.cpp Makefile main.cpp
	g++ -O2 -Wall fatsim.cpp main.cpp -o fatsim

clean: Makefile
	/bin/rm -f *~
	/bin/rm -f fatsim
