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
			Node t1 = T[curi];
			t1.childFalse = curIndex(t1.childFalse, &m);
			t1.childTrue = curIndex(t1.childTrue, &m);
			if (curi > 1 && curj > 1 && curi != curj)
			{

				Node t2 = T[curj];
				t2.childFalse = curIndex(t2.childFalse, &m);
				t2.childTrue = curIndex(t2.childTrue, &m);
				if (t1.childFalse == t2.childFalse && t1.childTrue == t2.childTrue && t1.var == t2.var)
				{
					m.insert({ curj, curi });
				}
			}
		}

		int curi = curIndex(i, &m);
		if (curi > 1)
		{
			Node t1 = T[curi];
			t1.childFalse = curIndex(t1.childFalse, &m);
			t1.childTrue = curIndex(t1.childTrue, &m);
			if (t1.childFalse == t1.childTrue)
			{
				m.insert({ curi, t1.childTrue });
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
	if (ROBDD.size() == 1)
	{
		return;
	}

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

void BDD::corrROBDD(map <int, Node> buffBDD)
{
	map <int, int> m;
	

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
					if (t1.childFalse == t2.childFalse && t1.childTrue == t2.childTrue && t1.var == t2.var && t1.var != -1)
					{
						m.insert({ curj, curi });
					}

				}
			}
			int curi = curIndex(i, &m);
			Node t1 = buffBDD[curi];
			t1.childFalse = curIndex(t1.childFalse, &m);
			t1.childTrue = curIndex(t1.childTrue, &m);
			if (t1.childFalse == t1.childTrue && curi > 1 && t1.var != -1)
			{
				m.insert({ curi, t1.childTrue });
			}
		}
	}
	ROBDD.clear();

	int trueBalance = 0;
	int falseBalance = 0;
	for (auto v : m)
	{
		if (v.second == 0)
		{
			trueBalance++;
		}
		else
		if (v.second == 1)
		{
			falseBalance++;
		}
		else
		{
			trueBalance++;
			falseBalance++;
			break;
		}
	}
	for (int i = 2; i < buffBDD.size(); ++i)
	{
		int curi = curIndex(i, &m);
		Node t = buffBDD[curi];
		if (t.var != -1)
		{
			t.childFalse = curIndex(t.childFalse, &m);
			t.childTrue = curIndex(t.childTrue, &m);
			ROBDD[curi] = t;
		}
	}

	if (ROBDD.size() == 2)
	{
		if (trueBalance == 0)
		{
			ROBDD[1] = buffBDD[1];
		}
		else if (falseBalance == 0)
		{
			ROBDD[0] = buffBDD[0];
		}
	}
}

void BDD::destroyEqualVertex(int pred ,int v, vector <int> x)
{
	v = curIndex(v, &copyNodeF2);
	pred = curIndex(pred, &copyNodeF2);
	if (ROBDD[v].var != Count)
	{
		if (x[ROBDD[v].var] == -1)
		{
			x[ROBDD[v].var] = 0;
			ROBDD[v].childFalse = curIndex(ROBDD[v].childFalse, &copyNodeF2);
			destroyEqualVertex(v, ROBDD[v].childFalse, x);
			x[ROBDD[v].var] = 1;
			ROBDD[v].childTrue = curIndex(ROBDD[v].childTrue, &copyNodeF2);
			destroyEqualVertex(v, ROBDD[v].childTrue, x);
		}
		else if (x[ROBDD[v].var] == 0)
		{			
			if (x[ROBDD[pred].var] == 0)
			{
				lastVertex++;
				ROBDD[lastVertex] = ROBDD[pred];
				copyNodeF1[pred] = lastVertex;
				ROBDD[pred].childFalse = ROBDD[v].childFalse;
			}
			else 
			{
				ROBDD[pred].childTrue = ROBDD[v].childFalse;
				int k = curIndex(pred, &copyNodeF1);
				copyNodeF2[pred] = k;
			}
			ROBDD[v].childFalse = curIndex(ROBDD[v].childFalse, &copyNodeF2);
			destroyEqualVertex(pred, ROBDD[v].childFalse, x);
		}
		else if (x[ROBDD[v].var] == 1)
		{
			if (x[ROBDD[pred].var] == 0)
			{
				lastVertex++;
				ROBDD[lastVertex] = ROBDD[pred];
				copyNodeF1[pred] = lastVertex;
				ROBDD[pred].childFalse = ROBDD[v].childTrue;
			}
			else
			{
				ROBDD[pred].childTrue = ROBDD[v].childTrue;
				int k = curIndex(pred, &copyNodeF1);
				copyNodeF2[pred] = k;
			}
			ROBDD[v].childTrue = curIndex(ROBDD[v].childTrue, &copyNodeF2);
			destroyEqualVertex(pred, ROBDD[v].childTrue, x);
		}
	}	
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
		T[(sizeTree - 1) - i + 2].childTrue = (sizeTree - 1) - 2 * i + 1;
		T[(sizeTree - 1) - i + 2].childFalse = (sizeTree - 1) - 2 * i;
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
		r.corrROBDD(buffBDD);
		r.restruct();
		return r;
	}
	else return BDD({});
}

void BDD::insertBDD(BDD x, int var)
{
	if (ROBDD.size() == 1)
	{
		return;
	}
	int v = lastVertex;

	int size = ROBDD.size();
	for (int i = 2; i < size; ++i)
	{
		if (ROBDD[i].var == var)
		{
			map <int, int> m;
			if (x.ROBDD.size() == 1)
			{
				for (auto vertex : x.ROBDD)
				{
					if (vertex.first == 0)
					{
						m[i] = ROBDD[i].childFalse;
					}
					else
					{
						m[i] = ROBDD[i].childTrue;
					}
				}
				if (v == i)
				{
					v = m[i];
				}
				for (int i = 0; i < ROBDD.size(); ++i)
				{
					ROBDD[i].childFalse = curIndex(ROBDD[i].childFalse, &m);
					ROBDD[i].childTrue = curIndex(ROBDD[i].childTrue, &m);
				}
			}
			else
			{
				m[0] = ROBDD[i].childFalse;
				m[1] = ROBDD[i].childTrue;
				ROBDD[i] = x.ROBDD[x.lastVertex];	
				int ost = lastVertex + 1;
				for (int j = 2; j < x.ROBDD.size() - 1; ++j)
				{
					++lastVertex;
					ROBDD[lastVertex] = x.ROBDD[j];
					m[j] = lastVertex;
				}
				ROBDD[i].childFalse = m[ROBDD[i].childFalse];
				ROBDD[i].childTrue = m[ROBDD[i].childTrue];
				for (int j = ost; j < ROBDD.size(); ++j)
				{
					ROBDD[j].childFalse = m[ROBDD[j].childFalse];
					ROBDD[j].childTrue = m[ROBDD[j].childTrue];
				}
			}

		}
	}
	copyNodeF1.clear();
	copyNodeF2.clear();
	destroyEqualVertex(-1, v, vector <int>(Count, -1));

 	queue <int> q;
	map <int, int> m;
	q.push(v);
	int c = 2;
	while (!q.empty())
	{
		int i = q.front();
		q.pop();
		if (ROBDD[i].childFalse > 1)
		{
			q.push(ROBDD[i].childFalse);
		}
		if (ROBDD[i].childTrue > 1)
		{
			q.push(ROBDD[i].childTrue);
		}
		if (m.find(i) == m.end())
		{
			m[i] = ROBDD.size() - 1 - c + 2;
			++c;
		}
	}
	map <int, Node> buffBDD;
	m[0] = 0;
	m[1] = 1;
	for (int i = 0; i < ROBDD.size(); ++i)
	{
		Node curN = ROBDD[i];
		int curi = m.find(i) != m.end() ? m[i] : -1;
		if (curi != -1)
		{
			curN.childFalse = m.find(curN.childFalse) != m.end() ? m[curN.childFalse] : curN.childFalse;
			curN.childTrue = m.find(curN.childTrue) != m.end() ? m[curN.childTrue] : curN.childTrue;
			buffBDD[curi] = curN;
		}
	}

	corrROBDD(buffBDD);
	restruct();



	return;
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
	ofstream f("outG.txt", ios_base::app);
	if (ROBDD.size() == 1)
	{
		for (auto el : ROBDD)
		{
			f << el.first <<endl;
		}
		f.close();
		return;
	}
	f << "0\n1\n";
	for (auto el : ROBDD)
	{
		if (el.second.childFalse > 1)
		{
			f << "x" << el.second.var + 1 << "_" << el.first << endl;
		}
	}
	for (auto el : ROBDD)
	{
		if (el.first > 1)
		{
			if (el.second.childFalse > 1)
			{
				f << "x" << el.second.var + 1 << "_" << el.first << " " << "x"
					<< ROBDD[el.second.childFalse].var + 1 << "_" << el.second.childFalse << " False" << endl;
			}
			else
			{
				f << "x" << el.second.var + 1 << "_" << el.first << " " << el.second.childFalse << " False" << endl;
			}

			if (el.second.childTrue > 1)
			{
				f << "x" << el.second.var + 1 << "_" << el.first << " " << "x"
					<< ROBDD[el.second.childTrue].var + 1 << "_" << el.second.childTrue << " True" << endl;
			}
			else
			{
				f << "x" << el.second.var + 1 << "_" << el.first << " " << el.second.childTrue << " True" << endl;
			}
		}		
		/*cout << el.first << " " << el.second.childFalse << " False" << endl;
		cout << el.first << " " << el.second.childTrue << " True" << endl;*/

	}
	f.close();
}

bool BDD::getValue(vector <bool> k)
{

	Node v = ROBDD[lastVertex];
	int r;
	while (v.var != Count)
	{
		if (k[orderVar[v.var]])
		{
			r = v.childTrue;
			v = ROBDD[v.childTrue];
		}
		else
		{
			r = v.childFalse;
			v = ROBDD[v.childFalse];
		}
	}
	return r;
}

bool BDD::test(vector<bool> a)
{
	return F(a);
}



APNBDD::APNBDD(vector<function<bool(vector<bool>)>> f, vector <int> o)
{
	order = o;
	for (int i = 0; i < f.size(); ++i)
	{
		func.push_back(BDD(f[i], order));
	}
}

APNBDD::APNBDD(vector<int> seq, vector <int> o)
{
	int rang = log2(seq.size());
	order = o;
	if (order.empty())
	{
		for (int i = 0; i < rang; ++i)
		{
			order.push_back(i);
		}
	}
	for (int i = 0; i < rang; ++i)
	{
		vector <bool> f(seq.size());
		for (int j = 0; j < seq.size(); ++j)
		{
			f[j] = seq[j] >> i & 1;
		}
		func.push_back(BDD(f, order));
	}
}

int APNBDD::getValue(int arg)
{
	vector <bool> k(order.size());
	for (int i = 0; i < order.size(); ++i)
	{
		k[i] = arg >> i & 1;
	}
	int r = 0;
	for (int i = 0; i < func.size(); ++i)
	{
		r += func[i].getValue(k) << i;
	}

	
	return r;
}

int APNBDD::getDig()
{
	int r = 0;
	for (int i = 0; i < func.size(); ++i)
	{
		r += func[i].getSize();
	}
	return r;
}

void APNBDD::printGraphs()
{
	for (int i = 0; i < func.size(); ++i)
	{
		func[i].printListG2();
		cout << endl;
	}
}