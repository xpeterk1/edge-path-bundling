#include <iostream>
#include <string>
#include "model/Graph.h"
#include "model/Edge.h"
#include "model/Node.h"
#include "parser/parser.h"

int main()
{
    std::string nodes_path = "data/airports.txt";
    std::string edges_path = "data/routes.csv";
    float d = 1.2;

    Graph g = Parser().load(nodes_path, edges_path, d);
}