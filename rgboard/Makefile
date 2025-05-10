# Compiler settings
CXX = g++
CXXFLAGS = -Wall -O3 -g -Wextra -Wno-unused-parameter

# Source files and executable
SRCS = src/main.cpp src/display.cpp src/queue-client.cpp
OBJECTS = $(SRCS:.cpp=.o)
BINARIES = rgboard

# Where our RGB library resides
RGB_LIB_DISTRIBUTION = ..
RGB_INCDIR = $(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR = $(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME = rgbmatrix
RGB_LIBRARY = $(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a

# Linker flags: add libcurl and jsoncpp
LDFLAGS += -L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lcurl -ljsoncpp -lrt -lm -lpthread

# Default target
all: $(BINARIES)

# Compile .o files
src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -Iinclude -I$(RGB_INCDIR) -c -o $@ $<

# Link final binary
rgboard: $(OBJECTS) $(RGB_LIBRARY)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

# Clean up
clean:
	rm -f src/*.o $(BINARIES)

.PHONY: all clean
