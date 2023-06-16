import math


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

    def distance_to(self, other, spherical = False) -> float:
        if spherical: 
            thetaS = (math.pi/2) - self.latitude
            thetaD = (math.pi/2) - other.latitude
            phiS = self.longitude
            phiD = other.longitude
            return math.sqrt(2 - 2*(math.sin(thetaS)*math.sin(thetaD)*math.cos(phiS-phiD) + math.cos(thetaS)*math.cos(thetaD))) 
        else:
            return math.sqrt(pow(other.longitude - self.longitude, 2) + pow(other.latitude - self.latitude, 2))

    def __lt__(self, other):
        return self.id < other.id

    def __gt__(self, other):
        return self.id > other.id

    def __le__(self, other):
        return self.id <= other.id

    def __ge__(self, other):
        return self.id >= other.id
