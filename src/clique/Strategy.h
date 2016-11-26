#pragma once

#include <vector>
#include <memory>
#include "minisat/core/Solver.h"
#include "Graph.h"

namespace CliqueSolver {
class Strategy
{
public:
    explicit Strategy(std::shared_ptr<Graph> sp_graph, int k)
    {
        m_graph = sp_graph;
        m_k = k;
    }

    virtual void SetClauses(Minisat::Solver &solver) = 0;

    virtual std::vector<int> GetClique(Minisat::Solver &solver) const = 0;

protected:
    std::shared_ptr<Graph> m_graph;
    int m_k;
};
}