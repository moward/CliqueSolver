#pragma once

#include "Strategy.h"

namespace CliqueSolver {
class FirstStrategy : public Strategy
{
    using Strategy::Strategy;

public:
    virtual void setClauses() override;

    virtual std::vector<int> getClique() const override;

private:
    /**
     * Takes in three bits and outputs the sum and carry out of each
     * Returns: A pair of new variables. First is sum. Second is carry out.
     */
    std::pair<Minisat::Var, Minisat::Var> addHalfAdder(Minisat::Var a, Minisat::Var b, Minisat::Var c);

    /**
     * Adds A and B, two n-bit numbers, and returns an (n+1)-bit number
     */
    std::vector<Minisat::Var> addNBitAdder(size_t n, std::vector<Minisat::Var> &A, std::vector<Minisat::Var> &B);

    Minisat::Var getFalseVar();

    // initialized to var_Undef
    Minisat::Var m_falseVar { -1 };
};
}