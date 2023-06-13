import numpy as np


def get(source, dest, nodes, path, smoothing):
    control_points = []
    current_node = source
    for edge_in_path in path:
        control_points.append(np.array([current_node.longitude, current_node.latitude]))

        other_node_id = edge_in_path.destination if edge_in_path.source == current_node.id else edge_in_path.source
        current_node = nodes[other_node_id]

    control_points.append(np.array([dest.longitude, dest.latitude]))
    return split(control_points, smoothing)


def split(points, smoothing):
    p = points
    # for each level of smoothing, insert new control point in the middle of all points already in the array
    # loop starts from 1 => smoothing level of 1 keeps only node points
    for smooth in range(1, smoothing):
        new_points = []
        for i in range(len(p) - 1):
            new_point = np.divide(p[i] + p[i + 1], 2.0)
            new_points.append(p[i])
            new_points.append(new_point)
        new_points.append(p[-1])
        p = new_points
    return p