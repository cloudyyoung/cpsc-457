.PHONY: all clean

all:	slow-pali fast-pali

slow-pali: slow-pali.cpp
	g++ -O2 -Wall slow-pali.cpp -o slow-pali

fast-pali: fast-pali.cpp
	g++ -O2 -Wall fast-pali.cpp -o fast-pali

clean:
	-/bin/rm -f slow-pali fast-pali *.o *~

