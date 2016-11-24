#include <iostream>
#include <fstream>
#include <algorithm>    // std::max
#include <cassert>      // assert
#include "Graph.h"

namespace CliqueSolver {

void Graph::addEdge(int u, int v)
{
    assert(u != v);

    maxVertex = max(max(maxVertex, u), v);
    if (!edges.count(u))
    {
        // u unknown
        numVertices++;
        edges.emplace(u, std::set<int>{});
    }

    if (!edges.count(v))
    {
        // v unknown
        numVertices++;
        edges.emplace(v, std::set<int>{});
    }

    // make sure u is less than v
    if (u > v) {
        int t = u;
        u = v;
        v = t;
    }

    auto insertRetUv = edges[u].insert(v);

    if (invertRevUv.second)
    {
        // insert edge count
        numEdges++;
    }
}

bool Graph::containsEdge(int u, int v)
{
    // make sure u is less than v
    if (u > v) {
        int t = u;
        u = v;
        v = t;
    }

    auto edgeList = edges[u];

    return edgeList.find(v) != edgeList.end();
}


int Graph::getNumVertices()
{
    return numVertices;
}

int Graph::getNumEdges()
{
    return numEdges;
}

/**
 * Takes in a file of space-separated edges and produces a graph
 */
Graph makeGraph(ofstream& file)
{
    Graph graph;
    string line;
    int u, v;

    while (getline(file, line))
    {
        int numArgs = sscanf(line, "%d %d", &u, &v);
        if (numArgs == 2) {
            graph.addEdge(u, v);
        }
    }

    myfile.close();
}
}