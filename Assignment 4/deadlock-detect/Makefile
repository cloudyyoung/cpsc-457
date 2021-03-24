SOURCES = main.cpp deadlock_detector.cpp common.cpp
CPPC = g++
CPPFLAGS = -c -Wall -O2
LDLIBS = 
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = deadlock

all: $(TARGET)

deadlock_detector.o: common.h deadlock_detector.h
main.o: common.h deadlock_detector.h
%.o : %.c
$(OBJECTS): Makefile 

.cpp.o:
	$(CPPC) $(CPPFLAGS) $< -o $@

$(TARGET): $(OBJECTS)
	$(CPPC) -o $@ $(OBJECTS) $(LDLIBS)

.PHONY: clean
clean:
	rm -f .*~ *~ *.o $(TARGET)
