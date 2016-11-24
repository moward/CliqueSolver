CXX=clang-3.5
CFLAGS=-Wall -std=gnu++11 -g -I ./build/minisat/include -D __STDC_LIMIT_MACROS -D __STDC_FORMAT_MACROS -Wno-parentheses -Wextra
LDFLAGS=-lm -Lbuild/minisat/lib -lminisat
SOURCES=FirstStrategy.cpp Graph.cpp
OBJECTS=$(SOURCES:.cpp=.o)
OBJECTPATHS=$(addprefix $(ODIR),$(OBJECTS))
EXECUTABLE=
ODIR=build/obj/
DEPS=$(wildcard *.h)

all: $(EXECUTABLE) $(OBJECTPATHS)
	echo $(OBJECTPATHS)

$(OBJECTPATHS):$(ODIR)%.o: src/clique/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)
#$(addprefix $(ODIR),$(OBJECTS)): %.c $(DEPS)
#	$(CXX) -c -o $@ $< $(CFLAGS)

$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CXX) $(CFLAGS) $< -o $@

clean:
	rm -f $(ODIR)/*.o