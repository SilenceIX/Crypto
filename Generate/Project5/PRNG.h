#pragma once
#include <vector>
#include <fstream>
#include "DataBase.h"
using namespace std;

class PRNGVector
{
private:
	const int modPlace[2] = { 2 };
	int matrix[100000][7];
	int size;
	vector <int> pol;
	vector <int> mods;
	int g[9][9], l[9][9], u[9][9];
	int pr, mod;
	int seed;
	int cBits;
	int curIndex;
	static vector <int> sum(vector <int> a, vector <int> b);
	vector <int> scal(int k, int *a);
	void newPol(int *a = NULL);
public:
	PRNGVector(int seed);
	int *createNextVector();
	void GetBinRandom(int power);
};