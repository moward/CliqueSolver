#include <vector>
#include <set>
#include <unordered_map>

namespace CliqueSolver {

/**
 * Stores the graph in an adjacency list
 * Since graph is undirected, each edge appears in two adjacency lists
 * Graph has invariant that every vertex has a positive degree
 */
class Graph
{
private:
    std::unordered_map<int, std::set<int>> edges;

    int numVertices;

    int numEdges;

    // the highest-numbered vertex
    int maxVertex;

public:
    void addEdge(int u, int v);

    bool containsEdge(int u, int v);

    int getNumVertices();

    int getNumEdges();
};

/**
 * Takes in a file of space-separated edges and produces a graph
 */
Graph makeGraph(ofstream& file);

}