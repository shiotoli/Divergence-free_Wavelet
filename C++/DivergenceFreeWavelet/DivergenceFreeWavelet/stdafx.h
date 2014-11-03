#pragma once
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <map>
using namespace std;
#define DIM				8
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