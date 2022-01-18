from multiprocessing.context import Process
from os import PathLike
from sanic import Sanic
from sanic.response import json
from sanic.response import text
from multiprocessing import Pool, Manager
import numpy as np
from time import time
from shapely import geometry

app = Sanic('Population')

data_1 = {}
data_2 = {}
population_total = 0


@app.route('/population')
async def population(request):
    polygon = request.json['coordinates']
    t1 = time()
    points = calc_population(polygon)
    t2 = time()
    print('spend', t2 - t1, 's')
    population_data = {
        'population_total': population_total,
        'population_points': points
    }
    return json(population_data)


def calc_population(lonLats):
    polygon = geometry.Polygon(lonLats)
    lonMin, latMin, lonMax, latMax = polygon.bounds
    step = 30 / 3600
    cellArea = geometry.Polygon(
        [(0, 0), (0, step), (step, step), (step, 0)]).area
    global population_total
    population_total = 0
    points = []
    for lon in np.arange(lonMin, lonMax, step):
        for lat in np.arange(latMin, latMax, step):
            cellLon1 = lon - lon % step
            cellLon2 = lon - lon % step + step
            cellLat1 = lat - lat % step
            cellLat2 = lat - lat % step + step
            cellPolygon = geometry.Polygon(
                [(cellLon1, cellLat1), (cellLon1, cellLat2), (cellLon2, cellLat2), (cellLon2, cellLat1)])
            area = cellPolygon.convex_hull.intersection(
                polygon.convex_hull).area
            if area > 0.0:
                p = get_population_from_file(cellLon1, cellLat1)
                points.append([cellLon1, cellLat1, p])
                population_total += (area / cellArea) * p
    return points


def get_population_from_file(lon, lat):
    step = 30 / 3600
    x = int((lon - (-180)) / step)
    y = int((lat - (-90)) / step)
    if x >= 0 and x < 10800 and y >= 10800 and y < 21600:
        if (21600 - y, x) in data_1.keys():
            return data_1[(21600 - y, x)]
        else:
            return 0
    elif x >= 10800 and x < 21600 and y >= 10800 and y < 21600:
        if (21600 - y, x - 10800) in data_2.keys():
            return data_2[(21600 - y, x - 10800)]
        else:
            return 0


def get_population_data(file):
    data = {}
    f = open(file)
    for line in f.readlines():
        line = line.split()
        data[(int(line[0]), int(line[1]))] = float(line[2])
    return data


def concurrent_process():
    global data_1, data_2
    files = ['1.csv', '2.csv']
    pool = Pool(10)
    data_1, data_2 = pool.map(get_population_data, files)
    pool.close()
    pool.join()


if __name__ == '__main__':
    t1 = time()
    concurrent_process()
    t2 = time()
    print('spend', t2 - t1, 's')
    app.run(host='127.0.0.1', port=8000)
