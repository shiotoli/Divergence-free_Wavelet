#pragma once
#include "stdafx.h"
class Velocity2D
{
public:
	Velocity2D()
	{
		u = new double*[DIM];
		v = new double*[DIM];
		for (int i = 0;i<DIM;i++)
		{
			u[i] = new double[DIM];
			v[i] = new double[DIM];
		}

	}
	double **u,**v;
};