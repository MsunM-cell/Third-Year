from sanic import Sanic
from sanic.response import json
from multiprocessing import Pool
import numpy as np
from shapely import geometry

app = Sanic('Population')

# 全局变量，用于存储墨卡托投影平面等分8块的人口数据
data_1, data_2, data_3, data_4 = {}, {}, {}, {}
data_5, data_6, data_7, data_8 = {}, {}, {}, {}
# 全局变量，记录人口总数
population_total = 0


@app.route('/population')
async def population(request):
    # 获取客户端传来的多边形json数据中的经纬度点对集合
    polygon = request.json['coordinates']
    # 计算人口总数，并得到返回的点集，用于客户端绘制散点图
    points = calc_population(polygon)
    # 构造返回给客户端的json数据，包括人口总数和人口点集（经度，纬度，人口数值）
    population_data = {
        'population_total': population_total,
        'population_points': points
    }
    # 以json格式回传给客户端
    return json(population_data)


def calc_population(lonLats):
    # 使用Polygon类构造多边形
    polygon = geometry.Polygon(lonLats)
    # 获取多边形边界
    lonMin, latMin, lonMax, latMax = polygon.bounds
    # 步长为30角秒，转化成角度
    step = 30 / 3600
    # 获取每一个grid的面积area
    cellArea = geometry.Polygon(
        [(0, 0), (0, step), (step, step), (step, 0)]).area
    global population_total
    population_total = 0
    points = []
    # 根据bound遍历所包含的所有grid列表
    for lon in np.arange(lonMin, lonMax, step):
        for lat in np.arange(latMin, latMax, step):
            cellLon1 = lon - lon % step
            cellLon2 = lon - lon % step + step
            cellLat1 = lat - lat % step
            cellLat2 = lat - lat % step + step
            # 构造当前grid的多边形模型
            cellPolygon = geometry.Polygon(
                [(cellLon1, cellLat1), (cellLon1, cellLat2), (cellLon2, cellLat2), (cellLon2, cellLat1)])
            # 计算当前grid和多边形的相交面积
            area = cellPolygon.convex_hull.intersection(
                polygon.convex_hull).area
            # 如果面积不为零，则进一步获取该grid的人口数值，并根据相交面积近似获取多边形在该grid中的人口数值
            if area > 0.0:
                p = get_population_from_file(cellLon1, cellLat1)
                # 添加到绘制点对集合
                points.append([cellLon1, cellLat1, p])
                population_total += (area / cellArea) * p
    return points


def get_population_from_file(lon, lat):
    step = 30 / 3600
    # 计算经纬度映射的序号对
    x = int((lon - (-180)) / step)
    y = int((lat - (-90)) / step)
    # 根据序号对判断该grid位于墨卡托投影平面等分8块的哪一块中
    # 如果序号对存在于人口数据的键值集合中，则根据键找出人口数值，否则返回0
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
    elif x >= 21600 and x < 32400 and y >= 10800 and y < 21600:
        if (21600 - y, x - 21600) in data_3.keys():
            return data_3[(21600 - y, x - 21600)]
        else:
            return 0
    elif x >= 32400 and x < 43200 and y >= 10800 and y < 21600:
        if (21600 - y, x - 32400) in data_4.keys():
            return data_4[(21600 - y, x - 32400)]
        else:
            return 0
    elif x >= 0 and x < 10800 and y >= 0 and y < 10800:
        if (10800 - y, x) in data_5.keys():
            return data_5[(10800 - y, x)]
        else:
            return 0
    elif x >= 10800 and x < 21600 and y >= 0 and y < 10800:
        if (10800 - y, x - 10800) in data_6.keys():
            return data_6[(10800 - y, x - 10800)]
        else:
            return 0
    elif x >= 21600 and x < 32400 and y >= 0 and y < 10800:
        if (10800 - y, x - 21600) in data_7.keys():
            return data_7[(10800 - y, x - 21600)]
        else:
            return 0
    elif x >= 32400 and x < 43200 and y >= 0 and y < 10800:
        if (10800 - y, x - 32400) in data_8.keys():
            return data_8[(10800 - y, x - 32400)]
        else:
            return 0


def get_population_data(file):
    data = {}
    f = open(file)
    # 遍历人口数据文件中的每一行
    for line in f.readlines():
        # 对一行的数据进行拆分
        line = line.split()
        data[(int(line[0]), int(line[1]))] = float(line[2])
    return data


def concurrent_process():
    global data_1, data_2, data_3, data_4, data_5, data_6, data_7, data_8
    files = ['1.csv', '2.csv', '3.csv', '4.csv',
             '5.csv', '6.csv', '7.csv', '8.csv']
    # 引入进程池，池里最大进程数目为10
    pool = Pool(10)
    data_1, data_2, data_3, data_4, data_5, data_6, data_7, data_8 = pool.map(
        get_population_data, files)
    # 不允许更多的进程加入
    pool.close()
    # 阻塞，等待所有子进程完成
    pool.join()


if __name__ == '__main__':
    # 并行读取人口数据，加快处理速度
    concurrent_process()
    # 运行本地主机，端口号8000
    app.run(host='127.0.0.1', port=8000)
