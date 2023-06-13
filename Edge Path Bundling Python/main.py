import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import math
import typing
from typing import List
from tqdm import tqdm
import bezier as bz
import geopandas as gpd
import descartes
from shapely.geometry import Point, Polygon
import math
from model import Edge, Node
import airports
import migrations
import heapq


def find_shortest_path(source: Node, dest: Node, nodes, edges) -> List[Edge]:
    # reset nodes
    for node in nodes.values():
        node.distance = 99999999999999
        node.visited = False
        node.previous = None
        node.previous_edge = None

    source.distance = 0
    queue = []
    heapq.heappush(queue, (source.distance, source))

    while not len(queue) == 0:
        next_node = heapq.heappop(queue)[1]
        next_node.visited = True

        for edge in next_node.edges:
            if edge.skip:
                continue

            other_id = edge.destination if edge.source == next_node.id else edge.source
            other = nodes[other_id]

            current_distance = next_node.distance + edge.distance

            if current_distance < other.distance:
                other.distance = current_distance
                other.previous = next_node
                other.previous_edge = edge
                heapq.heappush(queue, (other.distance, other))

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


# MAIN CYCLE
d = 3.0
k = 2

nodes, edges = airports.get_airpors_data(d)
#nodes, edges = migrations.get_migrations_data(d)

controlPointLists = []
too_long = 0
no_path = 0
for edge in tqdm(edges, desc="Computing: "):
    if edge.lock:
        continue

    edge.skip = True

    source = nodes[edge.source]
    dest = nodes[edge.destination]

    path = find_shortest_path(source, dest, nodes, edges)

    if len(path) == 0:
        no_path += 1
        edge.skip = False
        continue

    original_edge_distance = source.distance_to(dest)
    new_path_length = sum([e.distance for e in path])

    if new_path_length > k * original_edge_distance:
        too_long += 1
        edge.skip = False
        continue

    for edge_in_path in path:
        edge_in_path.lock = True

    # Get control points for drawing
    controlPoints = []
    current_node = source
    for edge_in_path in path:
        controlPoints.append(np.array([current_node.longitude, current_node.latitude]))

        other_node_id = edge_in_path.destination if edge_in_path.source == current_node.id else edge_in_path.source
        current_node = nodes[other_node_id]

    controlPoints.append(np.array([dest.longitude, dest.latitude]))
    controlPointLists.append(controlPoints)


nodes_list = pd.read_csv("data/airports-extended.csv")
map = gpd.read_file('data/maps/World_Countries.shp')
geometry = [Point(xy) for xy in zip(nodes_list['8'], nodes_list['7'])]

geo_df = gpd.GeoDataFrame(nodes_list, crs='epsg:4326', geometry=geometry)

fig, ax = plt.subplots(figsize=(50, 50))

map.plot(ax=ax, alpha=0.4, color='grey')
geo_df.plot(ax=ax, markersize=1)


# create bezier curves 
bezierPolygons =[]
for controlPoints in tqdm(controlPointLists, desc="Drawing: "):
    n = 100
    polygon = bz.createBezierPolygon(controlPoints, n) # returns list of 2d vectors
    bezierPolygons.append(polygon)
    y = [arr[0] for arr in polygon]
    x = [arr[1] for arr in polygon]
    plt.plot(x, y, color='red', linewidth=0.1)
plt.show()

print(f"Out of {len(edges)}, {too_long} had too long detour and {no_path} had no alternative path.")


