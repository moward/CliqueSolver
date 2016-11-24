CXX=clang-3.5
CFLAGS=-Wall -std=c++11 -g I ../../build/
LDFLAGS=-lm -Lbuild/minisat/lib -lminisat
SOURCES=FirstStrategy.cpp Graph.cpp
OBJECTS=$(SOURCES:.cpp=.o)
OBJECTPATHS=$(addprefix $(ODIR),$(OBJECTS))
EXECUTABLE=
ODIR=../../build/obj/
DEPS=$(wildcard *.h)

all: $(EXECUTABLE) $(OBJECTPATHS)
	echo $(OBJECTPATHS)

$(ODIR)%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)
#$(addprefix $(ODIR),$(OBJECTS)): %.c $(DEPS)
#	$(CXX) -c -o $@ $< $(CFLAGS)

$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CXX) $(CFLAGS) $< -o $@

clean:
	rm -f $(ODIR)/*.o