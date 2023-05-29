#pragma once

#include <set>
#include <vector>
#include <map>
#include <queue>

#include "Edge.h"
#include "Node.h"


class Graph
{

public:
	std::map<int, Node> nodes;
	std::vector<Edge> edges;

	double d;

public:	
	Graph(std::map<int, Node>& nodes, std::vector<Edge>& edges, double d);
	std::vector<Node*> find_shortest_path(Node& source, Node& destination);

};

