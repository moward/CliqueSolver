# CliqueSolver
Studying the Application of SAT Solvers to the Clique Problem


# Building
In order to build minisat, run
```bash
> cd src/minisat
> make config prefix=/PATH/TO/CLIQUESOLVER/build/minisat
> make install
```

# Usage
```bash
./build/bin/CliqueSolver data/DIR/FILE.edges result -k=15

for X in A B C D E F G H I J; do python ./src/data-utils/graph-stats.py data/random/$X.edges; done
```

## Python Dependencies
 * [python-igraph](http://igraph.org/python/)