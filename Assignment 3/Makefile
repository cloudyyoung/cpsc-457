SOURCES = main.cpp calcpi.cpp
CPPC = g++
CPPFLAGS = -c -Wall -O2
LDLIBS = -pthread
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = calcpi

all: $(TARGET)

calcpi.o: calcpi.h
main.o: calcpi.h
%.o : %.c
$(OBJECTS): Makefile 

.cpp.o:
	$(CPPC) $(CPPFLAGS) $< -o $@

$(TARGET): $(OBJECTS)
	$(CPPC) -o $@ $(OBJECTS) $(LDLIBS)

.PHONY: clean
clean:
	rm -f *~ *.o $(TARGET) 

