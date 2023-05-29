#include "Graph.h"

Graph::Graph(std::map<int, Node>& nodes, std::vector<std::shared_ptr<Edge>>& edges, double d)
{
	this->nodes = nodes;
	this->edges = edges;
	this->d = d;
}

std::vector<std::shared_ptr<Edge>> Graph::find_shortest_path(Node& source, Node& destination)
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
			if (e.get()->skip) continue;

			int other_id;
			if (e.get()->source_id == source.ID) other_id = e.get()->destination_id;
			else other_id = e.get()->source_id;
			Node& other = nodes.at(other_id);
			
			if (other.visited) continue;
			
			double current_distance = current->distance + e.get()->weight;

			if (current_distance < other.distance) {
				other.distance = current_distance;
				other.previous = current;
				other.previous_edge = e;
				queue.push_back(&other);
			}
		}

		// sort queue => mimic priority queue
		std::sort(queue.begin(), queue.end(), [](const Node* n1, const Node* n2)
			{
				return n1->distance < n2->distance;
			});

		// already visited the destination
		if (current == &destination) break;
	}

	// extract path
	std::vector<std::shared_ptr<Edge>> path;

	// return empty path, destination was not found
	Node* pointer = &destination;
	if (destination.previous != nullptr) 
	{
		while (pointer->previous_edge != nullptr)
		{
			path.push_back(pointer->previous_edge);
			pointer = pointer->previous;
		}
	}

	std::reverse(path.begin(), path.end());

	return path;
}