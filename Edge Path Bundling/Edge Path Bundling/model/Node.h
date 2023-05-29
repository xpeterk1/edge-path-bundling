#pragma once

#include <string>
#include <vector>
#include <climits>
#include <memory>

#include "Edge.h"

class Node
{
public:

	int ID;
	float longitude;
	float latitude;
	std::string name;
	std::vector<std::shared_ptr<Edge>> edges;

	// dijkstra related fields
	double distance = DBL_MAX; // -1 = not visited, other value = distance
	bool visited = false;
	Node* previous;

public:

	Node(int ID, float longitude, float latitude, std::string name);
	double distance_to(Node& other);

	bool operator <(const Node& other) const
	{
		return this->distance < other.distance;
	}

	bool operator >(const Node& other) const
	{
		return this->distance > other.distance;
	}

	bool operator ==(const Node& other) const
	{
		return this->distance == other.distance;
	}

};

