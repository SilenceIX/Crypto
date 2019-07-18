#include "irredPol.h"
#include "PRNG.h"
#include <fstream>
#include <ctime>
using namespace std;

int comp(int *a, int *b, int c)
{
	for (int i = 0; i < c; ++i)
	{
		if (a[i] != b[i])
		{
			return false;
		}
	}
	return true;
}


int main()
{
	ofstream f("out.bin", ios_base::binary);
	PRNGVector r(0);
	for (int i = 0; i < pow(2, 20); ++i)
	{
		//int *a = r.getNewVectorValue();
		//for (int k = 0; k < r.getDegree(); ++k)
		//{
		//	f1 << a[k] << " ";
		//}
		//f1 << endl;
		char c = r.getNewBinOrder();
		f.write(&c, sizeof(c));
	}
	f.close();
	system("pause");
}