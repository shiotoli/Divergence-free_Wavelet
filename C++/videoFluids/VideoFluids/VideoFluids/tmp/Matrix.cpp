#include "Matrix.h"

void DotMul(Matrix& x,Matrix& y,Matrix& z)
{
	for (int i = 0;i<x.n;i++)
		for (int j = 0;j<x.m;j++)
			z[i][j] = x[i][j]*y[i][j];
}
void Partial(Matrix& x, Matrix& y,int axis)
{
	switch (axis)
	{
	case AXIS_X:
		for (int i = 0;i<x.n;i++)
			for (int j = 1;j<x.m-1;j++)
				y[i][j] = (x[i][j+1]-x[i][j-1])/2;
		for (int i = 0;i<x.n;i++)
			y[i][0] = (-3*x[i][0]+4*x[i][1]-x[i][2])/2;
		for (int i = 0;i<x.n;i++)
			y[i][x.m-1] = (3*x[i][x.m-1]-4*x[i][x.m-2]+x[i][x.m-3])/2;
		break;
	case AXIS_Y:
		for (int i = 1;i<x.n-1;i++)
			for (int j = 0;j<x.m;j++)
				y[i][j] = (x[i+1][j]-x[i-1][j])/2;
		for (int i = 0;i<x.m;i++)
			y[0][i] = (-3*x[0][i]+4*x[1][i]-x[2][i])/2;
		for (int i = 0;i<x.m;i++)
			y[x.n-1][i] = (3*x[x.n-1][i]-4*x[x.n-2][i]+x[x.n-3][i])/2;
		break;
	}
}
//u~:AXIS_X u^_:AXIS_Y
void Average(Matrix& x,Matrix& y,int axis)
{
	if (axis == AXIS_X)
	{
		for (int i = 0;i<x.n;i++)
			for (int j = 0;j<x.m;j++)
			{
				if (j == 0||j==x.m-1)
					y[i][j] = x[i][j];
				else
					y[i][j] = (x[i][j-1]+x[i][j+1])/2;
			}
	}
	else if (axis == AXIS_Y)
	{
		for (int i = 0;i<x.n;i++)
			for (int j = 0;j<x.m;j++)
			{
				if (i==0 ||i == x.n-1)
					y[i][j] = x[i][j];
				else
					y[i][j] = (x[i-1][j]+x[i+1][j])/2;
			}
	}
}
float Difference(Matrix& x,Matrix& y)
{
	float di = 0;
	for (int i = 0;i<x.n;i++)
		for (int j = 0;j<x.m;j++)
			di+=sqr(x[i][j]-y[i][j]);
//	return sqr(x[x.n/2][x.m/2]-y[x.n/2][x.m/2]);
	return di;
}