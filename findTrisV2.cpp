// File IO then build graph of adjacency
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>

#define rmax 20
#define rmax2 400

class Point {
	public:
		int depth;
		int x,y,z;
		Point* origin;
		Point(int xa, int ya, int za) 
		{
			depth = -1; origin = NULL;
			x = xa; y = ya; z = za; 
		}
};

void evalPoint(Point* eval, int depth);

bool isTri(Point* a, Point* b)
{
	if (a > b) return false;
	if (a->origin != b->origin) return false;

	int dist = (a->x - b->x)*(a->x - b->x); if (dist > rmax2) return false;
	dist += ((a->y - b->y)*(a->y - b->y)); if (dist > rmax2) return false;
	dist += ((a->z - b->z)*(a->z - b->z)); if (dist > rmax2) return false;

	return true;
}

bool isWithinDist(Point* a, Point* b)
{
	int dist = (a->x - b->x)*(a->x - b->x); if (dist > rmax2) return false;
	dist += ((a->y - b->y)*(a->y - b->y)); if (dist > rmax2) return false;
	dist += ((a->z - b->z)*(a->z - b->z)); if (dist > rmax2) return false;

	return true;
}

void printTri(Point* a, Point* b)
{
	Point* c = a->origin;
	
	// printf("Found Triangle: ");
	printf("%d %d %d ", a->x, a->y, a->z);
	printf("%d %d %d ", b->x, b->y, b->z);
	printf("%d %d %d\n", c->x, c->y, c->z);
}

// Recursive function to find all triangles without duplicates
void evalPoints(std::vector<Point*>* unevaluated, Point* eval, int depth)
{
	std::vector<Point*> peers;
	for (int i=0; i<unevaluated->size(); i++)
	{
		if (isWithinDist(eval, (*unevaluated)[i]))
		{
			(*unevaluated)[i]->depth = depth;
			(*unevaluated)[i]->origin = eval;
			peers.push_back((*unevaluated)[i]);
			unevaluated->erase(unevaluated->begin()+i);
		}
	}

	for (int i=0; i<peers.size(); i++)
	{
		for (int j=0; j<peers.size(); j++)
		{
			if (i==j) continue;
			if (isTri(peers[i], peers[j]))
			{
				printTri(peers[i], peers[j]);
			}
		}
	}

	for (int i=0; i<peers.size(); i++)
	{
		evalPoints(unevaluated, peers[i], depth+1);
	}
}

int main(int argc, char** argv) {
	// Read in file and output adjacency matrix
	std::ifstream file(argv[1]);
	int r = atoi(argv[2]);
	int N = atoi(argv[3]);

	std::vector<Point*> allPoints;

	for (int i=0; i<N; i++)
	{
		int x,y,z;
		file >> x;
		file >> y;
		file >> z;
		allPoints.push_back(new Point(x,y,z));
	}

	evalPoints(&allPoints, allPoints[0], 0);

	return 0;
}