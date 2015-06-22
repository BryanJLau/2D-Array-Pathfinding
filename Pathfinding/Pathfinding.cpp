// Pathfinding.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "conio.h"
#include "stdio.h"
#include "string.h"
#include "Area.h"
#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	Area* a = new Area(10, 10, 0, 0);
	int s = 0;
	while (s < 100) {
		cout << "Floor" << endl;
		a->printArray();

		cout << "1 to set, 2 to pathfind";
		cin >> s;
		switch (s) {
		case 1: 
			int x;
			cout << "x: ";
			cin >> x;

			int z;
			cout << "z: ";
			cin >> z;

			int t;
			cout << "t: ";
			cin >> t;

			a->setTile(z, x, t);
			break;
		case 2:
			cout << "Path" << endl << endl;
			a->fillPaths();
			a->printPaths();
			break;
		default:
			break;
		}
	}
	return 0;
}

