#pragma once

#include <cmath>

class Edge {
public:

	bool skip = false;
	bool lock = false;

	double distance = -1;
	double weight = -1;

	int source_id = -1;
	int destination_id = -1;

public:

	Edge();
	Edge(int source_id, int destination_id);
	void set_weight(double distance, double d);

	bool operator <(const Edge& other) const 
	{
		return this->weight < other.weight;
	}

	bool operator ==(const Edge& other) const 
	{
		return this->weight == other.weight;
	}

	//TODO: other dijkstra related fields
};