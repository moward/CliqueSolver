# CliqueSolver
Studying the Application of SAT Solvers to the Clique Problem

# Documentation
Read the [report](http://matt.how/clique.pdf) detailing both the theory and evaluation of the solver.

# Building

## Installing MiniSat
In order to build the Clique Solver, you must first install [MiniSat](http://minisat.se/MiniSat.html) version 2.2.0. Then, copy the installation directory (containing `include` and `lib` folders) to `build/minisat`. 

Alternatively, you can install MiniSat directly in the build directory like so:
```bash
> wget http://minisat.se/downloads/minisat-2.2.0.tar.gz
> tar -xvf minisat-2.2.0.tar.gz
> cd minisat-2.2.0
> make config prefix=/PATH/TO/CLIQUESOLVER/build/minisat
> make install
```

## Building the Clique Solver
Once MiniSat is installed, you can build the Clique Solver using `make`.

# Usage
The Clique Solver runs on an `.edge` file. The file represents one edge in the graph on each line, with each edge id separated by a single space. The file must be "normalized", meaning the edges are numbered continuously from 1 to *m*. There is a Python script which normalizes an edge file in the `src/data-utils` directory.

The Clique Solver is a decision solver, so it takes in a parameter *k*.

E.g.
```bash
./build/bin/CliqueSolver data/DIR/FILE.edges result -k=15
```

## Python Dependencies
 * [python-igraph](http://igraph.org/python/)