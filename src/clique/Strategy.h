#include <vector>
#include "minisat/core/Solver.h"
#include "Graph.h"

namespace CliqueSolver {
class Strategy
{
public:
    Strategy(std::shared_ptr<Graph> graph)
    {
        m_graph = graph;
    }

    virtual void SetClauses(Minisat::Solver &solver) = 0;

    virtual std::vector<int> GetClique(Minisat::Solver &solver) const = 0;

protected:
    std::shared_ptr<Graph> m_graph;
};
}