#pragma once

#include <stack>
#include <vector>
using namespace std;

class Area
{
public:
	Area(int z, int x, int sz, int sx);
	~Area();

	void setTile(int z, int x, int t);
	void printArray();
	void printPaths();

	int getIndex(int z, int x);

	void fillPaths();
	void getPathLengthGrid(vector<int>& pathLengthGrid);

	bool isWalkable(int z, int x);

private:
	struct Slot {
		int x;
		int z;
	};

	int m_width;
	int m_height;
	Slot start;
	vector<int> arr;
	vector<stack<Slot*>*> paths;
};