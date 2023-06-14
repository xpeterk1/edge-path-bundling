import pandas as pd
from model import Edge, Node

def get_airpors_data(d):
    # Load data into dataframes
    nodes_list = pd.read_csv("data/airports-extended.csv")
    edges_list = pd.read_csv("data/routes-preprocessed.csv")

    nodes = {}
    edges = []

    # Load nodes into dict. Maps ID -> Node instance
    for index, row in nodes_list.iterrows():
        id = row['1']
        name = row['2']
        lat = row['7']
        long = row['8']
        nodes[id] = Node(id, long, lat, name)

    # Load edges to list
    for index, row in edges_list.iterrows():
        so = row['source_id']
        dest = row['destination_id']
        edges.append(Edge(so, dest))

    # Assign edges to nodes
    for edge in edges:

        # eliminate edges without nodes
        if edge.destination not in nodes or edge.source not in nodes:
            edges.remove(edge)

        source = nodes[edge.source]
        dest = nodes[edge.destination]
        distance = source.distance_to(dest)

        edge.distance = distance
        edge.weight = pow(distance, d)

        source.edges.append(edge)
        dest.edges.append(edge)

    # Eliminate nodes without edges
    to_remove = [node.id for node in nodes.values() if len(node.edges) == 0]
    for key in to_remove:
        del nodes[key]

    # Sort edges inside nodes in ascending order
    for node in nodes.values():
        node.edges.sort(key=lambda x: x.distance)

    # Sort edges
    edges.sort(key=lambda x: x.weight, reverse=True)
    return nodes, edges
