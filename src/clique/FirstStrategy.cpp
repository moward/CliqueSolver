#include "Strategy.h"
#include "FirstStrategy.h"

namespace CliqueSolver {

void FirstStrategy::SetClauses(Minisat::Solver &solver)
{
    int u, v;
    int n = m_graph->getNumVertices();
    // add variable for each vertex

    m_vertexToVar = std::vector<Minisat::Var>{ n };

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
                solver.addClause(
                    Minisat::mkLit(m_vertexToVar[u], false),
                    Minisat::mkLit(m_vertexToVar[v], false));
            }
        }
    }
}

std::vector<int> FirstStrategy::GetClique(Minisat::Solver &solver) const
{
    // TODO: implement
    return std::vector<int>{};
}

}