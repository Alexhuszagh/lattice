#  :copyright: (c) 2016 The Regents of the University of California.
#  :license: MIT, see LICENSE.md for more details.

# COMPILER
# --------

SHELL = /bin/sh
CXX = c++
AR = ar
RANLIB = ranlib

# FLAGS
# -----

SOURCES = $(shell echo src/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
EXAMPLE_SOURCES = $(shell echo example/*.cpp)
EXAMPLE_OBJECTS = $(EXAMPLE_SOURCES:.cpp=.o)
EXAMPLES = $(EXAMPLE_SOURCES:.cpp=)
INCLUDE = include/
CXXFLAGS  = -Wall -O3 -fPIC -std=c++11 -pthread -I$(INCLUDE)

# TARGETS
# -------

STATICLIB = liblattice.a
SHAREDLIB = liblattice.so

all: static shared examples
static: $(OBJECTS)
	$(AR) qc $(STATICLIB) $(OBJECTS)
	$(RANLIB) $(STATICLIB)
shared: $(OBJECTS)
	$(CXX) -o $(SHAREDLIB) $(CXXFLAGS) $(OBJECTS) -shared
examples: static $(EXAMPLE_OBJECTS)
	$(foreach example, $(EXAMPLES), $(CXX) $(example).o -o $(example) $(CXXFLAGS) $(STATICLIB))

clean:
	rm -f $(OBJECTS) $(STATICLIB) $(SHAREDLIB) $(EXAMPLE_OBJECTS) $(EXAMPLES)
