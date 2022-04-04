SOURCES := $(wildcard *.cpp) $(wildcard */*.cpp)
OBJECTS := $(SOURCES:.cpp=.o)
TARGET := $(shell pwd | xargs basename)
CXX = g++
CXXFLAGS = -g -Wall -std=c++20 -MMD -I ./ -DDEBUG
DEPENDS = ${OBJECTS:.o=.d} main.d


timer.o: Timer.cpp
	g++ -g -std=c++20 Timer.cpp -c -o timer.o

${TARGET}: ${OBJECTS}
	g++ -g -std=c++20 ${OBJECTS} -o ${TARGET} 

-include ${DEPENDS}

clean:
	rm -f *.o
	rm -f */*.o
	rm -f *.d
	rm -f */*.d
