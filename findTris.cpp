/*
import numpy as np

x, y, z = np.loadtxt("inputfile.dat")

rmax = 50
rmax_squared = rmax * rmax

N = np.size(x)
counts = np.zeros((N,N,N))

for i in range(N):
    for j in range(N):
        for k in range(N):
            d12 = (x[i] - x[j])**2 + (y[i] - y[j])**2 + (z[i] - z[j])**2
            d23 = (x[j] - x[k])**2 + (y[j] - y[k])**2 + (z[j] - z[k])**2
            d31 = (x[k] - x[i])**2 + (y[k] - y[i])**2 + (z[k] - z[i])**2
            if d12> rmax_squared or d23 > rmax_squared or d31 > rmax_squared:
                continue
            else:
                counts[int(np.sqrt(d12))][int(np.sqrt(d23))][int(np.sqrt(d31))] += 1

# save the 3D array counts

*/
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

// argv[1] = input file
// argv[2] = rmax
// argv[3] = points
int main(int argc, char** argv) {
	std::ifstream file(argv[1]);
	int rmax = atoi(argv[2]);
	int N = atoi(argv[3]);

	std::vector<int> xV,yV,zV;
	
	if (file.is_open())
	{
		int x,y,z;
		for (int i=0; i<N; i++)
		{
			file >> x;
			xV.push_back(x);
		}
		for (int i=0; i<N; i++)
		{
			file >> y;
			yV.push_back(y);
		}
		for (int i=0; i<N; i++)
		{
			file >> z;
			zV.push_back(z);
		}
		file.close();
	}

	int*** counts = (int***)std::malloc(sizeof(int***)*N);
	for (int i=0; i<N; i++)
	{
		counts[i] = (int**)std::malloc(sizeof(int**)*N);
		for (int j=0; j<N; j++)
		{
			counts[i][j] = (int*)std::malloc(sizeof(int*)*N);
			for (int k=0; k<N; k++)
			{
				counts[i][j][k] = 0;
			}
		}
	}

	for (int i=0; i<N; i++)
	for (int j=0; j<N; j++)
	for (int k=0; k<N; k++)
	{
		int xixj = (xV[i] - xV[j]);
		int xjxk = (xV[j] - xV[k]);
		int xkxi = (xV[k] - xV[i]);
		int yiyj = (yV[i] - yV[j]);
		int yjyk = (yV[j] - yV[k]);
		int ykyi = (yV[k] - yV[i]);
		int zizj = (zV[i] - zV[j]);
		int zjzk = (zV[j] - zV[k]);
		int zkzi = (zV[k] - zV[i]);
		
		float d12 = std::sqrt(
			(xV[i] - xV[j])*(xV[i] - xV[j]) + 
			(yV[i] - yV[j])*(yV[i] - yV[j]) + 
			(zV[i] - zV[j])*(zV[i] - zV[j])
		);
		float d23 = std::sqrt(
			(xV[j] - xV[k])*(xV[j] - xV[k]) + 
			(yV[j] - yV[k])*(yV[j] - yV[k]) + 
			(zV[j] - zV[k])*(zV[j] - zV[k])
		);
		float d31 = std::sqrt(
			(xV[k] - xV[i])*(xV[k] - xV[i]) + 
			(yV[k] - yV[i])*(yV[k] - yV[i]) + 
			(zV[k] - zV[i])*(zV[k] - zV[i])
		);

		if (d12 > rmax || d23 > rmax || d31 > rmax) continue;
		else {
			counts[int(d12)][int(d23)][int(d31)] += 1;
		}
	}

	for (int i=0; i<N; i++)
	for (int j=0; j<N; j++)
	for (int k=0; k<N; k++)
	try {
		std::cout << counts[i][j][k] << std::endl;		
	} catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}	
