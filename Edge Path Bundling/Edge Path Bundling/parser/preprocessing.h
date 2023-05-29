#pragma once
#include <vector>
#include <fstream>

#include "../model/Edge.h"


void remove_edge_duplicates(std::vector<Edge> edges)
{
	// check for duplicate edges, also remove directed edges
	// note: O(n^2), could be done faster, however, we only have a small dataset...
	int counter = 0;
	for (auto it = edges.begin(); it < edges.end(); /*NOTHING HERE!*/)
	{
		bool duplicate = false;
		for (auto it2 = it + 1; it2 < edges.end(); ++it2)
		{
			// both iterators are pointing to the same edge
			if (it == it2)
			{
				int aaa = 2;
				continue;
			}

			if (it->source_id == 1 && it->destination_id == 5 && it2->source_id == 5 && it2->destination_id == 1)
			{
				int a = 3;
			}

			// different edge instances but: a) same IDs and direction, b) same IDs with opposite direction
			if ((it->source_id == it2->source_id && it->destination_id == it2->destination_id) || (it->source_id == it2->destination_id && it->destination_id == it2->source_id))
			{
				duplicate = true;
				break;
			}
		}

		// remove if duplicate
		if (duplicate)
		{
			if (it + 1 == edges.end())
			{
				int b = 3;
				edges.pop_back();
				break;
			}

			it = edges.erase(it);
		}
		else
		{
			// important!!! can't be in the for loop - edges.erase(it) already moves the iterator. it++ in the foor loop would result in jump 2 long when erasing the element
			it++;
		}

		counter++;
	}

	std::ofstream myfile;
	myfile.open("routes_preprocessed.csv");
	myfile << "source id, destination id\n";

	for (const auto& e : edges)
	{
		myfile << e.source_id << "," << e.destination_id << std::endl;
	}
	myfile.close();
}