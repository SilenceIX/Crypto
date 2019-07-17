#include "PRNG.h"



void PRNGVector::calculateDigreeModAndCountBits()
{
	degree = (17 * seed + 13) % (endDigreePolynom - startDigreePolynom + 1) + startDigreePolynom;
	mod = modVariation[(19 * seed + 23) % (modCountVariation)];	
	countBits = 1;
	maxLengthCycle = (int)pow(mod, degree);
	while ((mod - 1) >> countBits != 0)
	{
		++countBits;
	}
}

void PRNGVector::createMatrix()
{
	for (int i = 0; i <= degree - 1; ++i)
	{
		for (int j = i + 1; j <= degree; ++j)
		{
			l[i][j] = (i * seed + j * 3) % mod;
			u[i][j] = (j * seed + i * 3) % mod;
		}
	}
	int def1 = 1, def2 = 1;
	for (int i = 0; i <= degree; ++i)
	{
		l[i][i] = 1;
		def1 *= l[i][i];
		u[i][i] = (seed * i + 13) % (mod - 1) + 1;
		def2 *= u[i][i];
	}
	multyMatrixLU();
	countVectors = degree;
	nowVector = degree - 1;
}

void PRNGVector::suchPolynom()
{
	suchPrimitivePolynom = DATA[{degree, mod}][(29 * seed + 31) % DATA[{degree, mod}].size()];
	for (int i = 1; i < suchPrimitivePolynom.size(); ++i)
	{
		suchPrimitivePolynom[i] = mod - suchPrimitivePolynom[i];
	}
}

void PRNGVector::multyMatrixLU()
{
	for (int i = 0; i <= degree; ++i)
	{
		for (int j = 0; j <= degree; ++j)
		{
			matrix[i][j] = 0;
			for (int k = 0; k <= degree; ++k)
			{
				matrix[i][j] += l[i][k] * u[j][k];
			}
			matrix[i][j] = matrix[i][j] % mod;
		}
	}
}

void PRNGVector::createNewVector()
{
	for (int i = 0; i <= degree; ++i)
	{
		newVector[i] = 0;
	}
	for (int i = countVectors - degree; i < countVectors; ++i)
	{
		int correctIndex = i - (countVectors - degree);
		sumVertex(newVector,
			scalVertex(suchPrimitivePolynom[suchPrimitivePolynom.size() - 1 - correctIndex], matrix[i], degree),
			degree);
	}
}

void PRNGVector::addBit(bool bit)
{
	accumulator = accumulator << 1;
	accumulator += bit;
}

void PRNGVector::restart()
{
	for (int i = 0; i <= degree; ++i)
	{
		seed = (2521 * seed + matrix[nowVector][i] * seed + 2819) % 333667;
	}
	calculateDigreeModAndCountBits();
	suchPolynom();
	createMatrix();
}

void PRNGVector::sumVertex(int *result, const int *b, const int degree)
{
	for (int i = 0; i <= degree; ++i)
	{
		result[i] += b[i];
	}
}

int *PRNGVector::scalVertex(const int a, const int *c, const int degree)
{
	int *result = new int[degree + 1];
	for (int i = 0; i <= degree; ++i)
	{
		result[i] = a * c[i];
	}
	return result;
}

PRNGVector::PRNGVector(int s)
{
	seed = s;
	accumulator = 1;
	calculateDigreeModAndCountBits();
	suchPolynom();	
	createMatrix();
}

int *PRNGVector::getNewVectorValue()
{
	if (countVectors < rightInterval * maxLengthCycle)
	{
		createNewVector();
		++nowVector;
		++countVectors;
		for (int i = 0; i <= degree; ++i)
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
	int cZero = 0;
	while (accumulator < 512)
	{
		for (int i = countVectors; i < leftInterval * maxLengthCycle; ++i)
		{
			getNewVectorValue();
		}

		int *r = getNewVectorValue();
		for (int i = 0; i <= degree; ++i)
		{
			for (int k = countBits - 1; k >= 0; k -= 1)
			{
				bool bit = (r[i] >> k) & 1;
				addBit(bit);
				/*if (!bit)
				{
					++cZero;
				}
				else
				{
					cZero = 0;
				}
				if (cZero < 3)
				{
					addBit(bit);
				}
				else if (cZero > 3)
				{
					cZero = 0;
				}	*/
			}
		}
	}
	char c = accumulator % 512 - 256 - 128;
	accumulator = accumulator >> 8;
	return c;
}

int PRNGVector::getDegree()
{
	return degree;
}



