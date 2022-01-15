import numpy as np
import matplotlib.pyplot as plt

plt.xlabel('longitude')
plt.ylabel('latitude')    

# in_data = np.loadtxt('in_centroid.csv', usecols=[0, 1])
# plt.scatter(in_data[:, 0], in_data[:, 1], c='red', marker='o', label='in')

# out_data = np.loadtxt('out_centroid.csv', usecols=[0, 1])
# plt.scatter(out_data[:, 0], out_data[:, 1], c='blue', marker='o', label='out')

mooring_data = np.loadtxt('mooring_centroid.csv', usecols=[0, 1])
plt.scatter(mooring_data[:, 0], mooring_data[:, 1],
            c='green', marker='o', label='see')

# plt.legend(('in', 'out', 'mooring'))
plt.legend(('mooring',))

plt.show()
