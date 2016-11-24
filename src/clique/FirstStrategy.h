#pragma once

#include "Strategy.h"

namespace CliqueSolver {
class FirstStrategy : public Strategy
{
public:
    virtual void SetClauses(Minisat::Solver &solver) override;

    virtual std::vector<int> GetClique(Minisat::Solver &solver) const override;

private:
	std::vector<Minisat::Var> m_vertexToVar;
};
}