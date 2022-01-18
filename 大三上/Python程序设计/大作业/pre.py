import numpy as np

def save_2_csv(original_file, target_file):
    data = np.loadtxt(original_file, skiprows=6)
    f = open(target_file, 'a')
    for i in range(10800):
        for j in range(10800):
            if data[i][j] > 0:
                f.write('%i %i %.8f\n' % (i, j, data[i][j]))

if __name__ == '__main__':
    for i in range(1, 9):
        original_file = str(i) + '.asc'
        target_file = str(i) + '.csv'
        save_2_csv(original_file, target_file)
