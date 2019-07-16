#include "irredPol.h"
#include "PRNG.h"
#include <fstream>

using namespace std;


int main()
{
	ofstream f("out.bin", ios_base::binary);
	PRNGVector r(5431);
	for (int i = 0; i < pow(2, 10) * 125; ++i)
	{
		char c = r.getNewBinOrder();
		f.write(&c, sizeof(c));
		/*int *a = r.getNewVectorValue();
		for (int i = 0; i < 7; ++i)
		{
			cout << a[i] << " ";
		}
		cout << endl;*/
	}

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
	f.close();
	system("pause");
}