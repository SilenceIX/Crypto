#include "PRNG.h"



void PRNGVector::calculateDigreeModAndCountBits()
{
	digree = (17 * seed + 13) % (endDigreePolynom - startDigreePolynom) + startDigreePolynom;
	mod = modVariation[(19 * seed + 23) % (modCountVariation)];	
	countBits = 1;
	maxLengthCycle = (int)pow(mod, digree);
	while ((mod - 1) >> countBits != 0)
	{
		++countBits;
	}
}

void PRNGVector::createMatrix()
{
	for (int i = 0; i < digree - 1; ++i)
	{
		for (int j = i + 1; j < digree; ++j)
		{
			l[i][j] = (i * seed + j * 3) % mod;
			u[i][j] = (j * seed + i * 3) % mod;
		}
	}
	int def1 = 1, def2 = 1;
	for (int i = 0; i < digree; ++i)
	{
		l[i][i] = 1;
		def1 *= l[i][i];
		u[i][i] = (seed * i + 13) % (mod - 1) + 1;
		def2 *= u[i][i];
	}
	multyMatrixLU();
	countVectors = digree;
	nowVector = digree - 1;
}

void PRNGVector::suchPolynom()
{
	suchPrimitivePolynom = DATA[{digree, mod}][(29 * seed + 31) % DATA[{digree, mod}].size()];
	for (int i = 1; i < suchPrimitivePolynom.size(); ++i)
	{
		suchPrimitivePolynom[i] = mod - suchPrimitivePolynom[i];
	}
}

void PRNGVector::multyMatrixLU()
{
	for (int i = 0; i < digree; ++i)
	{
		for (int j = 0; j < digree; ++j)
		{
			matrix[i][j] = 0;
			for (int k = 0; k < digree; ++k)
			{
				matrix[i][j] += l[i][k] * u[j][k];
			}
			matrix[i][j] = matrix[i][j] % mod;
		}
	}
}

void PRNGVector::createNewVector()
{
	for (int i = 0; i < digree; ++i)
	{
		newVector[i] = 0;
	}
	for (int i = countVectors - digree; i < countVectors; ++i)
	{
		int correctIndex = i - (countVectors - digree);
		sumVertex(newVector,
			scalVertex(suchPrimitivePolynom[suchPrimitivePolynom.size() - 1 - correctIndex], matrix[i], digree),
			digree);
	}
}

void PRNGVector::restart()
{
	for (int i = 0; i < digree; ++i)
	{
		seed = (2521 * seed + matrix[nowVector][i] * seed + 2819) % 333667;
	}
	calculateDigreeModAndCountBits();
	suchPolynom();
	createMatrix();
}

void PRNGVector::sumVertex(int *result, const int *b, const int digree)
{
	for (int i = 0; i < digree; ++i)
	{
		result[i] += b[i];
	}
}

int *PRNGVector::scalVertex(const int a, const int *c, const int digree)
{
	int *result = new int[digree];
	for (int i = 0; i < digree; ++i)
	{
		result[i] = a * c[i];
	}
	return result;
}

PRNGVector::PRNGVector(int s)
{
	seed = s;
	calculateDigreeModAndCountBits();
	suchPolynom();	
	createMatrix();
}

int *PRNGVector::getNewVectorValue()
{
	if (countVectors < 3 * maxLengthCycle / 4)
	{
		createNewVector();
		++nowVector;
		++countVectors;
		for (int i = 0; i < digree; ++i)
		{
			matrix[nowVector][i] = newVector[i] % mod;
		}
	}
	else
	{
		restart();
		getNewVectorValue();
	}
	return matrix[nowVector];
}

char PRNGVector::getNewBinOrder()
{
	while (accumulat < 256)
	{
		int *r = getNewVectorValue();
		for (int i = 0; i < digree; ++i)
		{
			accumulat += r[i];
		}
	}
	char c = accumulat % 256 - 128;
	accumulat -= 256;
	return c;
}



