import aiohttp
import asyncio
import ujson
import json
import matplotlib.pyplot as plt
import numpy as np
from multiprocessing import Pool
from mpl_toolkits.basemap import Basemap
from time import time

population_points_1 = []
population_points_2 = []
population_points_3 = []
population_points_4 = []
population_points_5 = []
population_points_6 = []


async def main(url, data):
    timeout = aiohttp.ClientTimeout(total=20000)
    async with aiohttp.ClientSession() as session:
        async with session.get(url=url, json=data, timeout=timeout) as response:
            population_data = await response.json()
            print(population_data['population_total'])
            # print(f'content-type={response.headers["content-type"]}')
            get_population_classification(population_data['population_points'])
            draw_population_scatter()


def draw_population_scatter():
    plt.figure(figsize=(10, 8))
    m = Basemap(llcrnrlat=0, urcrnrlat=60, llcrnrlon=-140, urcrnrlon=-70)
    m.drawcoastlines()
    m.drawmapboundary(fill_color='white')
    m.fillcontinents(color='none', lake_color='white')

    parallels = np.arange(-90., 90., 10.)
    m.drawparallels(parallels, labels=[False, True, True, False])
    meridians = np.arange(-180., 180., 20.)
    m.drawmeridians(meridians, labels=[True, False, False, True])

    polygon = np.array([(-100, 43), (-95, 40), (-98, 35), (-102, 35), (-105, 40), (-100, 43)])
    m.plot(polygon[:, 0], polygon[:, 1], c='k', lw=1)

    points_1 = np.array(population_points_1)
    points_2 = np.array(population_points_2)
    points_3 = np.array(population_points_3)
    points_4 = np.array(population_points_4)
    points_5 = np.array(population_points_5)
    points_6 = np.array(population_points_6)

    if len(points_1) > 0:
        p1 = m.scatter(
            points_1[:, 0], points_1[:, 1], marker='o', s=0.01, c='#fefde1', label='<1')
    if len(points_2) > 0:
        p2 = m.scatter(
            points_2[:, 0], points_2[:, 1], marker='o', s=0.01, c='#adcab4', label='1-5')
    if len(points_3) > 0:
        p3 = m.scatter(
            points_3[:, 0], points_3[:, 1], marker='o', s=0.01, c='#509894', label='5-25')
    if len(points_4) > 0:    
        p4 = m.scatter(
            points_4[:, 0], points_4[:, 1], marker='o', s=0.01, c='#227295', label='25-250')
    if len(points_5) > 0:
        p5 = m.scatter(
            points_5[:, 0], points_5[:, 1], marker='o', s=0.01, c='#2b458f', label='250-1000')
    if len(points_6) > 0:
        p6 = m.scatter(
            points_6[:, 0], points_6[:, 1], marker='o', s=0.01, c='#030064', label='>1,000(persons)')
    plt.legend(loc='lower left', markerscale=100)
    plt.show()


def get_population_classification(population_points):
    for p in population_points:
        if p[2] >= 0 and p[2] < 1:
            population_points_1.append(p)
        elif p[2] >= 1 and p[2] < 5:
            population_points_2.append(p)
        elif p[2] >= 5 and p[2] < 25:
            population_points_3.append(p)
        elif p[2] >= 25 and p[2] < 250:
            population_points_4.append(p)
        elif p[2] >= 250 and p[2] < 1000:
            population_points_5.append(p)
        else:
            population_points_6.append(p)


if __name__ == '__main__':
    # parser = argparse.ArgumentParser(description='world temperature client')
    # parser.add_argument('--start', dest='start', type=int, default=1880)
    # parser.add_argument('--end', dest='end', type=int, default=2020)
    # parser.add_argument('--format', dest='format', default='json')
    # parser.add_argument('host')
    # parser.add_argument('port')
    # args = parser.parse_args()
    # print(f'{args}')

    url = 'http://127.0.0.1:8000/population'
    data = {
        'type': 'Polygon',
        'coordinates': [
            # (-127, 50), (-73, 50), (-73, 25), (-127, 25), (-127, 50) 全美
            (-100, 43), (-95, 40), (-98, 35), (-102, 35), (-105, 40), (-100, 43)
        ]
    }

    asyncio.run(main(url, data))
