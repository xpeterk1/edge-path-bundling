#pragma once

#include <string>
#include <vector>

#include "Edge.h"

class Node
{
public:

	int ID;
	float longitude;
	float latitude;
	std::string name;
	std::vector<Edge> edges;

public:

	Node(int ID, float longitude, float latitude, std::string name);
	double distance_to(Node& other);

	bool operator <(const Node& other) const
	{
		return this->ID < other.ID;
	}

	bool operator ==(const Node& other) const
	{
		return this->ID == other.ID;
	}

};

