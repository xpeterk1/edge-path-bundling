#include <iostream>
#include <string>
#include "model/Graph.h"
#include "model/Edge.h"
#include "model/Node.h"
#include "parser/parser.h"

int main()
{
    std::string nodes_path = "data/airports.txt";
    std::string edges_path = "data/routes-preprocessed.csv";
    float d = 1.2;

    /*
    This graph is/has:
    
    a) Loaded nodes, edges
    b) No nodes without edges
    c) It is not oriented -> even though edges have 'direction', they should be treated as both way edges.
    d) Edges have computed real distances and weights using the d parameter. They are also sorted descending by this value.
    e) Edge references inside nodes are sorted according to their distance, ascending
    
    */

    Graph g = Parser().load(nodes_path, edges_path, d);

    for (Edge& edge : g.edges)
    {
        if (edge.lock) continue;

        edge.skip = true;
        Node& source = g.nodes.at(edge.source_id);
        Node& dest = g.nodes.at(edge.destination_id);

        g.find_shortest_path(source, dest);
    }
}