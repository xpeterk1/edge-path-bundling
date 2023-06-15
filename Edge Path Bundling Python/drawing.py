import matplotlib.pyplot as plt
import geopandas as gpd
import pandas as pd

from bezier import bezierSpherical
from tqdm import tqdm
from shapely.geometry import Point
from bezier import bezier as bz


def draw(control_points, nodes, edges, n, use_3d, draw_map):

    if draw_map:
        nodes_list = pd.read_csv("data/airports-extended.csv")
        map = gpd.read_file('data/maps/World_Countries.shp')
        geometry = [Point(xy) for xy in zip(nodes_list['8'], nodes_list['7'])]

        geo_df = gpd.GeoDataFrame(nodes_list, crs='epsg:4326', geometry=geometry)

        fig, ax = plt.subplots(figsize=(50, 25))
        map.plot(ax=ax, alpha=0.4, color='grey')
        geo_df.plot(ax=ax, markersize=1)
    else:
        plt.gcf().set_dpi(300)

    if use_3d:
        n = -1
        step_size = 5
        bezierSpherical.plot_spherical(control_points, nodes, edges, n, step_size)
    else:
        # create and bezier curves
        bezier_polygons = []
        for controlPoints in tqdm(control_points, desc="Drawing curves: "):
            polygon = bz.create_bezier_polygon(controlPoints, n)  # returns list of 2d vectors
            bezier_polygons.append(polygon)
            x = [arr[0] for arr in polygon]
            y = [arr[1] for arr in polygon]
            plt.plot(x, y, color='red', linewidth=0.1, alpha=1)

        # draw lines without detour or with detour that was too long
        for edge in tqdm(edges, desc="Drawing lines: "):
            if edge.skip:
                continue

            s = nodes[edge.source]
            d = nodes[edge.destination]
            x = [s.longitude, d.longitude]
            y = [s.latitude, d.latitude]
            plt.plot(x, y, color='red', linewidth=0.1, alpha=1)

    plt.axis('scaled')
    plt.axis('off')
    plt.tight_layout()
    plt.show()
