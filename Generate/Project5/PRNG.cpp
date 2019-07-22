#include "PRNG.h"



void PRNGVector::calculateDigreeModAndCountBits()
{
	degree = suchPrimitivePolynom.size() - 1;	
	countBits = 1;
	maxLengthCycle = (int)pow(mod, degree);
	while ((mod - 1) >> countBits > 0)
	{
		++countBits;
	}
}

void PRNGVector::createMatrix()
{
	for (int i = 0; i < degree - 1; ++i)
	{
		for (int j = i + 1; j < degree; ++j)
		{
			l[i][j] = abs(i * seed + j * 3) % mod;
			u[i][j] = abs(j * seed + i * 3) % mod;
		}
	}
	for (int i = 0; i < degree; ++i)
	{
		l[i][i] = 1;
		u[i][i] = 1;
	}
	multyMatrixLU();
	countVectors = degree;
	nowVector = degree - 1;
}

void PRNGVector::suchPolynom()
{
	suchPrimitivePolynom = DATA[seed % DATA.size()];
	for (int i = 1; i < suchPrimitivePolynom.size(); ++i)
	{
		suchPrimitivePolynom[i] = mod - suchPrimitivePolynom[i];
	}
}

void PRNGVector::multyMatrixLU()
{
	for (int i = 0; i < degree; ++i)
	{
		for (int j = 0; j < degree; ++j)
		{
			matrix[i][j] = 0;
			for (int k = 0; k < degree; ++k)
			{
				matrix[i][j] += l[i][k] * u[j][k];
			}
			matrix[i][j] = abs(matrix[i][j]) % mod;
		}
	}
}

void PRNGVector::createNewVector()
{
	for (int i = 0; i < degree; ++i)
	{
		newVector[i] = 0;
	}
	for (int i = countVectors - degree; i < countVectors; ++i)
	{
		int correctIndex = i - (countVectors - degree);
		int *r = scalVertex(suchPrimitivePolynom[suchPrimitivePolynom.size() - 1 - correctIndex], matrix[i], degree - 1);
		sumVertex(newVector, r,	degree);
		delete r;
	}
}

void PRNGVector::addBit(bool bit)
{
	accumulator.push(bit);
}

void PRNGVector::restart()
{
	for (int i = 0; i < degree; ++i)
	{
		seed = (2521 * seed + i * matrix[nowVector][i] * seed + 2819) % 333667;		
	}
	mod = 2;
	suchPolynom();
	calculateDigreeModAndCountBits();
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
	mod = 2;
	suchPolynom();
	calculateDigreeModAndCountBits();		
	createMatrix();
}

bool PRNGVector::cmp(int *a, int *b, int c)
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

int *PRNGVector::getNewVectorValue()
{
	if (countVectors < rightInterval * maxLengthCycle + degree)
	{
		createNewVector();
		++nowVector;
		++countVectors;
		for (int i = 0; i < degree; ++i)
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
	while (accumulator.size() < 8)
	{
		for (int i = countVectors; i < leftInterval * maxLengthCycle; ++i)
		{
			getNewVectorValue();
		}

		int *r = getNewVectorValue();
		for (int i = 0; i < degree; ++i)
		{
			for (int k = countBits - 1; k >= 0; k -= 1)
			{
				bool bit;
				if (nowVector % 2 == 0)
				{
					bit = (r[i] >> k) & 1;
				}
				else
				{
					bit = (r[degree - 1 - i] >> k) & 1;
				}
				addBit(bit);
			}
		}
	}
	unsigned char buff = 0;
	for (int i = 0; i < 8; ++i)
	{
		buff += accumulator.front() << (7 - i);
		accumulator.pop();
	}
	return buff;
	
}

int PRNGVector::getDegree()
{
	return degree;
}



