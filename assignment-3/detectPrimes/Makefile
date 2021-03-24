SOURCES = main.cpp detectPrimes.cpp
CPPC = g++
CPPFLAGS = -c -Wall -O2
LDLIBS = -pthread -lm
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = detectPrimes

all: $(TARGET)

sumFactors.o: detectPrimes.h
main.o: detectPrimes.h
%.o : %.c
$(OBJECTS): Makefile 

.cpp.o:
	$(CPPC) $(CPPFLAGS) $< -o $@

$(TARGET): $(OBJECTS)
	$(CPPC) -o $@ $(OBJECTS) $(LDLIBS)

.PHONY: clean
clean:
	rm -f *~ *.o $(TARGET) 

