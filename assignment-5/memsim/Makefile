.PHONY: all clean
all: memsim

memsim.cpp main.cpp: memsim.h

memsim:	memsim.cpp Makefile main.cpp
	g++ -O2 -Wall memsim.cpp main.cpp -o memsim

clean:
	/bin/rm -f *~ memsim

