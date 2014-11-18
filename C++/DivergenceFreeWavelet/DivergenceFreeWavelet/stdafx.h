#pragma once
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <map>
using namespace std;
#define DIM				128
#define GRID_SIZE		512
#define DUAL_CONVOLUTION
enum SceneType
{
	LOOP,
	BOUNDARY
};
inline void release1D(double* tmp)
{
	delete[] tmp;
}
inline void release2D(double** tmp,int size)
{
	for (int i = 0;i<size;i++)
		delete[] tmp[i];
	delete[] tmp;
}
inline void release3D(double*** &tmp, int xn, int yn)
{
	for (int i = 0; i<xn; i++)
	for (int j = 0; j < yn; j++)
		delete[] tmp[i][j];
	for (int i = 0; i < xn; i++)
		delete[] tmp[i];
	delete[] tmp;
}
inline void new1D(double* &tmp,int size)
{
	tmp = new double[size];
}
inline void new2D(double** &tmp, int row, int col)
{
	tmp = new double*[row];
	for (int i = 0; i<row; i++)
		tmp[i] = new double[col];
}
inline void new3D(double*** &tmp, int xn, int yn,int zn)
{
	tmp = new double**[xn];
	for (int i = 0; i < xn; i++)
	{
		tmp[i] = new double*[yn];
		for (int j = 0; j < yn; j++)
			tmp[i][j] = new double[zn];
	}
}
inline void reset3D(double*** tmp, int xn, int yn, int zn)
{
	for (int i = 0; i < xn;i++)
	for (int j = 0; j < yn;j++)
	for (int k = 0; k < zn; k++)
		tmp[i][j][k] = 0;
}
inline void print2d(string path,double** tmp,int row, int col)
{
	FILE* fp = fopen(path.c_str(),"w");
	for (int i =0;i<row;i++)
	{
		for (int j = 0;j<col;j++)
			fprintf(fp,"%lf ",tmp[i][j]);
		fprintf(fp,"\n");
	}
	fclose(fp);
}
inline void swap(double& a,double &b)
{
	double c = a;
	a = b;
	b = c;
}
inline double calcDivergence_biggest(double** u, double** v, int row, int col)
{
	double sum = -1;
	for (int i = 1; i<row; i++)
	for (int j = 1; j<col; j++)
	if (abs(u[i][j] - u[i][(j - 1 + col) % col] + v[i][j] - v[(i - 1 + row) % row][j])>sum)
		sum = abs(u[i][j] - u[i][(j - 1 + col) % col] + v[i][j] - v[(i - 1 + row) % row][j]);
	return sum;
}
inline double calcDivergence_biggest(double*** u, double*** v, double*** w, int xn, int yn, int zn, SceneType st)
{
	double sum = -1;
	if (st == LOOP)
	{
		for (int i = 0; i<xn; i++)
		for (int j = 0; j<yn; j++)
		for (int k = 0; k<zn; k++)
		{
			if (abs(u[i][j][k] - u[(i - 1 + xn) % xn][j][k] + v[i][j][k] - v[i][(j - 1 + yn) % yn][k] + w[i][j][k] - w[i][j][(k - 1 + zn) % zn])>sum)
				sum = abs(u[i][j][k] - u[(i - 1 + xn) % xn][j][k] + v[i][j][k] - v[i][(j - 1 + yn) % yn][k] + w[i][j][k] - w[i][j][(k - 1 + zn) % zn]);
		}
	}
	else if (st == BOUNDARY)
	{
		for (int i = 2; i<xn-1; i++)
		for (int j = 2; j<yn-1; j++)
		for (int k = 2; k<zn-1; k++)
		{
			if (abs(u[i][j][k] - u[i - 1][j][k] + v[i][j][k] - v[i][j - 1][k] + w[i][j][k] - w[i][j][k - 1])>sum)
				sum = abs(u[i][j][k] - u[i - 1][j][k] + v[i][j][k] - v[i][j - 1][k] + w[i][j][k] - w[i][j][k - 1]);
		}
	}
	return sum;
}
inline double calcDivergence_sum(double** u,double** v,int row,int col)
{
	double sum = 0;
	for (int i=0;i<row;i++ )
		for (int j = 0;j<col;j++)
			sum += abs(u[i][j]-u[i][(j-1+col)%col]+v[i][j]-v[(i-1+row)%row][j]);
	return sum;
}
inline double calcDivergence_sum(double*** u, double*** v, double*** w, int xn, int yn, int zn, SceneType st)
{
	double sum = 0;
	if (st == LOOP)
	{
		for (int i = 0; i<xn; i++)
		for (int j = 0; j<yn; j++)
		for (int k = 0; k<zn; k++)
		{
		//	if (abs(u[i][j][k] - u[(i - 1 + xn) % xn][j][k] + v[i][j][k] - v[i][(j - 1 + yn) % yn][k] + w[i][j][k] - w[i][j][(k - 1 + zn) % zn])>sum)
			sum += abs(u[i][j][k] - u[(i - 1 + xn) % xn][j][k] + v[i][j][k] - v[i][(j - 1 + yn) % yn][k] + w[i][j][k] - w[i][j][(k - 1 + zn) % zn]);
		}
	}
	else if (st == BOUNDARY)
	{
		for (int i = 1; i<xn; i++)
		for (int j = 1; j<yn; j++)
		for (int k = 1; k<zn; k++)
		{
		//	if (abs(u[i][j][k] - u[(i - 1 + xn) % xn][j][k] + v[i][j][k] - v[i][(j - 1 + yn) % yn][k] + w[i][j][k] - w[i][j][(k - 1 + zn) % zn])>sum)
			sum += abs(u[i][j][k] - u[i-1][j][k] + v[i][j][k] - v[i][j-1][k] + w[i][j][k] - w[i][j][k-1]);
		}
	}
	return sum;
}
inline void mul(double** u, double** v, int row, int col, double multipler)
{
	for (int i = 0; i<row; i++)
	for (int j = 0; j<col; j++)
	{
		u[i][j] *= multipler;
		v[i][j] *= 0.0;//multipler;
	}
}
inline void mul(double*** u, int xn, int yn, int zn, double multipler)
{
	for (int i = 0; i < xn; i++)
	for (int j = 0; j < yn; j++)
	for (int k = 0; k < zn; k++)
	{
		u[i][j][k] *= multipler;
	}
}
inline void set_field(double*** u, int xn, int yn, int zn, double v)
{
	for (int i = 0; i<xn; i++)
	for (int j = 0; j<yn; j++)
	for (int k = 0; k<zn; k++)
	{
		u[i][j][k] = v;
	}
}
inline void set_field(double** u, int row, int col, double v)
{
	for (int i = 0; i<row; i++)
	for (int j = 0; j<col; j++)
	{
		u[i][j] = v;
	}
}
inline void set_field_rand(double*** u, int xn, int yn, int zn, double v)
{
	for (int i = 0; i<xn; i++)
	for (int j = 0; j<yn; j++)
	for (int k = 0; k<zn; k++)
	{
		u[i][j][k] = rand() % 1000 / 100000.0;
	}
}
inline void set_field_rand(double** u,double** v,int row,int col)
{
	for (int i = 0;i<row;i++)
		for (int j = 0;j<col;j++)
		{
			u[i][j]=rand()%1000/100000.0;
			v[i][j]=0.0;
		}
}
inline void addMatrix_3d(double*** a, double*** b, double*** c, int nx, int ny, int nz)
{
	for (int i = 0; i < nx;i++)
	for (int j = 0; j < ny;j++)
	for (int k = 0; k < nz; k++)
		c[i][j][k] = a[i][j][k] + b[i][j][k];
}