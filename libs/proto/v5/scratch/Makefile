#CXX=C:\cygwin\usr\local\gcc-4.7\bin\g++
CXX="C:/cygwin/usr/local/clang-trunk/bin/clang++.exe"
#CFLAGS=-c -Wall -O2 -DNDEBUG
#CFLAGS=-c -O2 -DNDEBUG
CFLAGS=-c -O0
CXXFLAGS=-std=gnu++11 -I../../../.. -I/cygdrive/c/Users/eric/Code/boost/org/trunk
LDFLAGS=
SOURCES=./main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=proto

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CXX) $(CFLAGS) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE).exe
