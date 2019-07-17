#include "irredPol.h"


namespace irred
{
	polynoms irreduciblePolynoms(int order, int mod)
	{
		polynoms pols;
		polynom pol(order + 1);
		int variation = mod - 1;
		for (int i = pow(variation, order); i < 2 * pow(variation, order); ++i)
		{
			int buff = i;
			for (int k = 0; k <= order; ++k)
			{
				pol[order - k] = buff % variation;
				buff /= variation;
			}
			bool f = true;
			for (int x = 0; x <= variation; ++x)
			{
				int sum = 0;
				int gx = 1;
				for (int j = 0; j <= order; ++j)
				{
					sum += gx * pol[order - j];
					gx *= x;
				}
				if (sum % mod == 0)
				{
					f = false;
					break;
				}
			}
			if (f)
				pols.push_back(pol);
		}
		return pols;
	}
}
