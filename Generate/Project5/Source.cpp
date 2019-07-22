#include "irredPol.h"
#include "PRNG.h"
#include <fstream>
#include <ctime>
#include <random>
#include <windows.h>
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
	mt19937 r1(13);
	ofstream f1("out1.bin", ios_base::binary);
	ofstream ft("time.csv");
	ULONGLONG t1 = GetTickCount64();
	//PRNGVector r1(18072019);
	for (int i = 0; i < pow(2, 20) * 50; ++i)
	{
		char c = r1();
		f1.write(&c, sizeof(c));
		if (i % int(pow(2, 20)) == 0)
		{
			ft << "i;" << GetTickCount64() - t1 << endl;
		}
	}
	f1.close(); 
	system("pause");
}