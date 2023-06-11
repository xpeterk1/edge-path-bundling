import matplotlib.pyplot as plt
import numpy as np
import geopandas as gpd
import descartes
import pandas as pd
from shapely.geometry import Point, Polygon
import math

# https://towardsdatascience.com/geopandas-101-plot-any-data-with-a-latitude-and-longitude-on-a-map-98e01944b972

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

# MAIN DRAW

map = gpd.read_file('data/maps/World_Countries.shp')
geometry = [Point(xy) for xy in zip(nodes_list['8'], nodes_list['7'])]

geo_df = gpd.GeoDataFrame(nodes_list, crs='epsg:4326', geometry=geometry)

fig, ax = plt.subplots(figsize=(50, 50))

map.plot(ax=ax, alpha=0.4, color='grey')
geo_df.plot(ax=ax, markersize=1)

for edge in edges:
    s = nodes[edge.source]
    d = nodes[edge.destination]

    x = [s.latitude, d.latitude]
    y = [s.longitude, d.longitude]

    plt.plot(x, y, color='red')

plt.show()