#pragma once

#include <string>
#include <set>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

#include "../model/Node.h"
#include "../model/Edge.h"
#include "../model/Graph.h"

static class Parser
{

public:
	Graph load(std::string nodes_path, std::string edges_path, double d);

private:
	std::map<int, Node> load_nodes(std::string path);
	std::vector<Edge> load_edges(std::string path);
	void combine(std::map<int, Node>& nodes, std::vector<Edge>& edges, double d);

};

