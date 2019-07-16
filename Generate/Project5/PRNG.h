#pragma once
#include <vector>
#include <fstream>
#include "DataBase.h"
using namespace std;

class PRNGVector
{
private:
	static const int maxValueMatrix = 50000;
	static const int startDigreePolynom = 2;
	static const int endDigreePolynom = 5;
	static const int modCountVariation = 4;
	const int modVariation[modCountVariation] = { 2, 3, 5, 7 };
	int matrix[maxValueMatrix][endDigreePolynom];
	int l[endDigreePolynom][endDigreePolynom];
	int u[endDigreePolynom][endDigreePolynom];
	int newVector[endDigreePolynom];

	vector <int> suchPrimitivePolynom;
	int countBits;
	int digree, mod;
	int seed;
	int countVectors;
	int nowVector;
	int maxLengthCycle;
	int accumulat= 0;

	void calculateDigreeModAndCountBits();
	void createMatrix();
	void suchPolynom();
	void multyMatrixLU();
	void createNewVector();
	void restart();

	static void sumVertex(int *result, const int *b, const int digree);
	static int *scalVertex(const int a, const int *c, const int digree);

public:
	PRNGVector(int seed);
	int *getNewVectorValue();
	char getNewBinOrder();
};