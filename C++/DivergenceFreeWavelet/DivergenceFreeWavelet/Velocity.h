#pragma once
#include "stdafx.h"
class Velocity2D
{
public:
	Velocity2D()
	{
		u = new double*[DIM];
		v = new double*[DIM];
		for (int i = 0; i<DIM; i++)
		{
			u[i] = new double[DIM];
			v[i] = new double[DIM];
		}

	}
	double **u, **v;
};
class Velocity3D
{
public:
	Velocity3D()
	{
		u = new double**[DIM];
		v = new double**[DIM];
		w = new double**[DIM];
		for (int i = 0; i<DIM; i++)
		{
			u[i] = new double*[DIM];
			v[i] = new double*[DIM];
			w[i] = new double*[DIM];
			for (int j = 0; j < DIM; j++)
			{
				u[i][j] = new double[DIM];
				v[i][j] = new double[DIM];
				w[i][j] = new double[DIM];
			}
		}

	}
	double ***u, ***v,***w;
};