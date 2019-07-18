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
	BDD a1(f1, { 0, 1, 2, 3 });
	BDD a2(f1, { 0, 1, 2, 3 });
	BDD a3(f1, { 0, 1, 2, 3 });
	BDD a4(f1, { 0, 1, 2, 3 });
	BDD a5(f1, { 0, 1, 2, 3 });
	a2.insertBDD(a1, 0);	
	a2.printListG2();
	/*a3.insertBDD(a2, 0);
	a3.insertBDD(a1, 1);

	a4.insertBDD(a3, 0);
	a4.insertBDD(a2, 1);
	a4.insertBDD(a1, 2);
	
	a5.insertBDD(a4, 0);
	a5.insertBDD(a3, 1);
	a5.insertBDD(a2, 2);
	a5.insertBDD(a1, 3);*/

	/*for (auto el : a2.AllSat())
	{
		for (int k : el)
			cout << k;
		cout << endl;
	}*/


	/*for (int i = 0; i < 16; ++i)
	{
		vector <bool> x1(4);
		for (int j = 0; j < 4; ++j)
		{
			x1[3 - j] = (i >> j) & 1;
		}
		vector <bool> x2 = { f1(x1), x1[1], x1[2], x1[3] };
		if (f1(x2))
		{
			for (int k = 0; k < 4; ++k)
			{
				cout << x1[k];
			}
			cout << endl;
		}
	}*/
	//vector <bool> x3 = { f1(x2), f1(x1), x1[2], x1[3] };
	//vector <bool> x4 = { f1(x3), f1(x2), f1(x1), x1[3] };
	//vector <bool> x5 = { f1(x4), f1(x3), f1(x2), f1(x1)};
	//cout << f1(x3) << endl;

	



	system("pause");
}