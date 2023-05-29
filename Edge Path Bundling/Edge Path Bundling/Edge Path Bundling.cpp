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
    
    a) loaded nodes, edges
    b) no nodes without edges
    c) it is not oriented -> even though edges have 'direction', they should be treated as both way edges
    d) edges have computed real distances and weights using the d parameter. They are also sorted descending by this value
    
    */

    Graph g = Parser().load(nodes_path, edges_path, d);
}