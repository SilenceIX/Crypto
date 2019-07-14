#include "irredPol.h"
#include "PRNG.h"
#include <fstream>

using namespace std;


int main()
{
	ofstream f("out.txt");
	PRNGVector r(543);
	r.GetBinRandom(2e6);

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