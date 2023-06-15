from model import Edge, Node
from typing import List
import heapq


def find_shortest_path(source: Node, dest: Node, nodes) -> List[Edge]:
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

            current_distance = next_node.distance + edge.weight

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