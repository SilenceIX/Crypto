#include "irredPol.h"
#include "PRNG.h"
#include <random>
#include <fstream>

using namespace std;


int main()
{
	//mt19937 m(2143);
	//ofstream f("out.bin", ios_base::binary);
	PRNGVector r(3213);
	r.GetBinRandom(pow(2, 10) * 1000);
	//for (int i = 0; i < pow(2, 10) * 1000; ++i)
		//f << (char)(m() % 256 - 128);
	/*for (int i = 0; i < p.size();)
	{
		char c = 0;
		for (int k = 0; k < 8 && i < p.size(); ++k, ++i)
		{
			c = c << 1;
			c += p[i];
			f2 << p[i];
		}
		f.write(&c, sizeof(char));
	}*/
	system("pause");
}