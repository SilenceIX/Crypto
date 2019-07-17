#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <omp.h>
#include "BDD.h"

using namespace std;


bool f1(vector <bool> x)
{
	return x[0] ^ x[2] & !x[3] ^ x[1]&x[3];
}





int main()
{	
	BDD a(f1, { 0, 1, 2, 3 });
	a.insertBDD(a, 0);
	a.printListG2();



	system("pause");
}