import aiohttp
import asyncio
import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.basemap import Basemap

# 存储不同人口密集度的点集，包括<1, 1-5, 5-25, 25-250, 250-1000, >1000(persons)
population_points_1 = []
population_points_2 = []
population_points_3 = []
population_points_4 = []
population_points_5 = []
population_points_6 = []


async def main(url, data):
    # 设置超时时间
    timeout = aiohttp.ClientTimeout(total=1000)
    # 客户端会话
    async with aiohttp.ClientSession() as session:
        async with session.get(url=url, json=data, timeout=timeout) as response:
            # 阻塞等待服务端消息
            population_data = await response.json()
            # 输出人口总数
            print(population_data['population_total'])
            # print(f'content-type={response.headers["content-type"]}')
            # 将人口点集按照密集度分类
            get_population_classification(population_data['population_points'])
            # 绘制人口散点图
            draw_population_scatter()


def draw_population_scatter():
    # 设置图片大小
    plt.figure(figsize=(10, 8))
    # 绘制基础地图，可以设置经纬度的范围
    m = Basemap(llcrnrlat=0, urcrnrlat=60, llcrnrlon=-140, urcrnrlon=-70)
    # 绘制海岸线
    m.drawcoastlines()
    # 绘制边界
    m.drawmapboundary(fill_color='white')
    # 填充大陆
    m.fillcontinents(color='none', lake_color='white')
    
    # 绘制经纬度点的分割
    parallels = np.arange(-90., 90., 10.)
    m.drawparallels(parallels, labels=[False, True, True, False])
    meridians = np.arange(-180., 180., 20.)
    m.drawmeridians(meridians, labels=[True, False, False, True])

    # 绘制多边形边界
    polygon = np.array([(-52.3, 45.6), (-40.5, 47.8), (-30.6, 35.1), (-52.3, 45.6)])
    m.plot(polygon[:, 0], polygon[:, 1], c='k', lw=1)

    # 使用np.array初始化人口点集
    points_1 = np.array(population_points_1)
    points_2 = np.array(population_points_2)
    points_3 = np.array(population_points_3)
    points_4 = np.array(population_points_4)
    points_5 = np.array(population_points_5)
    points_6 = np.array(population_points_6)

    # 绘制散点图
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
            
    # 添加图例
    plt.legend(loc='lower left', markerscale=100)
    plt.show()


def get_population_classification(population_points):
    # 通过人口数值给当前点分类，包括<1, 1-5, 5-25, 25-250, 250-1000, >1000(persons)
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
    url = 'http://127.0.0.1:8000/population'
    # 构造GeoJSON的多边形数据
    data = {
        'type': 'Polygon',
        'coordinates': [
            # 这里是全美数据，可自行修改进行查询
            (-127, 50), (-73, 50), (-73, 25), (-127, 25), (-127, 50)
        ]
    }
    asyncio.run(main(url, data))


data = {
    'type': 'Polygon',
    'coordinates': [
        # 这里是全美数据，可自行修改进行查询
        (-127, 50), (-73, 50), (-73, 25), (-127, 25), (-127, 50)
    ]
}