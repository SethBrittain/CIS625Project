#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <tbb/concurrent_unordered_map.h>
#include <tbb/parallel_for.h>
#include <tbb/tbb.h>

#define RMAX 20
#define RMAX2 400


typedef struct ISVALID {
	bool valid;
	int dist;
} ISVALID;

class Point {
	public:
		int x,y,z;
		Point(int xa, int ya, int za) {
			x = xa; y = ya; z = za; 
		}

		bool operator==(const Point& other) const {
			return (x == other.x && y == other.y && z == other.z);
		}
};

struct Edge {
	public:
		Point* a;
		Point* b;

		Edge(Point* aa, Point* bb) {
			a = aa; b = bb;
		}

		bool operator==(const Edge& other) const {
			return (a == other.a && b == other.b) || (a == other.b && b == other.a);
		}
};

struct Triplet {
	int a, b, c;

	bool operator==(const Triplet& other) const {
		return (a == other.a && b == other.b && c == other.c);
	}
};


void printPoint(Point& point)
{
	std::cout << "(" << point.x << "," << point.y << "," << point.z << ")" << std::endl;
}

template <>
struct tbb::tbb_hash<Edge>
{
  std::size_t operator()(const Edge& k) const
  {
    using std::size_t;
    using tbb::tbb_hash;

    // Compute individual hash values for first,
    // second and third and combine them using XOR
    // and bit shifting:
	size_t h1 = ((tbb_hash<int>()(k.a->x)
		^ (tbb_hash<int>()(k.a->y) << 1)) >> 1)
		^ (tbb_hash<int>()(k.a->z) << 1);

	size_t h2 = ((tbb_hash<int>()(k.b->x)
		^ (tbb_hash<int>()(k.b->y) << 1)) >> 1)
		^ (tbb_hash<int>()(k.b->z) << 1);
	
	return h1 ^ h2;
  }
};

template <>
struct tbb::tbb_hash<Triplet>
{
	std::size_t operator()(const Triplet& t) const
	{
		using std::size_t;
		using tbb::tbb_hash;

		// Compute individual hash values for first,
		// second and third and combine them using XOR
		// and bit shifting:
		size_t seed = 0;
		seed ^= tbb_hash<int>()(t.a) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= tbb_hash<int>()(t.b) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= tbb_hash<int>()(t.c) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

		return seed;
	}
};

void printProgressBar(const char fillChars[], const char emptyChars[], int count, int max, int chars)
{
	int toPrint = (int)((((float)count) / max) * chars);
	std::cout << "\033[2J\n[" << fillChars + chars - toPrint << emptyChars + toPrint << "]" << std::endl;
}

bool isWithinDist(Point* a, Point* b, float &dist)
{
	dist = (a->x - b->x)*(a->x - b->x); if (dist > RMAX2) return false;
	dist += ((a->y - b->y)*(a->y - b->y)); if (dist > RMAX2) return false;
	dist += ((a->z - b->z)*(a->z - b->z)); if (dist > RMAX2) return false;
	dist = sqrt(dist);
	return true;
}

bool isValidEdge(Edge &edge, tbb::concurrent_unordered_map<Edge, ISVALID> &edgeMap)
{
	if (edgeMap.find(edge) != edgeMap.end())
		return edgeMap.at(edge).valid; // valid
	else 
	{
		struct ISVALID result;
		float dist;
		result.valid = isWithinDist(edge.a, edge.b, dist);
		result.dist = (int)dist;
		edgeMap[edge] = result;
		return result.valid;
	}
}

void evalPoint(Point &eval, tbb::concurrent_vector<Point*> &points, tbb::concurrent_unordered_map<Edge, ISVALID> &edgeMap, tbb::concurrent_unordered_map<Triplet, long> &edgeCounts, tbb::concurrent_vector<Point*> &peers)
{
	// Setup peers
	peers.clear();
	for (int i=0; i<points.size(); i++)
	{
		if (eval == *(points[i])) continue;
		Edge* edge = new Edge(&eval, points[i]);
		if (isValidEdge(*edge, edgeMap))
			peers.push_back(points[i]);

		delete edge;
	}

	int peerSize = peers.size();
	for (int i=0; i<peerSize; i++)
	for (int j=0; j<peerSize; j++)
	{
		if (*(peers[i])==*(peers[j])) continue;
		Edge* edge = new Edge(peers[i], peers[j]);
		if (isValidEdge(*edge, edgeMap))
		{
			Edge* e2 = new Edge(peers[j], &eval);
			Edge* e3 = new Edge(&eval, peers[i]);

			Triplet trip{
				edgeMap.at(*edge).dist,
				edgeMap.at(*e2).dist,
				edgeMap.at(*e3).dist
			};
			delete edge;
			delete e2;
			delete e3;
			//if (trip.a==0||trip.b==0||trip.c==0) continue;
			
			if (edgeCounts.find(trip) == edgeCounts.end())
			{

				edgeCounts[trip] = 1;

			}
			else edgeCounts[trip]++;
		}
	}
}

/// @brief 
/// @param argc number of cmd lin
/// @param argv argv[1] is the input file, argv[2] is rmax, argv[3] is number of points
/// @return 
int main(int argc, char** argv)
{
	std::ifstream file(argv[1]);
	int N = atoi(argv[2]);
	tbb::concurrent_vector<Point*> allPoints;


	// parse input
	for (int i=0; i<N; i++)
	{
		int x,y,z;
		file >> x; file >> y; file >> z;
		allPoints.push_back(new Point(x,y,z));
	}

	// for each point in allPoints find all tris that include current point
	tbb::concurrent_unordered_map<Edge, ISVALID> edgeMap;
	// Results
	tbb::concurrent_unordered_map<Triplet, long> edgeCounts;

	tbb::enumerable_thread_specific<tbb::concurrent_vector<Point*>> privateVectors([]() { return tbb::concurrent_vector<Point*>(); });

	int total = 0;
	// true is valid, false is invalid
	tbb::parallel_for(0, (int)(allPoints.size()), [&privateVectors, &allPoints, &edgeMap, &edgeCounts, &N, &total](int i){
		tbb::concurrent_vector<Point*> peers = privateVectors.local();
		Point* cur = allPoints[i];
		evalPoint(*cur, allPoints, edgeMap, edgeCounts, peers);
		total++;
		//if (tbb::this_task_arena::current_thread_index() == 0)
		//	printProgressBar("===============", "               ", total, N, 15);
	});
	
	long sum = 0;
	for (auto it = edgeCounts.begin(); it != edgeCounts.end(); ++it) {
		const Triplet& key = it->first;
        const long& value = it->second;
		printf("%d %d %d, %ld\n", key.a, key.b, key.c, value);
		sum += value;
	}
	printf("Total: %ld\n", sum);

	return 0;
}
