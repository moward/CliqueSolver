/**
 * Based heavily on example code from Niklas Sorensson, Niklas Een
 * https://github.com/niklasso/minisat-examples/blob/master/minumerate/Main.cc
 */

/*****************************************************************************************[Main.cc]
Copyright (c) 2003-2006, Niklas Een, Niklas Sorensson
Copyright (c) 2007-2011, Niklas Sorensson

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************/

#include <fstream>      // std::ofstream
#include <errno.h>
#include <signal.h>
#include <zlib.h>
#include <stdio.h>

#include "Graph.h"
#include "FirstStrategy.h"
#include "minisat/utils/System.h"
#include "minisat/utils/ParseUtils.h"
#include "minisat/utils/Options.h"

using namespace Minisat;
using namespace CliqueSolver;

void printStats(Solver& solver)
{
    double cpu_time = cpuTime();
    double mem_used = memUsedPeak();
    printf("restarts              : %" PRIu64"\n", solver.starts);
    printf("conflicts             : %-12" PRIu64"   (%.0f /sec)\n", solver.conflicts   , solver.conflicts   /cpu_time);
    printf("decisions             : %-12" PRIu64"   (%4.2f %% random) (%.0f /sec)\n", solver.decisions, (float)solver.rnd_decisions*100 / (float)solver.decisions, solver.decisions   /cpu_time);
    printf("propagations          : %-12" PRIu64"   (%.0f /sec)\n", solver.propagations, solver.propagations/cpu_time);
    printf("conflict literals     : %-12" PRIu64"   (%4.2f %% deleted)\n", solver.tot_literals, (solver.max_literals - solver.tot_literals)*100 / (double)solver.max_literals);
    if (mem_used != 0) printf("Memory used           : %.2f MB\n", mem_used);
    printf("CPU time              : %g s\n", cpu_time);
}

//=================================================================================================
// Main:

static Solver* solver;
// Note that '_exit()' rather than 'exit()' has to be used. The reason is that 'exit()' calls
// destructors and may cause deadlocks if a malloc/free function happens to be running (these
// functions are guarded by locks for multithreaded use).
static void SIGINT_exit(int) {
    printf("\n");
    printf("*** INTERRUPTED ***\n");
    if (solver->verbosity > 0){
        printStats(*solver);
        printf("\n");
        printf("*** INTERRUPTED ***\n");
    }
    _exit(1);
}

int main(int argc, char** argv)
{
    try {
        setUsageHelp("USAGE: %s [options] <input-file> <result-output-file>\n");
        
#if defined(__linux__) && defined(_FPU_EXTENDED) && defined(_FPU_DOUBLE) && defined(_FPU_GETCW)
        fpu_control_t oldcw, newcw;
        _FPU_GETCW(oldcw);
        newcw = (oldcw & ~_FPU_EXTENDED) | _FPU_DOUBLE; _FPU_SETCW(newcw);
        printf("WARNING: for repeatability, setting FPU to use double precision\n");
#endif
        // Extra options:
        IntOption    verb   ("MAIN", "verb",   "Verbosity level (0=silent, 1=some, 2=more).", 1, IntRange(0, 2));
        IntOption    cpu_lim("MAIN", "cpu-lim","Limit on CPU time allowed in seconds.\n", INT32_MAX, IntRange(0, INT32_MAX));
        IntOption    mem_lim("MAIN", "mem-lim","Limit on memory usage in megabytes.\n", INT32_MAX, IntRange(0, INT32_MAX));
        BoolOption   cnt_all("MAIN", "all",    "Count all models.", false);
        BoolOption   use_pol("MAIN", "pol",    "Set preferred polarity for minimal model search.", true);

        parseOptions(argc, argv, true);

        Solver S {};
        double initial_time = cpuTime();

        S.verbosity = verb;

        solver = &S;
        // Use signal handlers that forcibly quit:
        signal(SIGINT, SIGINT_exit);
        signal(SIGXCPU,SIGINT_exit);

        if (argc < 2)
        {
        	fprintf(stderr, "Missing data file\n");
        	exit(1);
        }

        std::fstream file { argv[1], std::fstream::in };
		std::shared_ptr<Graph> sp_graph { makeGraph(file) };

		// TODO: take from CMD
		int k = 5;

		FirstStrategy solverStrategy ( sp_graph, k, &S );

		solverStrategy.setClauses();

		file.close();
        
        if (verb > 0)
        {
            printf("============================[ Problem Statistics ]=============================\n");
            printf("|                                                                             |\n");
        }
        
        if (verb > 0)
        {
            printf("|  Number of variables:  %12d                                         |\n", S.nVars());
            printf("|  Number of clauses:    %12d                                         |\n", S.nClauses());
        }
        
        double parsed_time = cpuTime();
        if (verb > 0)
        {
            printf("|  Parse time:           %12.2f s                                       |\n",
            	parsed_time - initial_time);
            printf("|                                                                             |\n");
        }
 
        if (!S.simplify())
        {
            if (verb > 0)
            {
                printf("===============================================================================\n");
                printf("Solved by unit propagation\n");
                printStats(S);
                printf("\n");
            }
            printf("UNSATISFIABLE\n");
            exit(20);
        }
        
        if (verb > 0)
        {
            printf("===============================================================================\n");
            printStats(S);
            printf("\n");
        }

        return 0;
    } catch (OutOfMemoryException&){
        printf("===============================================================================\n");
        printf("INDETERMINATE\n");
        exit(0);
    }
}
