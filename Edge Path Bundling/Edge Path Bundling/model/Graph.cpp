#include "Graph.h"

Graph::Graph(std::map<int, Node>& nodes, std::vector<Edge>& edges, double d)
{
	this->nodes = nodes;
	this->edges = edges;
	this->d = d;
}