#include "stdafx.h"
#include "Area.h"
#include "math.h"
#include <iostream>

using namespace std;

Area::Area(int z, int x, int sz, int sx)
{
	this->m_width = x;
	this->m_height = z;

	arr = vector<int>(x*z);
	paths = vector<stack<struct Slot*>*>(x * z);

	for (int i = 0; i < z; i++)
		for (int j = 0; j < x; j++)
			arr[getIndex(i, j)] = 0;

	// Mark the start
	setTile(sz, sx, 1);
}

Area::~Area()
{
	for (int i = 0; i < paths.size(); i++) {
		while (!paths[i]->empty()) {
			Slot* s = paths[i]->top();
			delete s;
			paths[i]->pop();
		}
		delete paths[i];
	}
}

void Area::setTile(int z, int x, int t) {
	if (x < m_width && z < m_height && x >= 0 && z >= 0)
		this->arr[getIndex(z, x)] = t;
}

void Area::printArray() {
	for (int z = 0; z < this->m_height; z++) {
		cout << "|";
		for (int x = 0; x < this->m_width; x++) {
			cout << arr[getIndex(z, x)] << "|";
		}
		cout << endl;
	}
}

int Area::getIndex(int z, int x) {
	return x + m_height * z;
}

void Area::fillPaths() {
	vector<int> pathLengths(m_height * m_width);
	getPathLengthGrid(pathLengths);

	for (int z = 0; z < m_height; z++)
		for (int x = 0; x < m_width; x++) {
			if (arr[getIndex(z, x)] == 2) {
				// The path matrix doesn't have the path to obstacles. We need to get the surrounding min
				int minLength1 = fmin(pathLengths[getIndex(z, x + 1)], pathLengths[getIndex(z, x - 1)]);
				int minLength2 = fmin(pathLengths[getIndex(z + 1, x)], pathLengths[getIndex(z - 1, x)]);
				int minLength = fmin(minLength1, minLength2);
				int totalLength = minLength + 1;

				stack<Slot*>* currentStack = new stack<Slot*>;
				int xx = x;
				int zz = z;
				while (totalLength > 0) {
					if ((xx - 1) >= 0 && zz >= 0 && (xx-1) < m_width && 
							zz < m_height && pathLengths[getIndex(zz, xx - 1)] == (totalLength - 1)) {
						Slot* s = new Slot;
						s->x = --xx;
						s->z = zz;
						currentStack->push(s);

						totalLength--;
						continue;
					}

					if ((xx + 1) >= 0 && zz >= 0 && (xx + 1) < m_width &&
							zz < m_height && pathLengths[getIndex(zz, xx + 1)] == (totalLength - 1)) {
						Slot* s = new Slot;
						s->x = ++xx;
						s->z = zz;
						currentStack->push(s);

						totalLength--;
						continue;
					}

					if (xx >= 0 && (zz - 1) >= 0 && xx < m_width &&
							(zz - 1) < m_height && pathLengths[getIndex(zz - 1, xx)] == (totalLength - 1)) {
						Slot* s = new Slot;
						s->x = xx;
						s->z = --zz;
						currentStack->push(s);

						totalLength--;
						continue;
					}

					if (xx >= 0 && (zz + 1) >= 0 && xx < m_width &&
							(zz + 1) < m_height && pathLengths[getIndex(zz - 1, xx)] == (totalLength - 1)) {
						Slot* s = new Slot;
						s->x = xx;
						s->z = ++zz;
						currentStack->push(s);

						totalLength--;
						continue;
					}
				}

				paths[getIndex(z, x)] = currentStack;
			}
		}
}

void Area::getPathLengthGrid(vector<int>& pathLengthGrid) {
	stack<struct Slot> current, next;
	current.push(this->start);

	fill(pathLengthGrid.begin(), pathLengthGrid.end(), 0);

	int pathLength = 1;
	int size = this->arr.size();

	do {
		// Have to set next to empty after while comparison
		stack<struct Slot> emptyStack;
		next = emptyStack;

		while (!current.empty()) {
			Slot s = current.top();

			if (isWalkable(s.z, s.x - 1) && pathLengthGrid[getIndex(s.z, s.x - 1)] == 0) {
				pathLengthGrid[getIndex(s.z, s.x - 1)] = pathLength;
				Slot newSlot;
				newSlot.x = s.x - 1;
				newSlot.z = s.z;
				next.push(newSlot);
			}

			if (isWalkable(s.z, s.x + 1) && pathLengthGrid[getIndex(s.z, s.x + 1)] == 0) {
				pathLengthGrid[getIndex(s.z, s.x + 1)] = pathLength;
				Slot newSlot;
				newSlot.x = s.x + 1;
				newSlot.z = s.z;
				next.push(newSlot);
			}

			if (isWalkable(s.z - 1, s.x) && pathLengthGrid[getIndex(s.z - 1, s.x)] == 0) {
				pathLengthGrid[getIndex(s.z - 1, s.x)] = pathLength;
				Slot newSlot;
				newSlot.x = s.x;
				newSlot.z = s.z - 1;
				next.push(newSlot);
			}

			if (isWalkable(s.z + 1, s.x) && pathLengthGrid[getIndex(s.z + 1, s.x)] == 0) {
				pathLengthGrid[getIndex(s.z + 1, s.x)] = pathLength;
				Slot newSlot;
				newSlot.x = s.x;
				newSlot.z = s.z + 1;
				next.push(newSlot);
			}

			current.pop();
		}

		pathLength++;
		current = next;
	} while (!next.empty());

	for (int i = 0; i < this->m_height; i++) {
		cout << "|";
		for (int j = 0; j < this->m_width; j++) {
			cout << pathLengthGrid[getIndex(i, j)] << "|";
		}
		cout << endl;
	}
	cout << endl << endl;
	return;
}

void Area::printPaths() {
	vector<int> v(m_width * m_height);
	for (int z = 0; z < m_height; z++)
		for (int x = 0; x < m_width; x++) {
			if (arr[getIndex(z, x)] == 2) {
				stack<Slot*>* path = paths[getIndex(z, x)];

				while (!path->empty()) {
					Slot* top = path->top();
					v[getIndex(top->z, top->x)] = 8;
					path->pop();
				}
			}
		}

	for (int z = 0; z < this->m_height; z++) {
		cout << "|";
		for (int x = 0; x < this->m_width; x++) {
			cout << v[getIndex(z, x)] << "|";
		}
		cout << endl;
	}
	cout << endl << endl;
	return;
}

// 0 is walkable
bool Area::isWalkable(int z, int x) {
	// Out of bounds
	if (x < m_width && z < m_height && x >= 0 && z >= 0)
		return this->arr[getIndex(z, x)] == 0;

	return false;
}