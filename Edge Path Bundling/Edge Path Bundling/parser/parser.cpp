#include "parser.h"

Graph Parser::load(std::string nodes_path, std::string edges_path, double d)
{
	std::map<int, Node> nodes = load_nodes(nodes_path);
	std::vector<Edge> edges = load_edges(edges_path);

	combine(nodes, edges, d);

	// remove nodes without edges
	for (auto node_pair = nodes.begin(); node_pair != nodes.end(); )
	{
		if (node_pair->second.edges.empty())
		{
			node_pair = nodes.erase(node_pair);
		} else 
		{
			node_pair++;
		}
	}

	// sort edges
	std::sort(edges.begin(), edges.end(), std::greater<>());

	Graph g = Graph(nodes, edges, d);
	return g;
}

std::map<int, Node> Parser::load_nodes(std::string path)
{

	std::map<int, Node> nodes = std::map<int, Node>();
	std::ifstream file;

	file.open(path, std::ios::out);

	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string word;

		int id;
		std::string name;
		float latitude, longitude;

		for (int i = 0; i < 14; i++)
		{
			std::getline(ss, word, ',');
			if (i == 0) // ID
			{
				id = std::stoi(word);
			}
			else if (i == 1) // Name
			{
				name = word;
				name = name.substr(1, name.size() - 2);
			}
			else if (i == 6) // Latitude
			{
				latitude = std::stof(word);
			}
			else if (i == 7) // Longitude
			{
				longitude = std::stof(word);
			}
		}

		Node n = Node(id, longitude, latitude, name);
		nodes.emplace(id, n);
	}

	file.close();

	return nodes;
}

std::vector<Edge> Parser::load_edges(std::string path)
{
	std::vector<Edge> edges = std::vector<Edge>();

	std::ifstream file;

	file.open(path, std::ios::out);

	std::string line;

	// skip the first line with header
	std::getline(file, line);
	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string word;

		int source_id, destination_id;
		bool valid = true;

		for (int i = 0; i < 2; i++)
		{
			std::getline(ss, word, ',');
			if (i == 0) // source ID
			{
				if (word == "\\N")
				{
					valid = false;
					break;
				}

				source_id = stoi(word);
			}
			else if (i == 1) // destination ID
			{
				if (word == "\\N")
				{
					valid = false;
					break;
				}

				destination_id = stoi(word);
			}
		}

		if (valid)
		{
			Edge e = Edge(source_id, destination_id);
			edges.push_back(e);
		}
	}

	return edges;
}

void Parser::combine(std::map<int, Node>& nodes, std::vector<Edge>& edges, double d)
{
	for (auto it = edges.begin(); it < edges.end(); ) {
		int source_id = it->source_id;
		int dest_id = it->destination_id;

		// either source of destination ID was not found -> remove this edge
		if (!nodes.contains(source_id) || !nodes.contains(dest_id))
		{
			it = edges.erase(it);
			continue;
		}

		Node& source = nodes.at(source_id);
		Node& destination = nodes.at(dest_id);

		// set distances and weights of the edges
		double distance = source.distance_to(destination);

		it->set_weight(distance, d);

		// set edges to nodes
		source.edges.push_back(*it);
		destination.edges.push_back(*it);
		it++;
	}
}