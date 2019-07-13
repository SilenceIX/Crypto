#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <omp.h>
#include "BDD.h"

using namespace std;

vector <vector <bool>> Ben =
{
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,1,1,0,1,0,0,1,1,0,0,0,1,1,1,0,1,1,0,1,0,0,0,1,0,0,1,1,1,1,0,0,0,0,1,1,1,0,0,0,1,0,1,1,0,1,1,0,1,0,0,0,1,0,1,0,0,1,1,0,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,0,1,1,0,1,1,0,1,0,0,1,0,1,0,1,0,1,1,0,0,1,0,1,1,0,0,1,0,0,1,1,1,1,0,1,0,0,0,0,1,1,1,1,0,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,1,0,0,1,1,1,0,0,1,0,1,0,1,0,1,0,1,1,1,0,1,0,0,0,0,1,1,0,1,1,1,1,1,0,1,1,0,0,0,1,0,0,0,0,0,1,1,0,1,1,0,0,0,1,0,1,1,1,0,0,1,0,0,0,1,1,0,1,1,0,0,1,1,0,0,1,0,0,0,1,1,0,1,0,1,1,0,1,0,1,0,1},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,1,1,0,1,1,1,0,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,1,0,1,0,1,1,0,0,1,1,1,0,0,1,0,0,1,1,1,1,0,0,0,1,0,0,1,0,0,1,0,1,1,1,1,1,0,1,0,0,1,0,0,1,0,0,1,0,1,1,0,1,1,0,0,0,0,1,1,1,0,1,1,0,0,1,0,1,1,1,0,1,1,1,0,0,0,0,0,1,0,0,0,1,1,0,1,1,1,0,1,0,0,0,1,0,1,0,0,1,1,1,1,0,0,0,0,1,0,1,0,1,1,0,1,1,0,0,1,0,1,1,1,1,0,0,0,1,0,0,1,1,0,1,1,1,0,1,0,0,0,1,1,0,0,1,1,0,1,0,1,0,1,0,0,0,0,1,1,1,0,1,1,0,1,1,1,1,0,0,1,1,0,0,0,0,0,1,0,0,0,0,1,1,1,1,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,1,1,1},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,0,1,0,1,1,1,1,0,1,0,1,1,0,1,0,1,0,0,0,0,1,0,1,1,1,0,1,0,0,1,1,0,0,1,0,1,0,1,0,1,1,1,0,0,0,0,1,1,0,1,1,0,1,0,0,1,1,1,0,1,0,0,0,0,0,0,1,1,0,1,1,1,0,1,1,1,0,1,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,1,0,1,0,0,1,1,1,1,0,0,1,1,0,1,1,1,0,0,0,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,0,0,0,1,0,0,1,0,0,1,1,0,1,0,0,1,0,1,0,0,0,1,1,1,0,1,1,1,1,0,1,0,0,0,0,1,1,0,0,1,0,0,0,1,1,1,1,1,0,0,1,0,0,1,0,1,0,0,0,0,1,1,0,0,0,1,1,1,1,0,1,1,1,0,1,0,1,0,1,0,0,0,1,0,1,1,1,0,1,0,1,1,1,0,0,1,0,1,1,1,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,1,0,0,0,1,0,0,0,1,1,1,1,1,0,0,1,0,1,0,1,0,0,0,0,1,0,0,0,1,1,1,0,1,1,0,1,1,0,1,0,0,1,1,0,1,0,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,0,1,1,0,1,0,0,1,1,1,0,1,1,1,0,0,1,0,0,1,1,0,0,0,1,0,1,0,0,1,0,1,0,0,1,1,1,0,0,1,1,0,1,0,1,0,1,1,1,1,0,0,1,0,0,0,1,0,1,1,1,0,1,0,0,0,0,1,0,0,1,1,1,0,1,1,0,1,0,0,1,1,1,0,0,0,1,0,1,1,1,0,1,0,0,0,0,1,1,1,1,0,1,0,0,1,1,0,0,1,1,0,1,0,0,0,1,0,1,1,0,1,1,1,0,1,1,1,0,1,0,1,0,0,0,0,0,1,1,1,1,0,0,1,1,0,1,0,0,0,0,1,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,0,1,0,0,0,1,0,1,1,0,1,1,0,1,0,0,0,0,1,1,0,1,1,0,1,0,1,0,1,0,0,1,0,1,1,1,0,0,1,1,0,0,1,0,0,0,0,0,1,1,0,1,1,1,0,1,1,0,1,1,1,1,0,1,1,0,0,1,1,0,0,0,0,0,0,1,0,1,0,0,1,1,1,0,1,0,1,1,0,0,0,0,1,0,0,1,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,0,0,1,0,1,0,0,1,1,0,0,1,1,1,1,1,0,0,0,0,0,1,0,0,1,1,0,1,0,1,0,0,1,0,0,1,1,1,0,0,0,0,1,1,1,1,1,0,1,0,1,1,0,0,0,0,0,1,1,1,0,0,1,1,1,1,0,0,0,1,0,0,1,0,0,1,1,0,0,1,1,1,1,0,1,0,0,1,1,1,0,1,0,0,1,0,0,1,1,1,0,0,0,1,0,0,1,1,1,0,0,0,0,1,0,1,1,1},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,1,0,1,1,1,1,0,0,1,1,1,1,0,0,1,0,1,0,1,0,0,0,1,1,0,0,1,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,0,1,0,1,0,0,1,0,1,0,1,0,0,0,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,0,1,0,0,1,1,0,1,0,0,1,1,0,1,0,1,0,0,0,1,0,1,1,0,0,1,1,0,1,1,0,0,0,1,0,1,0,1,0,0,1,0,0,1,1,1,1,0,1,1,0,1,1,1,0,0,1,1,1,0,0,0,0,1,1,0,0,1,1,1,1,0,0,0,0,1,0,0,1,0,1,1,0,1,0,0,1,1,0,0,1,0,0,1,1,1,1,0,0,1,0,0,0,1,1,1,0,1,0,1,0,1,0,0,0,1,1,0,0,1,1,0,1,0,1,0,1,0,0,1,0,1,1,0,1,1,0,1,0,1,1,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,0,0,0,0,0,0,0,1,1,1,0,1,0,0,1,1,0,1,1,1,1,0,0,0,0,1,1,0,1,0,0,0,0,1,1,1,1,0,0,1,1,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,1,1,1,0,0,0,1,1,1,0,1,0,0,0,0,1,1,1,1,0,1,1,0,0,1,0,0,1,0,1,1,0,1,0,0,0,1,1,0,0,1,0,1,0,1,0,0,1,1,0,1,1,0,1,1,0,0,0,1,0,1,1,1,0,1,1,0,1,0,1,1,0,0,0,0,1,0,0,1,1,1,1,0,1,1,0,0,1,0,0,1,0,0,1,0,0,1,0,1,1,1,0,0,1,0,0,0,1,1,0,1,0,0,0,1,1,0,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,1,0,0,0,1,0,0,0,0,1,0,1,1,0,1,0,1,0,1,1,1,1,1,1,0,0,1,1,1,0,1,0,1,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,1,0,1,0,0,0,0,1,1,0,1,0,0,0,1,1,1,1,0,0,1,0,1,0,1,0,1,1,0,1,1,0,0,1,1,0,1,0,1,1,0,0,0,0,0,1,1,1,1,0,0,1,1,1,0,1,0,0,0,1,0,0,1,0,0,0,1,1,1,1,0,0,1,0,1,0,0,1,0,0,0,1,0,1,0,1,1,1,1,0,1,1,0,0,0,1,1,1,1,1,0,1,1,0,0,0,0,0,0,1,1,1,0,0,1,1,0,0,1,0,0,1,1,1,0,0,1,0,1,0,0,1,0,0,1,1,1,1,0,0,0,1,0,0,1,1,1,1,1,0,0,0,1,1,0,1,0,0,0,1,0,1,0,1,1,0,0,1,0,1,1,0,0,0,0,0,1,0,1,0,1,1,1,1,0,1,1,1,0,1,0,1,0,0,0,0,1,1,1,0,1,1,0,0,0,0,1,1,0,1,1,0,0,1,0,1,1,1,0,1,0,1,1,0,1,1,0,0,0,1,0,0,0,1,1},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,1,1,0,0,1,1,0,0,1,0,1,1,1,1,0,0,1,1,0,0,0,1,0,1,0,1,1,1,0,1,0,1,0,0,0,0,1,0,0,1,1,1,0,1,0,1,0,0,1,1,1,1,0,0,0,0,1,0,0,1,1,1,0,1,0,0,1,1,1,1,0,0,0,1,1,0,1,0,0,0,0,0,1,1,0,1,0,1,1,0,1,0,0,0,0,1,1,1,1,1,1,0,0,1,0,0,0,1,0,0,0,0,0,1,0,1,1,1,1,1,1,1,0,1,1,0,0,1,1,0,0,1,0,0,1,1,0,0,1,0,1,1,0,0,1,0,0,1,1,0,1,0,1,1,1,0,0,1,0,1,0,1,0,1,0,1,1,0,0,1,0,0,0,1,0,0,1,0,1,0,1,1,1,1,0,0,0,0,1,1,0,1,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,1,0,1,0,1,0,0,0,1,1,0,0,0,1,1,0,1,0,0,1,1,0,1},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,1,0,1,0,0,0,0,1,1,1,0,0,1,1,0,1,0,0,0,1,0,1,1,1,0,1,1,0,0,1,0,0,1,0,1,1,0,1,1,0,1,0,1,1,0,1,1,0,0,0,1,0,0,1,0,1,0,1,0,1,1,1,1,1,1,0,0,0,0,1,0,0,1,0,1,1,1,0,1,1,0,0,1,1,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,1,1,1,1,0,1,0,0,0,1,0,0,1,1,1,1,0,1,1,0,1,1,1,0,0,0,1,0,0,1,1,1,0,0,0,1,0,1,1,1,0,0,0,0,1,0,1,0,1,0,1,1,1,1,0,1,0,1,1,0,0,0,1,0,1,1,0,0,1,0,0,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,1,1,1,0,1,0,1,1,1,0,0,1,0,0,1,0,1,0,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,0,1,0,1,1,1,0,0,1}
};


int main()
{
	vector <vector <int>> p;
	ofstream op("res.txt");
	ifstream fr("out.txt");
	while (!fr.eof())
	{
		vector <int> k(8);
		fr >> k[0] >> k[1] >> k[2] >> k[3] >> k[4] >> k[5] >> k[6] >> k[7];
		p.push_back(k);
	}
	p.erase(p.end() - 1, p.end());
#pragma omp parallel
	{
		int k;
#pragma omp for private(k) nowait
		for (k = 0; k < Ben.size(); ++k)
		{
			int max = 0, min = 10000;
			double avr = 0;
			for (int i = 0; i < p.size(); ++i)
			{
				BDD b(Ben[k], p[i]);
				if (b.getSize() > max)
				{
					max = b.getSize();
				}
				if (b.getSize() < min)
				{
					min = b.getSize();
				}
				avr += b.getSize();
			}
			avr /= p.size();
			op << max << " " << min << " " << avr << " " << endl;
			cout << k << endl;
		}
	}
}