#include <vector>
#include <algorithm>    // std::max

#include "Strategy.h"
#include "FirstStrategy.h"

namespace CliqueSolver {

/*int nextPower2(int v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    return v++;
}*/

/*
 * The base 2 logarithm of a number, rounded up
 */
size_t log2Up(size_t v) {
    v--;
    int ex = 0;
    while (v > 0) {
        v = v / 2;
        ex++;
    }
    return ex;
}

void FirstStrategy::setClauses()
{
    int u, v;
    int n = m_graph->getMaxVertex();

    // add variable for each vertex
    while (n >= m_solver->nVars())
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
                m_solver->addClause(Minisat::mkLit(u, true), Minisat::mkLit(v, true));
            }
        }
    }

    // Add adders

    size_t log_k { log2Up(m_k) };

    size_t k_up { 1u << log_k };

    // least integer [k_prime] such that [m_k + k_prime] is 2 ^ log_k
    size_t k_prime { k_up - m_k };

    printf("k: %d\nLog k: %lu\nnext power 2: %lu\nk_prime: %lu\n", m_k, log_k, k_up, k_prime);

    // get number of vertices in clique
    std::vector<Minisat::Var> clique_size (addRangeVariables(1, n + 1, log_k + 1u));

    std::vector<Minisat::Var> clique_size_k_prime (addNBitAdder(log_k + 1u, clique_size, intToVector(k_prime)));

    // inequality check clique_size >= m_k
    m_solver->addClause(Minisat::mkLit(clique_size_k_prime[log_k], false));
}

std::vector<Minisat::Var> FirstStrategy::intToVector(size_t n)
{
    std::vector<Minisat::Var> v;

    while (n > 0)
    {
        v.push_back(n & 0x1 ? getTrueVar() : getFalseVar());
        n = n / 2;
    }

    return std::move(v);
}

/*
 * Add together the values of variables [a] through [b], including [a] but not [b]
 * Returns a binary vector of size [max (max_bits, # of bits to represent (b - a))]
 * Note: Uses a divide-and-conquer adder tree
 */
std::vector<Minisat::Var> FirstStrategy::addRangeVariables(int a, int b, size_t max_bits)
{
    assert(a < b);
    assert(a > 0);
    assert(b > 0);

    if (b == a + 1)
    {
        // base case
        return std::vector<Minisat::Var> { a };
    } else {
        // divide and conquery
        int size { b - a };
        int mid {a + size / 2};

        std::vector<Minisat::Var> leftChild (addRangeVariables(a, mid, max_bits));
        std::vector<Minisat::Var> rightChild (addRangeVariables(mid, b, max_bits));

        size_t bitsNeeded { log2Up(size + 1u) };

        // add subtrees together
        return addNBitAdder(std::min(max_bits, bitsNeeded), leftChild, rightChild);
    }
}

/**
 * Takes in three bits and outputs the sum and carry out of each
 * Returns: A pair of new variables. First is sum. Second is carry out.
 */
std::pair<Minisat::Var, Minisat::Var> FirstStrategy::addFullAdder(Minisat::Var a, Minisat::Var b, Minisat::Var c)
{
    Minisat::Lit la ( Minisat::mkLit(a, false) );
    Minisat::Lit lb ( Minisat::mkLit(b, false) );
    Minisat::Lit lc ( Minisat::mkLit(c, false) );

    // carry out
    Minisat::Var c_out { m_solver->newVar() };
    Minisat::Lit lc_out ( Minisat::mkLit(c_out, false) );

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
    Minisat::Lit lsum ( Minisat::mkLit(sum, false) );

    Minisat::Var half_s { m_solver->newVar() };
    Minisat::Lit lhalf_s ( Minisat::mkLit(half_s, false) );

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
        m_solver->addClause(Minisat::mkLit(m_falseVar, true));
    }
    return m_falseVar;
}

Minisat::Var FirstStrategy::getTrueVar() {
    if (m_trueVar == Minisat::var_Undef) {
        // create new variable
        m_trueVar = m_solver->newVar();

        // force false
        m_solver->addClause(Minisat::mkLit(m_trueVar, false));
    }
    return m_trueVar;
}

/*
 * Construct an n-bit adder, for A + B. Outputs n-bit vector
 * If sum doesn't fit into n-bits, the remaining bits
 * will be cut off
 */
std::vector<Minisat::Var> FirstStrategy::addNBitAdder(size_t n, const std::vector<Minisat::Var> &A, const std::vector<Minisat::Var> &B)
{
    std::vector<Minisat::Var> sum {};

    Minisat::Var carry ( getFalseVar() );

    size_t input_bits { std::max(A.size(), B.size()) };

    // make sure we're not asking for unnecessary leading zeros
    assert (input_bits + 1 >= n);

    size_t i;

    int a, b;

    for (i = 0; i < input_bits && i < n; i++)
    {
        a = b = getFalseVar();

        if (i < A.size()) {
            a = A[i];
        }

        if (i < B.size()) {
            b = B[i];
        }

        auto resultPair ( addFullAdder(a, b, carry) );
        sum.push_back(resultPair.first);
        carry = resultPair.second;
    }

    // fill remaining with carry and then zeros
    while (sum.size() < n) {
        sum.push_back(carry);
        carry = getFalseVar();
    }

    assert (sum.size() == n);

    return std::move(sum);
}

std::vector<int> FirstStrategy::getClique() const
{
    std::vector<int> clique {};
    int n = m_graph->getMaxVertex();
    
    for (int i = 1; i < n; i++) {
        if (m_solver->model[i] == Minisat::l_True)
        {
            clique.push_back(i);
        }
    }
    return std::move(clique);
}

}