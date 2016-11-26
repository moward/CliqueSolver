#include "Strategy.h"
#include "FirstStrategy.h"

namespace CliqueSolver {

int nextPower2 (int v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    return v++;
}

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

    // add adders
    int k_up = nextPower2(m_k);
    // least integer [k_prime] such that [m_k + k_prime] is a power of 2
    int k_prime = k_up - m_k;
}

std::vector<int> FirstStrategy::GetClique(Minisat::Solver &) const
{
    // TODO: implement
    return std::vector<int>{};
}

}