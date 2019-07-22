#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <queue>
#include "DataBase.h"
using namespace std;

class PRNGVector
{
private:
	static const int maxValueMatrix = 1'500'000;
	static const int startDigreePolynom = 3;
	static const int endDigreePolynom = 20;
	const double leftInterval = 0.4;
	const double rightInterval = 0.6;

	static const int countSingularity = 0;
	const int singularities[countSingularity + 1] = { };

	static const int modCountVariation = 1;
	const int modVariation[modCountVariation] = { 2 };

	int matrix[maxValueMatrix][endDigreePolynom + 1];
	int l[endDigreePolynom + 1][endDigreePolynom + 1];
	int u[endDigreePolynom + 1][endDigreePolynom + 1];
	int newVector[endDigreePolynom + 1];

	vector <int> suchPrimitivePolynom;
	queue <bool> accumulator;
	int countBits;
	int degree, mod;
	int seed;
	int countVectors;
	int nowVector;
	int maxLengthCycle;

	void calculateDigreeModAndCountBits();
	void createMatrix();
	void suchPolynom();
	void multyMatrixLU();
	void createNewVector();
	void addBit(bool bit);
	void restart();
	
	static bool cmp(int *a, int *b, int c);
	static void sumVertex(int *result, const int *b, const int degree);
	static int *scalVertex(const int a, const int *c, const int degree);

public:
	PRNGVector(int seed);
	int *getNewVectorValue();
	char getNewBinOrder();
	int getDegree();
};