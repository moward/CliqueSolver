#include <iostream>
#include <fstream>
#include <algorithm>    // std::max
#include <cassert>      // assert
#include <string>
#include <stdio.h>      // sscanf
#include "Graph.h"

namespace CliqueSolver {

Graph::Graph() :
      edges {}
    , numVertices { 0 }
    , numEdges { 0 }
    , maxVertex { 0 }
{}

void Graph::addEdge(int u, int v)
{
    assert(u != v);

    maxVertex = std::max(std::max(maxVertex, u), v);
    if (!edges.count(u))
    {
        // u unknown
        numVertices++;
        edges.emplace(std::make_pair(u, std::set<int>()));
    }

    if (!edges.count(v))
    {
        // v unknown
        numVertices++;
        edges.emplace(std::make_pair(v, std::set<int>()));
    }

    // make sure u is less than v
    if (u > v) {
        int t = u;
        u = v;
        v = t;
    }

    auto insertRetUv = edges[u].insert(v);

    if (insertRetUv.second)
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

int Graph::getMaxVertex()
{
    return maxVertex;
}

/**
 * Takes in a file of space-separated edges and produces a graph
 */
std::shared_ptr<Graph> makeGraph(std::fstream& file)
{
    std::shared_ptr<Graph> sp_graph = std::make_shared<Graph>();
    std::string line {};
    int u, v;

    while (std::getline(file, line))
    {
        int numArgs = sscanf(line.c_str(), "%d %d", &u, &v);
        if (numArgs == 2) {
            sp_graph->addEdge(u, v);
        }
    }

    return sp_graph;
}
}