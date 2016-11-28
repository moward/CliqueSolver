#include <vector>

#include "Strategy.h"
#include "FirstStrategy.h"

namespace CliqueSolver {

int nextPower2(int v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    return v++;
}

void FirstStrategy::setClauses()
{
    int u, v;
    int n = m_graph->getMaxVertex();

    // add variable for each vertex
    for (u = 0; u < n; u++)
    {
        m_solver->newVar();
    }

    // add clique condition clauses
    for (v = 2; v <= n; v++)
    {
        for (u = 1; u < v; u++)
        {
            if (!m_graph->containsEdge(u, v))
            {
                m_solver->addClause(Minisat::mkLit(u - 1, false), Minisat::mkLit(v - 1, false));
            }
        }
    }

    // add adders
    //int k_up { nextPower2(m_k) };
    // least integer [k_prime] such that [m_k + k_prime] is a power of 2
    //int k_prime { k_up - m_k };
}

std::pair<Minisat::Var, Minisat::Var> FirstStrategy::addHalfAdder(Minisat::Var a, Minisat::Var b, Minisat::Var c)
{
    Minisat::Lit la ( Minisat::mkLit(a, true) );
    Minisat::Lit lb ( Minisat::mkLit(b, true) );
    Minisat::Lit lc ( Minisat::mkLit(c, true) );

    // carry out
    Minisat::Var c_out { m_solver->newVar() };
    Minisat::Lit lc_out ( Minisat::mkLit(c_out, true) );

    // c_out = AB + BC + AC
    m_solver->addClause(~lc_out, la, lb);
    m_solver->addClause(~lc_out, lb, lc);
    m_solver->addClause(~lc_out, la, lc);
    m_solver->addClause(lc_out, ~la, ~lb);
    m_solver->addClause(lc_out, ~lb, ~lc);
    m_solver->addClause(lc_out, ~la, ~lc);

    // sum
    // Note: this implementation actually uses one intermediary half_s,
    // which is not explicitly necessary but makes the clauses cleaner
    Minisat::Var sum { m_solver->newVar() };
    Minisat::Lit lsum ( Minisat::mkLit(sum, true) );

    Minisat::Var half_s { m_solver->newVar() };
    Minisat::Lit lhalf_s ( Minisat::mkLit(half_s, true) );

    // lhalf = A xor B
    m_solver->addClause(~lhalf_s, la, lb);
    m_solver->addClause(~lhalf_s, ~la, ~lb);
    m_solver->addClause(lhalf_s, ~la, lb);
    m_solver->addClause(lhalf_s, la, ~lb);

    // sum = L_half xor C
    m_solver->addClause(~lsum, lhalf_s, lc);
    m_solver->addClause(~lsum, ~lhalf_s, ~lc);
    m_solver->addClause(lsum, ~lhalf_s, lc);
    m_solver->addClause(lsum, lhalf_s, ~lc);

    return std::make_pair(sum, c_out);
}

Minisat::Var FirstStrategy::getFalseVar() {
    if (m_falseVar == Minisat::var_Undef) {
        // create new variable
        m_falseVar = m_solver->newVar();

        // force false
        m_solver->addClause(Minisat::mkLit(m_falseVar, false));
    }
    return m_falseVar;
}

std::vector<Minisat::Var> FirstStrategy::addNBitAdder(size_t n, std::vector<Minisat::Var> &A, std::vector<Minisat::Var> &B)
{
    assert(A.size() == n && B.size() == n);

    std::vector<Minisat::Var> sum {};

    Minisat::Var carry ( getFalseVar() );

    size_t i;
    for (i = 0; i < n; i++)
    {
        auto resultPair ( addHalfAdder(A[i], B[i], carry) );
        sum.push_back(resultPair.first);
        carry = resultPair.second;
    }

    sum.push_back(carry);

    return std::move(sum);
}

std::vector<int> FirstStrategy::getClique() const
{
    // TODO: implement
    return std::vector<int>{};
}

}