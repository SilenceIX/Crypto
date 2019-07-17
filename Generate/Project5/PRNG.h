#pragma once
#include <vector>
#include <fstream>
#include "DataBase.h"
using namespace std;

class PRNGVector
{
private:
	static const int maxValueMatrix = 1'000'000;
	static const int startDigreePolynom = 4;
	static const int endDigreePolynom = 6;
	const double leftInterval = 0;
	const double rightInterval = 1.0;

	static const int countSingularity = 0;
	const int singularities[countSingularity + 1] = { };

	static const int modCountVariation = 2;
	const int modVariation[modCountVariation] = { 3, 5 };

	int matrix[maxValueMatrix][endDigreePolynom + 1];
	int l[endDigreePolynom + 1][endDigreePolynom + 1];
	int u[endDigreePolynom + 1][endDigreePolynom + 1];
	int newVector[endDigreePolynom + 1];

	vector <int> suchPrimitivePolynom;
	int countBits;
	int degree, mod;
	int seed;
	int countVectors;
	int nowVector;
	int maxLengthCycle;
	int accumulator;

	void calculateDigreeModAndCountBits();
	void createMatrix();
	void suchPolynom();
	void multyMatrixLU();
	void createNewVector();
	void addBit(bool bit);
	void restart();

	static void sumVertex(int *result, const int *b, const int degree);
	static int *scalVertex(const int a, const int *c, const int degree);

public:
	PRNGVector(int seed);
	int *getNewVectorValue();
	char getNewBinOrder();
	int getDegree();
};