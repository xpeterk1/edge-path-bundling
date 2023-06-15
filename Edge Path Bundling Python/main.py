import airports
import migrations
import dijkstra

from tqdm import tqdm
from bezier import control_points
from drawing import draw

from numba.core.errors import NumbaDeprecationWarning, NumbaPendingDeprecationWarning
import warnings

warnings.simplefilter('ignore', category=NumbaDeprecationWarning)
warnings.simplefilter('ignore', category=NumbaPendingDeprecationWarning)

############################################################################################
# PARAMETERS
############################################################################################

d = 2.0  # edge weight parameter, weight = distance^d
k = 2.0  # if new detour is longer than k times the original line it is not bundled
n = 100  # number of sampling points in Bezier curves
draw_map = True  # Draw map for the underlying data
plot_3d = True  # Plot for use on sphere
smoothing = 2  # Smoothing parameter for Bezier curves

# data source
nodes, edges = airports.get_airpors_data(d)
# nodes, edges = migrations.get_migrations_data(d)

############################################################################################
# MAIN CYCLE
############################################################################################

control_point_lists = []
too_long = 0
no_path = 0
for edge in tqdm(edges, desc="Computing: "):
    if edge.lock:
        continue

    edge.skip = True

    source = nodes[edge.source]
    dest = nodes[edge.destination]

    path = dijkstra.find_shortest_path(source, dest, nodes)

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
    control_point_lists.append(control_points.get(source, dest, nodes, path, smoothing))


############################################################################################
# DRAWING
############################################################################################
draw(control_point_lists, nodes, edges, n, plot_3d, draw_map)

print(f"Out of {len(edges)} edges, {too_long} had too long detour and {no_path} had no alternative path.")
