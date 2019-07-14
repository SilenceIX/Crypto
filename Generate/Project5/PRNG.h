#pragma once
#include <vector>
#include <fstream>
#include "DataBase.h"
using namespace std;

class PRNGVector
{
private:
	const int cBits[4] = {1, 2, 3, 3};
	const int modPlace[2] = { 5, 7 };
	vector <vector <int>> matrix;
	vector <int> pol;
	int pr, mod;
	int seed;
	int count;
	int curIndex;
	static vector <int> sum(vector <int> a, vector <int> b);
	static vector <int> scal(int k, vector <int> a);
	void newPol(vector <int> a = {});
public:
	PRNGVector(int seed);
	vector <int> Next();
	void GetBinRandom(int power);
};