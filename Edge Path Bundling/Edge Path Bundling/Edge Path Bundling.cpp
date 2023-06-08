#include <iostream>
#include <string>
#include "model/Graph.h"
#include "model/Edge.h"
#include "model/Node.h"
#include "parser/parser.h"
#include "model/bezier.h"

int main()
{
    std::string nodes_path = "data/airports.txt";
    std::string edges_path = "data/routes-preprocessed.csv";
    float d = 1.0;
    float k = 2.0;

    /*
    This graph is/has:
    
    a) Loaded nodes, edges
    b) No nodes without edges
    c) It is not oriented -> even though edges have 'direction', they should be treated as both way edges.
    d) Edges have computed real distances and weights using the d parameter. They are also sorted descending by this value.
    e) Edge references inside nodes are sorted according to their distance, ascending
    
    */

    Graph g = Parser().load(nodes_path, edges_path, d);
    std::vector<std::vector<Coordinates>> controlPointVectors;
    for (auto& edge : g.edges)
    {
        if (edge.get()->lock) continue;

        edge.get()->skip = true;
        Node& source = g.nodes.at(edge.get()->source_id);
        Node& dest = g.nodes.at(edge.get()->destination_id);
        
        // find shortest path using Dijkstra's algorithm
        std::vector<std::shared_ptr<Edge>> path = g.find_shortest_path(source, dest);

        // no path found
        if (path.empty())
        {
            edge.get()->skip = false;
            continue;
        }

        double path_length = 0;
        for (auto& edge : path) {
            //TODO: compute length using weight or distance of the edge?
            path_length += edge.get()->weight;
        }

        // new detour is more than k-times longer than direct route
        double original_edge_length = source.distance_to(dest);
        if (path_length > k * original_edge_length)
        {
            edge.get()->skip = false;
            continue;
        }

        // Lock every edge in the path so it wont be substituted in the future 
        std::vector<Coordinates> detourControlPoints;
        for (auto& edgeInPath : path) 
        {
            edgeInPath.get()->lock = true;
            int id = edgeInPath.get()->source_id;
            auto sourceNode = g.nodes.at(id);
            Coordinates point;(sourceNode.longitude, sourceNode.latitude);
            detourControlPoints.push_back(point);
        }
        int id = edge.get()->destination_id;
        auto destinationNode = g.nodes.at(id);
        Coordinates point(destinationNode.longitude, destinationNode.latitude);
        detourControlPoints.push_back(point);
        controlPointVectors.push_back(detourControlPoints); // final node must be added manually
    }
}