#include "PRNG.h"

vector<int> PRNGVector::sum(vector<int> a, vector<int> b)
{
	for (int i = 0; i < a.size(); ++i)
		a[i] = a[i] + b[i];
	return a;
}

vector<int> PRNGVector::scal(int k, vector<int> a)
{
	for (int i = 0; i < a.size(); ++i)
		a[i] = k * a[i];
	return a;
}

void PRNGVector::newPol(vector <int> a)
{

	if (!a.empty())
	{
		pr = a[a.size() - 2] % 2 + 5;
		mod = modPlace[a[a.size() - 1] % 2];
		pol = DATA[{pr, mod}][a[a.size() - 4] % DATA[{pr, mod}].size()];
	}
	else
	{
		pr = seed % 2 + 5;
		mod = modPlace[seed % 2];
		pol = DATA[{pr, mod}][seed % DATA[{pr, mod}].size()];
	}
	switch (mod)
	{
	case 2: count = 0;
		break;
	case 3: count = 1;
		break;
	case 5: count = 2;
		break;
	case 7: count = 3;
		break;
	}

	for (int i = 0; i < pol.size(); ++i)
		pol[i] = (mod - pol[i]) % mod;

	vector <vector <int>> g(pr, vector <int>(pr, 0));
	curIndex = pr - 1;
	matrix = g;

	vector <vector <int>> l(pr, vector <int>(pr, 0));
	vector <vector <int>> u(pr, vector <int>(pr, 0));

	vector <int> mods(mod - 1);
	for (int i = 0; i < mod - 1; ++i)
	{
		mods[i] = i + 1;
	}
	for (int i = 0; i < pr - 1; ++i)
	{
		for (int j = i + 1; j < pr; ++j)
		{
			l[i][j] = mods[(i * i + j * 3) % (mod - 1)];
			u[j][i] = mods[(j * j + i * 3) % (mod - 1)];
		}
	}
	int d1 = 1, d2 = 1;


	for (int i = 0; i < pr; ++i)
	{
		l[i][i] = 1;
		d1 *= l[i][i];
		u[i][i] = mods[(17 * i * i + seed * i) % (mod - 1)];
		d2 *= u[i][i];
	}
	if (d1 % mod != 0 && d2 % mod != 0 && d1 * d2 % mod != 0)
	{
		for (int i = 0; i < pr; ++i)
		{
			for (int j = 0; j < pr; ++j)
			{
				for (int k = 0; k < pr; ++k)
				{
					matrix[i][j] += l[i][k] * u[k][j];
				}
				matrix[i][j] = matrix[i][j] % mod;
			}
		}
	}
}


PRNGVector::PRNGVector(int s)
{
	seed = s;
	newPol();
}

vector<int> PRNGVector::Next()
{
	vector <int> buffRes(pr);
	for (int i = 0; i < pol.size() - 1; ++i)
	{
		int curM = (i + curIndex - pr + 1);
		if (curM < 0)
		{
			curM = matrix.size() + curM;
		}
		else
		{
			curM = curM % matrix.size();
		}
		buffRes = sum(buffRes, scal(pol[pol.size() - 1 - i], matrix[curM]));
	}
	for (int i = 0; i < pr; ++i)
	{
		buffRes[i] = buffRes[i] % mod;
	}
	if (matrix.size() < pow(mod, pr) - 2)
	{
		matrix.push_back(buffRes);
	}
	else
	{
		newPol(matrix[matrix.size() - 1]);
		--curIndex;
	}
	++curIndex;
	return matrix[curIndex];
}

void PRNGVector::GetBinRandom(int power)
{
	ofstream f("out.bin", ios_base::binary);
	ofstream g("out.txt");
	int c = 0;
	for (int i = 0; i < power; )
	{
		for (int p = 0; p < pow(mod, pr) / 2; ++p)
		{
			vector <int> k = Next();
			for (int j = 0; j <  k.size(); ++j)
			{
				for (int d = 0; d < cBits[count]; ++d, ++i)
				{
					char buffc = c - 128;
					c = c << 1;
					{
						c += k[j];
						g << int(k[j] & 1);
						k[j] = k[j] >> 1;
						
					}
					if (c > 255)
					{
						f.write(&buffc, sizeof(char));
						c -= 255;
					}

		
					if (4 * i > power)
					{
						return;
					}
				}
			}
		}
		newPol(matrix[matrix.size() - 1]);
		
	}
	char buff = c;
	f.write(&buff, sizeof(char));
}
