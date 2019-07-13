#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <map>
namespace irred
{
	typedef std::vector <int> polynom;
	typedef std::vector <polynom> polynoms;
	polynoms irreduciblePolynoms(int order, int mod);
}
