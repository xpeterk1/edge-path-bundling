import numpy as np
import matplotlib.pyplot as plt
import json
from model import Edge, Node


def get_migrations_data(d):
    f = open('data/migrationsTest.json')
    data = json.load(f)

    nodes_list = data['nodes']
    edges_list = data['links']

    nodes = {}
    edges = []

    for edge_entry in edges_list:
        edges.append(Edge(edge_entry['source'], edge_entry['target']))

    for node_entry in nodes_list:
        node_id = node_entry['id']
        n = Node(node_id, node_entry['x'], node_entry['y'], "")
        nodes[node_id] = n

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

    f.close()
    return nodes, edges
