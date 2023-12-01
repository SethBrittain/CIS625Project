from itertools import chain

import matplotlib.pyplot as plt

from mpl_toolkits.mplot3d import Axes3D
from matplotlib.pyplot import show

triangles =  []

with (open("trioutput.csv", 'r')) as f:
	for line in f.readlines():
		coords = line.split(' ')
		triangles.append(((int(coords[0]), int(coords[1]), int(coords[2])), (int(coords[3]), int(coords[4]), int(coords[5])), (int(coords[6]), int(coords[7]), int(coords[8]))))

print(triangles)

# Convert the list of triangles into a "flat" list of points

tri_points = list(chain.from_iterable(triangles))

# Get the X, Y and Z coordinates of each point

x, y, z = zip(*tri_points)

# Make list of triangle indices ([(0, 1, 2), (3, 4, 5), ...])

tri_idx = [(3 * i, 3 * i + 1, 3 * i + 2) for i in range(len(triangles))]

# Make 3D axes

ax = plt.figure().add_subplot(projection='3d')

# Plot triangles

ax.plot_trisurf(x, y, z, triangles=tri_idx)

show()