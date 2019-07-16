#include "PRNG.h"

vector<int> PRNGVector::sum(vector<int> a, vector<int> b)
{
	for (int i = 0; i < a.size(); ++i)
		a[i] = a[i] + b[i];
	return a;
}

vector<int> PRNGVector::scal(int k, int *a)
{
	vector <int> r(pr);
	for (int i = 0; i < pr; ++i)
	{
		r[i] = k * a[i];
	}
	return r;
}

void PRNGVector::newPol(int *a)
{
	if (a != NULL)
	{
		seed = (1337 * a[size - 4] * a[size - 3] + 1391 * seed + 17853) % 179321;
		pr = (seed + a[seed % size]) % 3 + 2;
		mod = modPlace[a[(13 * seed + 17) % size ] % 1];
		pol = DATA[{pr, mod}][seed % DATA[{pr, mod}].size()];
	}
	else
	{
		pr = seed % 4 + 2;
		mod = modPlace[seed % 1];
		pol = DATA[{pr, mod}][seed % DATA[{pr, mod}].size()];
	}
	switch (mod)
	{
	case 2: cBits = 1;
		break;
	case 3: cBits = 2;
		break;
	case 5: cBits = 3;
		break;
	case 7: cBits = 3;
		break;
	}

	for (int i = 0; i < pol.size(); ++i)
	{
		pol[i] = mod - pol[i];
	}
	
	curIndex = pr - 1;
	for (int i = 0; i < pr - 1; ++i)
	{
		for (int j = i + 1; j < pr; ++j)
		{
			l[i][j] = mods[(i * seed + j * 3) % (mod - 1)];
			u[i][j] = mods[(j * seed + i * 3) % (mod - 1)];
		}
	}
	int d1 = 1, d2 = 1;


	for (int i = 0; i < pr; ++i)
	{
		l[i][i] = 1;
		d1 *= l[i][i];
		u[i][i] = mods[(seed * i + 17) % (mod - 1)];
		d2 *= u[i][i];
	}
	for (int i = 0; i < pr; ++i)
	{
		for (int j = 0; j < pr; ++j)
		{
			matrix[i][j] = 0;
			for (int k = 0; k < pr; ++k)
			{
				matrix[i][j] += l[i][k] * u[j][k];
			}
			matrix[i][j] = matrix[i][j] % mod;
		}
	}
	size = pr;
}


PRNGVector::PRNGVector(int s)
{
	seed = s;
	for (int i = 0; i < 11; ++i)
	{
		mods.push_back(i + 1);
	}
	newPol();
}

int *PRNGVector::createNextVector()
{
	vector <int> buffRes(pr);
	for (int i = 0; i < pol.size() - 1; ++i)
	{
		int curM = (i + curIndex - pr + 1);
		if (curM < 0)
		{
			curM = size + curM;
		}
		else
		{
			curM = curM % size;
		}
		buffRes = sum(buffRes, scal(pol[pol.size() - 1 - i], matrix[curM]));
	}
	for (int i = 0; i < pr; ++i)
	{
		buffRes[i] = buffRes[i] % mod;
	}
	if (size < pow(mod, pr) - 2)
	{
		for (int k = 0; k < pr; ++k)
		{
			matrix[size][k] = buffRes[k];
		}
		++size;
	}
	else
	{
		newPol(matrix[size - 1]);
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
		for (int p = 0; p < pow(mod, pr) - 1; ++p)
		{
			int *k = Next();
			for (int j = 0; j < pr; ++j)
			{
				for (int d = 0; d < cBits; ++d)
				{
					char buffc = c - 128;
					c = c << 1;
					c += (k[j] >> (cBits - 1 - d)) & 1;						
					if (c > 255)
					{
						f.write(&buffc, sizeof(char));
						++i;
						c -= 255;
					}		
					if (i > power)
					{
						return;
					}
				}
			}
		}
		newPol(matrix[size - 1]);		
	}
}
