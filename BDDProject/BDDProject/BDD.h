#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <functional>
#include <map>
#include <queue>

using namespace std;
struct Node
{
	int var;
	int childFalse, childTrue;
	Node(int v, int f, int t)
	{
		var = v;
		childFalse = f;
		childTrue = t;
	}
	Node()
	{
		var = -1;
		childFalse = 0;
		childTrue = 0;
	}
};

class BDD
{
private:
	vector <Node> T;
	map <int, Node> ROBDD;
	function <bool(vector <bool>)> F;
	int Count;
	int sizeTree;
	int order;
	int lastVertex;
	vector <bool> VecValue;
	vector <int> orderVar;
	void makeROBDD();
	void restruct();
	static int app(vector <Node> *n, BDD a, BDD b, Node u, Node v, char op);
	static int curIndex(int, map <int, int> *);
public:
	BDD(const function <bool(vector <bool>)> f, vector <int> orderArg);
	BDD(const vector <bool> f, vector <int> orderArg);
	BDD(vector <int> orderArg);
	static BDD apply(BDD a, BDD b, char op);
	vector <vector <int>> AllSat(vector <pair <int, int>> v = { });
	int getSize();
	void printBDD();
	void printListG1();
	void printListG2();
	bool test(vector <bool>);
};


