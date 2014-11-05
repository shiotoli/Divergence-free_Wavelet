#pragma once
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <map>
using namespace std;
#define DIM				512
#define GRID_SIZE		512

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
inline void new1D(double* &tmp,int size)
{
	tmp = new double[size];
}
inline void new2D(double** &tmp,int row, int col)
{
	tmp = new double*[row];
	for (int i = 0;i<row;i++)
		tmp[i] = new double[col];
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
inline double calcDivergence_biggest(double** u,double** v,int row,int col)
{
	double sum = -1;
	for (int i=1;i<row;i++ )
		for (int j = 1;j<col;j++)
			if (abs(u[i][j]-u[i][(j-1+col)%col]+v[i][j]-v[(i-1+row)%row][j])>sum)
				sum = abs(u[i][j]-u[i][(j-1+col)%col]+v[i][j]-v[(i-1+row)%row][j]);
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
inline void mul(double** u,double** v,int row,int col,double multipler)
{
	for (int i = 0;i<row;i++)
		for (int j = 0;j<col;j++)
		{
			u[i][j]*=multipler;
			v[i][j]*=0.0;//multipler;
		}
}
inline void set_field(double** u,double** v,int row,int col,double uvalue,double vvalue)
{
	for (int i = 0;i<row;i++)
		for (int j = 0;j<col;j++)
		{
			u[i][j]=uvalue;
			v[i][j]=vvalue;
		}
}