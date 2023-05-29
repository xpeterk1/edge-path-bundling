#include "Graph.h"

Graph::Graph(std::map<int, Node>& nodes, std::vector<Edge>& edges, double d)
{
	this->nodes = nodes;
	this->edges = edges;
	this->d = d;
}

std::vector<Node*> Graph::find_shortest_path(Node& source, Node& destination)
{
	// set distances to all nodes to max value
	for (auto& n : nodes) {
		n.second.distance = DBL_MAX;
		n.second.visited = false;
		n.second.previous = nullptr;
	}
	source.distance = 0;	

	// queue for storing nodes to visit, ordered by the closest one
	// note: priority queue is not good here, pop() returns const value
	//       queue is also not good here, it can't store references
	std::vector<Node*> queue;
	queue.push_back(&source);

	while(!queue.empty())
	{
		Node* current = queue.back();
		queue.pop_back();

		current->visited = true;

		// add all children from the source
		for (auto& e : current->edges)
		{
			// skip edges for skip
			if (e.skip) continue;

			int other_id;
			if (e.source_id == source.ID) other_id = e.destination_id;
			else other_id = e.source_id;
			Node& other = nodes.at(other_id);
			
			if (other.visited) continue;
			
			double current_distance = current->distance + e.weight;

			if (current_distance < other.distance) {
				other.distance = current_distance;
				other.previous = current;
				queue.push_back(&other);
			}
		}

		// sort queue => mimic priority queue
		std::sort(queue.begin(), queue.end(), std::greater<>());

		// already found the destination
		if (current == &destination) break;
	}

	// extract path
	std::vector<Node*> path;

	// return empty path, destination was not found
	if (destination.previous == nullptr) return path;
}