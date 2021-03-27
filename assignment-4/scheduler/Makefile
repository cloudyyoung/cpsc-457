SOURCES = main.cpp scheduler.cpp common.cpp
CPPC = g++
CPPFLAGS = -c -Wall -O2
LDLIBS = 
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = scheduler

all: $(TARGET)

deadlock_detector.o: common.h scheduler.h
main.o: common.h scheduler.h
%.o : %.c
$(OBJECTS): Makefile 

.cpp.o:
	$(CPPC) $(CPPFLAGS) $< -o $@

$(TARGET): $(OBJECTS)
	$(CPPC) -o $@ $(OBJECTS) $(LDLIBS)

.PHONY: clean
clean:
	rm -f .*~ *~ *.o $(TARGET)
