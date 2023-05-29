#include "Node.h"

Node::Node(int ID, float longitude, float latitude, std::string name)
{
	this->ID = ID;
	this->longitude = longitude;
	this->latitude = latitude;
	this->name = name;

	this->edges = std::vector<std::shared_ptr<Edge>>();
}

double Node::distance_to(Node& other)
{
	//TODO: node-node distance
	return sqrt(pow(other.latitude - this->latitude, 2) + pow(other.longitude - this->longitude, 2));
}