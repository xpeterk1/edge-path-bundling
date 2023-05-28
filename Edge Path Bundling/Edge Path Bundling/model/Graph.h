#pragma once

#include <set>
#include <vector>
#include <map>

#include "Edge.h"
#include "Node.h"


class Graph
{

private:
	std::map<int, Node> nodes;
	std::vector<Edge> edges;

	double d;

public:	
	Graph(std::map<int, Node>& nodes, std::vector<Edge>& edges, double d);

};

