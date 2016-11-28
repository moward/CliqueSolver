CXX=g++
CFLAGS=-Wall -std=gnu++11 -g -I ./build/minisat/include -D __STDC_LIMIT_MACROS -D __STDC_FORMAT_MACROS -Wno-parentheses -Wextra
LDFLAGS=-lm -lz -Lbuild/minisat/lib -static -lminisat -g
SOURCES=FirstStrategy.cpp Graph.cpp Main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
OBJECTPATHS=$(addprefix $(ODIR),$(OBJECTS))
EXECUTABLE=build/bin/CliqueSolver
ODIR=build/obj/
DEPS=$(wildcard src/clique/*.h)

all: $(EXECUTABLE) $(OBJECTPATHS)

echo:
	echo $(OBJECTPATHS)

$(OBJECTPATHS):$(ODIR)%.o: src/clique/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)
#$(addprefix $(ODIR),$(OBJECTS)): %.c $(DEPS)
#	$(CXX) -c -o $@ $< $(CFLAGS)

$(EXECUTABLE): $(OBJECTPATHS)
	$(CXX) $(OBJECTPATHS) $(LDFLAGS) -o $@

.cpp.o:
	$(CXX) $(CFLAGS) $< -o $@

clean:
	rm -f $(ODIR)*.o $(EXECUTABLE)