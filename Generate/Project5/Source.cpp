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
	ofstream f("irred.txt");
	for (int i = 6; i <= 8; ++i)
	{
		f << "degree "<< i << "; mod 11\n";
		irred::polynoms p = irred::irreduciblePolynoms(i, 11);
		for (int i = 0; i < p.size(); ++i)
		{
			for (int a : p[i])
			{
				f << a << " ";
			}
			f << endl;
		}
	}
	system("pause");
}