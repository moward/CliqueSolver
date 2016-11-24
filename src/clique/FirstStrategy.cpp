#include "Strategy.h"

namespace CliqueSolver {

void FirstStrategy::SetClauses(Minisat::Solver &solver) override
{
    int u, v;
    int n = m_graph->getNumVertices();
    // add variable for each vertex

    m_vertexToVar = vector<Minisat::Var>{ n };

    for (u = 1; u <= n; u++)
    {
        m_vertexToVar[u] = solver.newVar();
    }

    // add clique condition clauses
    for (v = 2; v <= n; v++)
    {
        for (u = 1; u < v; u++)
        {
            if (!m_graph->containsEdge(u, v))
            {
                solver.
            }
        }
    }
}

std::vector<int> FirstStrategy::GetClique(Minisat::Solver &solver) override
{
    // TODO: implement
    return std::vector<int>{};
}

}