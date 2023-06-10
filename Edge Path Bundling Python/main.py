import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import math
import typing
from typing import List
from queue import PriorityQueue
from tqdm import tqdm

d = 1.2
k = 2

class Edge:

    def __init__(self, source, destination):
        self.source = source
        self.destination = destination
        self.distance = -1
        self.weight = -1
        self.skip = False
        self.lock = False


class Node:

    def __init__(self, id, longitude, latitude, name):
        self.id = id
        self.longitude = longitude
        self.latitude = latitude
        self.name = name
        self.edges = []

        # dijkstra related attributes
        self.distance = -1
        self.visited = False
        self.previous = None
        self.previous_edge = None

    def distance_to(self, other) -> float:
        return math.sqrt(pow(other.longitude - self.longitude, 2) + pow(other.latitude - self.latitude, 2))


def find_shortest_path(source: Node, dest: Node) -> List[Edge]:
    # reset nodes
    for node in nodes.values():
        node.distance = 99999999999999
        node.visited = False
        node.previous = None
        node.previsou_edge = None

    source.distance = 0

    queue = PriorityQueue()
    queue.put((source.distance, source))

    while not queue.empty():
        next_node = queue.get()[1]
        next_node.visited = True

        for edge in next_node.edges:
            if edge.skip:
                continue

            other_id = edge.destination if edge.source == next_node.id else edge.source
            other = nodes[other_id]

            current_distance = next_node.distance + edge.weight

            if current_distance < other.distance:
                other.distance = current_distance
                other.previous = next_node
                other.previous_edge = edge
                queue.put((other.distance, other))

        # Already found the destination, no need to continue with the search
        if next_node == dest:
            break

    # extract path
    path = []

    node = dest
    while node.previous is not None:
        path.append(node.previous_edge)
        node = node.previous

    path.reverse()
    return path


# Load data into dataframes
nodes_list = pd.read_csv("data/airports-extended.csv")
edges_list = pd.read_csv("data/routes-preprocessed.csv")

nodes = {}
edges = []

# Load nodes into dict. Maps ID -> Node instance
for index, row in nodes_list.iterrows():
    id = row['1']
    name = row['2']
    long = row['7']
    lat = row['8']
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
    edge.weight = pow(distance, k)

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


# MAIN CYCLE
for edge in tqdm(edges):
    if edge.lock:
        continue

    edge.skip = True

    source = nodes[edge.source]
    dest = nodes[edge.destination]

    path = find_shortest_path(source, dest)

    if len(path) == 0:
        edge.skip = False

    original_edge_distance = source.distance_to(dest)
    new_path_length = sum([e.weight for e in path])

    if new_path_length > k * original_edge_distance:
        edge.skip = False
        continue

    for edge in path:
        edge.lock = True
    # TODO: control points + draw

