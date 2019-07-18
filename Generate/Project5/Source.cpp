#include "irredPol.h"
#include "PRNG.h"
#include <fstream>

using namespace std;


int main()
{
	//ofstream f("out.bin", ios_base::binary);
	//ofstream f1("out.txt");
	//PRNGVector r(214);
	//for (int i = 0; i < pow(2, 20); ++i)
	//{
	//	//int *a = r.getNewVectorValue();
	//	//for (int k = 0; k < r.getDegree(); ++k)
	//	//{
	//	//	f1 << a[k] << " ";
	//	//}
	//	//f1 << endl;
	//	char c = r.getNewBinOrder();
	//	f.write(&c, sizeof(c));
	//	/*int *a = r.getNewVectorValue();
	//	for (int i = 0; i < 7; ++i)
	//	{
	//		cout << a[i] << " ";
	//	}
	//	cout << endl;*/
	//}
	//f.close();
	for (int k : {7, 8})
	{
		for (int i = 5; i <= 9; ++i)
		{
			irred::polynoms p = irred::irreduciblePolynoms(i, k);
		}
	}
	system("pause");
}