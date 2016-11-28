#pragma once

#include <vector>
#include <memory>
#include "minisat/core/Solver.h"
#include "Graph.h"

namespace CliqueSolver {
class Strategy
{
public:
    explicit Strategy(std::shared_ptr<Graph> sp_graph, int k, Minisat::Solver *solver) :
        m_graph (sp_graph),
        m_k (k),
        m_solver (solver)
    {}

    virtual void setClauses() = 0;

    virtual std::vector<int> getClique() const = 0;

protected:
    std::shared_ptr<Graph> m_graph;
    int m_k;
    Minisat::Solver *m_solver;
};
}