#pragma once

#include <vector>
#include <deque>

using namespace std;

class Area
{
public:
	Area(int z, int x, int sz, int sx);
	Area(int z, int x, int sz, int sx, vector<int> v);
	~Area();

	// Accessors
	bool isInBounds(int z, int x);
	bool isWalkable(int z, int x);
	int getIndex(int z, int x);

	// Mutators
	void setTile(int z, int x, int t);
	void fillPaths();

	// Debugging info
	void printArray();
	void printPaths();

private:
	void getPathLengthGrid(vector<int>& pathLengthGrid);

	// Represents a single tile in the array
	struct Cell {
		int x;
		int z;
	};

	int m_width;
	int m_height;
	Cell start;
	
	// Dynamic arrays for expandable restaurants
	vector<int> floor;				// Represents the actual floor of the restaurant
	vector<deque<Cell*>> paths;		// Holds the shortest distance path for tables only
};