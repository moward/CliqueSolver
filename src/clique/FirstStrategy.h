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
    std::pair<Minisat::Var, Minisat::Var> addFullAdder(Minisat::Var a, Minisat::Var b, Minisat::Var c);

    /*
     * Construct an n-bit adder, for A + B. Outputs n-bit vector
     * If sum doesn't fit into n-bits, the remaining bits
     * will be cut off
     */
    std::vector<Minisat::Var> addNBitAdder(size_t n, const std::vector<Minisat::Var> &A, const std::vector<Minisat::Var> &B);

    /*
     * Add together the values of variables [a] through [b], including [a] but not [b]
     * Returns a binary vector of size [max (max_bits, # of bits to represent (b - a))]
     * Note: Uses a divide-and-conquer adder tree
     */
    std::vector<Minisat::Var> addRangeVariables(int a, int b, size_t max_bits);

    std::vector<Minisat::Var> intToVector(size_t n);

    Minisat::Var getFalseVar();

    Minisat::Var getTrueVar();

    // initialized to var_Undef
    Minisat::Var m_falseVar { -1 };

        // initialized to var_Undef
    Minisat::Var m_trueVar { -1 };
};
}