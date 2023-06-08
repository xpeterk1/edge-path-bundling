#pragma once
#include "Edge.h"

Edge::Edge()
{}

Edge::Edge(int source_id, int destination_id)
{
	this->source_id = source_id;
	this->destination_id = destination_id;
}

void Edge::set_weight(double distance, double d)
{
	this->distance = distance;
	this->weight = pow(distance, d);
}