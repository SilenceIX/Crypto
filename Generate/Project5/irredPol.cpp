#include "irredPol.h"


namespace irred
{
	polynoms irreduciblePolynoms(int order, int mod)
	{
		polynoms pols;
		polynom pol(order + 1);
		int variation = mod;
		std::ofstream f1("irredPol.txt", std::ios_base::app);
		f1 << "\nOrder: " << order << " Mod: " << mod << std::endl;
		int org = pow(10, order - 4) * (mod / 4);
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
			{

				f1 << "{";
				for (int i = 0; i < pol.size(); ++i)
				{
					f1 << pol[i];
					if (i < pol.size() - 1)
					{
						f1 << ", ";
					}
				}
				f1 << "}," << std::endl;
			}
		}
		return pols;
	}
}
