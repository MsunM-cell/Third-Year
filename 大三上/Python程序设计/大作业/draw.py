from mpl_toolkits.basemap import Basemap
import numpy as np
import matplotlib.pyplot as plt

plt.figure(figsize=(10,8))
m = Basemap(llcrnrlat = 0, urcrnrlat = 60, llcrnrlon = -140, urcrnrlon = -70)         # 实例化一个map
m.drawcoastlines()    # 画海岸线
m.drawmapboundary(fill_color='white')    
# m.fillcontinents(color='white',lake_color='white') # 画大洲，颜色填充为白色
m.fillcontinents(color='none',lake_color='white') # 如果color设置为none,则地图为透明
 
parallels = np.arange(-90., 90., 10.)    # 这两行画纬度，范围为[-90,90]间隔为10
m.drawparallels(parallels,labels=[False, True, True, False])
meridians = np.arange(-180., 180., 20.)   # 这两行画经度，范围为[-180,180]间隔为10
m.drawmeridians(meridians,labels=[True, False, False, True])


# lon, lat = m(90, 0)    # lon, lat为给定的经纬度，可以使单个的，也可以是列表
a = [90, 100, 110]
b = [0, 0, 10]
# m.scatter(a, b, s=100, color='r')
size = [1, 10, 5]

a1 = [-90, -100, -110]
b1 = [0, 0, 10]
# m.scatter(a, b, s=100, color='r')
size1 = [1, 10, 5]

x = np.array([[10, 70], [-20, -40]])
y = m.plot(x[:, 0], x[:, 1], 'r-o')

p1 = m.scatter(a,b,marker='o',s=0.1, c='#FFFF00')
p2 = m.scatter(a1,b1,marker='o',s=0.1, c='b')
plt.legend([p1,p2],["First", "Second"], markerscale=20)
plt.show()