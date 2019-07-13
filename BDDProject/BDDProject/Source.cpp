#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <omp.h>
#include "BDD.h"

using namespace std;

bool f(vector <bool> x)
{
	return (x[0] | x[1] | x[2] | x[3]) & (x[0] | !x[1] | x[2] | !x[3]) & (!x[0] | x[1] | !x[2]);
}

bool a1(vector <bool> x)
{
	return (x[0] | !x[1]) & (!x[0] | x[1]);
}


int main()
{
	BDD y(f, { 0, 1, 2, 3 });
	y.printListG2();
	cout << endl;
	BDD p1(a1, { 0, 1 });
	p1.printListG2();
	cout << endl;
	y.insertBDD(p1, 2);
	cout << endl;
	y.printListG2();

	system("pause");
}