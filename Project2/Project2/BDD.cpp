#include "BDD.h"


void BDD::makeROBDD()
{
	map <int, int> m;
	for (int i = 2; i < sizeTree + 2; ++i)
	{
		for (int j = i + 1; j < T.size() && T[i].var == T[j].var; ++j)
		{
			int curi = curIndex(i, &m);
			int curj = curIndex(j, &m);
			if (curi > 1 && curj > 1 && curi != curj)
			{
				Node t1 = T[curi];
				t1.childFalse = curIndex(t1.childFalse, &m);
				t1.childTrue = curIndex(t1.childTrue, &m);

				Node t2 = T[curj];
				t2.childFalse = curIndex(t2.childFalse, &m);
				t2.childTrue = curIndex(t2.childTrue, &m);
				if (t1.childFalse == t2.childFalse && t1.childTrue == t2.childTrue)
				{
					m.insert({ curj, curi });
				}
				if (t1.childFalse == t1.childTrue)
				{
					m.insert({ curi, t1.childTrue });
				}
			}
		}
	}
	for (int i = 2; i < sizeTree + 2; ++i)
	{
		int curi = curIndex(i, &m);
		Node t = T[curi];
		t.childFalse = curIndex(t.childFalse, &m);
		t.childTrue = curIndex(t.childTrue, &m);
		ROBDD[curi] = t;
	}

}

void BDD::restruct()
{
	map <int, int> m;
	map <int, Node> rob;
	int index = 2;
	for (auto el : ROBDD)
	{
		if (el.first > 1)
		{
			m[el.first] = index;
			++index;
		}
	}
	lastVertex = index - 1;
	rob[0] = Node(Count, 0, 0);
	rob[1] = Node(Count, 1, 1);
	for (auto el : ROBDD)
	{
		if (el.first > 1)
		{
			int curi = el.first;
			curi = m.find(curi) != m.end() ? m[curi] : curi;
			Node curN = el.second;
			curN.childFalse = m.find(curN.childFalse) != m.end() ? m[curN.childFalse] : curN.childFalse;
			curN.childTrue = m.find(curN.childTrue) != m.end() ? m[curN.childTrue] : curN.childTrue;
			rob[curi] = curN;
		}
	}
	ROBDD.clear();
	ROBDD = rob;
}


int BDD::curIndex(int c, map<int, int> *m)
{
	while (m->find(c) != m->end() && (*m)[c] != c)
	{
		c = (*m)[c];
	}
	return c;
}

BDD::BDD(const vector<bool> f, vector <int> orderArg)
{
	Count = orderArg.size();
	orderVar = orderArg;
	sizeTree = 0;
	int k = 1;
	for (int i = 0; i < Count; ++i, k *= 2)
	{
		sizeTree += k;
	}
	order = k;
	T.resize(sizeTree + 2);
	T[0].var = Count;
	T[1].var = Count;
	VecValue.resize(order);
	for (int i = 0; i < Count; ++i)
	{
		orderVar[i] = orderArg[i];
	}
	for (int i = 0; i < order; ++i)
	{
		int curI = 0;
		for (int m = 0; m < Count; ++m)
		{
			curI += ((i >> m) & 1) << orderVar[Count - 1 - m];
		}
		VecValue[i] = f[curI];
		if (i % 2 == 0)
		{
			T[i / 2 + 2].childFalse = VecValue[i];
		}
		else
		{
			T[i / 2 + 2].childTrue = VecValue[i];
		}
	}
	for (int i = 0; i < sizeTree - order / 2; ++i)
	{
		T[(sizeTree - 1) - i + 2].childTrue = (sizeTree - 1) - (2 * i + 1) + 2;
		T[(sizeTree - 1) - i + 2].childFalse = (sizeTree - 1) - (2 * i + 2) + 2;
	}
	for (int i = 0; i < Count; ++i)
	{
		for (int k = pow(2, i); k < pow(2, i + 1); ++k)
		{
			T[sizeTree - k + 2].var = orderArg[i];
		}
	}
	makeROBDD();
	restruct();
}

BDD::BDD(const function<bool(vector <bool>)> f, vector <int> orderArg)
{
	F = f;
	Count = orderArg.size();
	orderVar = orderArg;
	vector <bool> value(Count);
	sizeTree = 0;
	int k = 1;
	for (int i = 0; i < Count; ++i, k *= 2)
	{
		sizeTree += k;
	}
	order = k;
	T.resize(sizeTree + 2);
	T[0].var = Count;
	T[1].var = Count;
	VecValue.resize(order);
	for (int i = 0; i < order; ++i)
	{
		for (int j = 0; j < Count; ++j)
		{
			value[orderArg[Count - 1 - j]] = (i >> j) & 1;
		}

		VecValue[i] = F(value);
		if (i % 2 == 0)
		{
			T[i / 2 + 2].childFalse = VecValue[i];
		}
		else
		{
			T[i / 2 + 2].childTrue = VecValue[i];
		}
	}
	for (int i = 0; i < sizeTree - order / 2; ++i)
	{
		T[(sizeTree - 1) - i + 2].childTrue = (sizeTree - 1) - (2 * i + 1) + 2;
		T[(sizeTree - 1) - i + 2].childFalse = (sizeTree - 1) - (2 * i + 2) + 2;
	}
	for (int i = 0; i < Count; ++i)
	{
		for (int k = pow(2, i); k < pow(2, i + 1); ++k)
		{
			T[sizeTree - k + 2].var = orderArg[i];
		}
	}
	makeROBDD();
	restruct();
}
BDD::BDD(vector<int> orderArg)
{
	Count = orderArg.size();
	orderVar = orderArg;
}
BDD BDD::apply(BDD a, BDD b, char op)
{
	if (a.orderVar == b.orderVar)
	{
		BDD r(a.orderVar);
		vector <Node> bd;
		bd.push_back(Node(a.Count, 0, 0));
		bd.push_back(Node(a.Count, 1, 1));
		switch (op)
		{
		case '&':
			r.F = [=](vector <bool> x) {return a.F(x) & b.F(x); };
			break;
		case '|':
			r.F = [=](vector <bool> x) {return a.F(x) | b.F(x); };
			break;
		case '^':
			r.F = [=](vector <bool> x) {return a.F(x) ^ b.F(x); };
			break;
		}
		app(&bd, a, b, a.ROBDD[a.lastVertex], b.ROBDD[b.lastVertex], op);
		map <int, int> m;

		queue <int> q;
		q.push(2);
		int c = 2;
		while (!q.empty())
		{
			int i = q.front();
			q.pop();
			if (bd[i].childFalse > 1)
			{
				q.push(bd[i].childFalse);
			}
			if (bd[i].childTrue > 1)
			{
				q.push(bd[i].childTrue);
			}
			m[i] = bd.size() - 1 - c + 2;
			++c;
		}
		map <int, Node> buffBDD;
		for (int i = 0; i < bd.size(); ++i)
		{
			Node curN = bd[i];
			int curi = m.find(i) != m.end() ? m[i] : i;
			curN.childFalse = m.find(curN.childFalse) != m.end() ? m[curN.childFalse] : curN.childFalse;
			curN.childTrue = m.find(curN.childTrue) != m.end() ? m[curN.childTrue] : curN.childTrue;
			buffBDD[curi] = curN;
		}

		m.clear();
		for (int i = 2; i < buffBDD.size(); ++i)
		{
			for (int j = 2; j < buffBDD.size(); ++j)
			{
				if (buffBDD[i].var == buffBDD[j].var)
				{
					int curi = curIndex(i, &m);
					int curj = curIndex(j, &m);
					if (curi > 1 && curj > 1 && curi != curj)
					{
						Node t1 = buffBDD[curi];
						t1.childFalse = curIndex(t1.childFalse, &m);
						t1.childTrue = curIndex(t1.childTrue, &m);

						Node t2 = buffBDD[curj];
						t2.childFalse = curIndex(t2.childFalse, &m);
						t2.childTrue = curIndex(t2.childTrue, &m);
						if (t1.childFalse == t2.childFalse && t1.childTrue == t2.childTrue)
						{
							m.insert({ curj, curi });
						}
						if (t1.childFalse == t1.childTrue)
						{
							m.insert({ curi, t1.childTrue });
						}
					}
				}
			}
		}
		for (int i = 2; i < buffBDD.size(); ++i)
		{
			int curi = curIndex(i, &m);
			Node t = buffBDD[curi];
			t.childFalse = curIndex(t.childFalse, &m);
			t.childTrue = curIndex(t.childTrue, &m);
			r.ROBDD[curi] = t;
		}
		r.restruct();
		return r;
	}
	else return BDD({});
}



vector<vector<int>> BDD::AllSat(vector <pair <int, int>> v)
{
	static vector <vector <int>> res;
	vector <int> sub(Count, -1);

	if (v.size() > 0)
	{
		int cur;
		if (v[v.size() - 1].first == 0)
		{
			cur = ROBDD[v[v.size() - 1].second].childFalse;
		}
		else
		{
			cur = ROBDD[v[v.size() - 1].second].childTrue;
		}
		if (cur == 1)
		{
			for (int i = 0; i < v.size(); ++i)
				sub[ROBDD[v[i].second].var] = v[i].first;
			res.push_back(sub);
			int st = res.size() - 1;
			for (int i = st; i < res.size(); ++i)
			{
				for (int j = 0; j < Count; ++j)
				{
					if (res[i][j] == -1)
					{
						res[i][j] = 0;
						res.push_back(res[i]);
						res[i][j] = 1;
						i = st;
					}
				}
			}


		}
		else if (cur > 1)
		{
			vector <pair <int, int>> vCopy = v;
			v.push_back({ 0, cur });
			vCopy.push_back({ 1, cur });
			AllSat(v);
			AllSat(vCopy);
		}
	}
	else
	{
		AllSat({ {0, lastVertex} });
		AllSat({ {1, lastVertex} });
	}
	return res;
}

int BDD::getSize()
{
	return lastVertex + 1;
}

int BDD::app(vector <Node> *n, BDD a, BDD b, Node u, Node v, char op)
{
	int p;
	if (u.var == v.var && u.var == a.Count)
	{
		switch (op)
		{
		case '&':
			return u.childFalse & v.childFalse ? 1 : 0;
			break;
		case '|':
			return u.childFalse | v.childFalse ? 1 : 0;
			break;
		case '^':
			return u.childFalse ^ v.childFalse ? 1 : 0;
			break;
		}
	}
	else if (u.var == v.var)
	{
		n->push_back(Node());
		p = n->size() - 1;
		(*n)[p].var = u.var;
		(*n)[p].childFalse = app(n, a, b, a.ROBDD[u.childFalse], b.ROBDD[v.childFalse], op);
		(*n)[p].childTrue = app(n, a, b, a.ROBDD[u.childTrue], b.ROBDD[v.childTrue], op);
	}
	else if (find(a.orderVar.begin(), a.orderVar.end(), u.var) - a.orderVar.begin() <
		find(a.orderVar.begin(), a.orderVar.end(), v.var) - a.orderVar.begin())
	{
		n->push_back(Node());
		p = n->size() - 1;
		(*n)[p].var = u.var;
		(*n)[p].childFalse = app(n, a, b, a.ROBDD[u.childFalse], v, op);
		(*n)[p].childTrue = app(n, a, b, a.ROBDD[u.childTrue], v, op);
	}
	else
	{
		n->push_back(Node());
		p = n->size() - 1;
		(*n)[p].var = v.var;
		(*n)[p].childFalse = app(n, a, b, u, b.ROBDD[v.childFalse], op);
		(*n)[p].childTrue = app(n, a, b, u, b.ROBDD[v.childTrue], op);
	}
	return p;
}

void BDD::printBDD()
{
	cout << sizeTree + 1 << endl;
	for (int i = 0; i < Count; ++i, cout << endl)
	{
		for (int k = pow(2, i); k < pow(2, i + 1); ++k)
		{
			cout << T[sizeTree - k + 2].childFalse << " " << T[sizeTree - k + 2].childTrue << " ";
		}
	}
}

void BDD::printListG1()
{
	for (int i = 2; i < T.size(); ++i)
	{
		cout << i << endl;
	}
	for (int i = 2; i < T.size(); ++i)
	{
		cout << i << " " << T[i].childFalse << " False" << endl;
		cout << i << " " << T[i].childTrue << " True" << endl;
	}
}

void BDD::printListG2()
{
	cout << "0\n1\n";
	for (auto el : ROBDD)
	{
		if (el.second.childFalse > 1)
		{
			cout << "x" << el.second.var + 1 << "_" << el.first << endl;
		}
	}
	for (auto el : ROBDD)
	{
		if (el.first > 1)
		{
			if (el.second.childFalse > 1)
			{
				cout << "x" << el.second.var + 1 << "_" << el.first << " " << "x"
					<< ROBDD[el.second.childFalse].var + 1 << "_" << el.second.childFalse << " False" << endl;
			}
			else
			{
				cout << "x" << el.second.var + 1 << "_" << el.first << " " << el.second.childFalse << " False" << endl;
			}

			if (el.second.childTrue > 1)
			{
				cout << "x" << el.second.var + 1 << "_" << el.first << " " << "x"
					<< ROBDD[el.second.childTrue].var + 1 << "_" << el.second.childTrue << " True" << endl;
			}
			else
			{
				cout << "x" << el.second.var + 1 << "_" << el.first << " " << el.second.childTrue << " True" << endl;
			}
		}

		/*cout << el.first << " " << el.second.childFalse << " False" << endl;
		cout << el.first << " " << el.second.childTrue << " True" << endl;*/

	}
}

bool BDD::test(vector<bool> a)
{
	return F(a);
}