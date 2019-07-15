#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <omp.h>
#include "BDD.h"

using namespace std;

int main()
{	
	vector <int> p;
	ifstream f("res.txt");
	int k = 0;
	while (f >> k)
	{
		p.push_back(k);
	}
	int min = 10000, max = 0; 
	double avr = 0;
	for (int i = 0; i < p.size(); ++i)
	{
		avr += p[i];
		if (p[i] > max)
		{
			max = p[i];
		}
		if (p[i] < min)
		{
			min = p[i];
		}
	}
	double dx = 0;
	avr /= p.size();
	for (int i = 0; i < p.size(); ++i)
	{
		dx += p[i] * p[i];
	}
	dx /= p.size();
	dx -= avr * avr;
	dx = sqrt(dx);
	cout << max << " " << min << " " << avr << " " << dx << endl;



	system("pause");
}