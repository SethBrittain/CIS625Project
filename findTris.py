import numpy as np

x, y, z = np.loadtxt("testData.txt")

rmax = 50

N = np.size(x)
counts = np.zeros((N,N,N))

for i in range(N):
    for j in range(N):
        for k in range(N):
            d12 = np.sqrt((x[i] - x[j])**2 + (y[i] - y[j])**2 + (z[i] - z[j])**2)
            d23 = np.sqrt((x[j] - x[k])**2 + (y[j] - y[k])**2 + (z[j] - z[k])**2)
            d31 = np.sqrt((x[k] - x[i])**2 + (y[k] - y[i])**2 + (z[k] - z[i])**2)
            if d12> rmax or d23 > rmax or d31 > rmax:
                continue
            else:
                counts[int(d12)][int(d23)][int(d31)] += 1

# save the 3D array counts
