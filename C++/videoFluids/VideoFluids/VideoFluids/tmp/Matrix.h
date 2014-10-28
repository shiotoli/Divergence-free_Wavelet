#pragma once
#include "stdafx.h"
class Vect
{
public:
	Vect()
	{
		m_data = nullptr;
	}
	Vect(int _n)
	{
		n = _n;
		m_data = new float[_n];
	}
	~Vect()
	{
		destroyVector();
	}
	void createVector(int _n)
	{
		if (m_data != nullptr)
		{
			destroyVector();
		}
		n = _n;
		m_data = new float[_n];
	}
	void destroyVector()
	{
		if (m_data != nullptr)
		{
			delete[] m_data;
		}
		m_data = nullptr;
	}
	float& operator[](int k)
	{
		return m_data[k];
	}
	float operator = (float f)
	{
		for (int i = 0;i<n;i++)
			m_data[i] = f;
		return f;
	}
	void randomize(float start = 0,float end = 1)
	{
		for (int i = 0;i<n;i++)
		{
			m_data[i] = (rand()%1000)/1000.0*(end-start)+start;
		}
	}
private:
	float* m_data;
	int n;
};
class Matrix
{
public:
	Matrix()
	{
		m_data = nullptr;
	}
	Matrix(int _n,int _m)
	{
		n = _n;
		m = _m;
		m_data = new float[_n*_m];
	}
	void createMatrix(int _n,int _m)
	{
		if (m_data != nullptr)
		{
			destroyMatrix();
		}
		n = _n;
		m = _m;
		m_data = new float[_n*_m];
	}
	~Matrix()
	{
//		destroyMatrix();
	}
	void destroyMatrix()
	{
		if (m_data != nullptr)
		{
			delete[] m_data;
		}
		m_data = nullptr;
	}
	float* operator[](int k) const
	{
		return m_data + k*m;
	}
	float operator = (float f)
	{
		for (int i = 0;i<n*m;i++)
			m_data[i] = f;
		return f;
	}
	Matrix& operator= (const Matrix& ma)
	{
		if (ma.n!=n || ma.m!=m)
		{
			cout<<"error! Copy diffent dimension\n";
		}
		for (int i = 0;i<n;i++)
			for (int j = 0;j<m;j++)
				m_data[i*m+j] = ma[i][j];
		return *this;
	}
	Matrix operator+ (const Matrix& ma)
	{
		Matrix tmpm(ma.n,ma.m);
		if (ma.n!=n || ma.m!=m)
		{
			cout<<"error! Add diffent dimension\n";
		}
		for (int i = 0;i<n;i++)
			for (int j = 0;j<m;j++)
				tmpm[i][j] = m_data[i*m+j]+ma[i][j];
		return tmpm;
	}
	Matrix operator- (const Matrix& ma)
	{
		Matrix tmpm(ma.n,ma.m);
		if (ma.n!=n || ma.m!=m)
		{
			cout<<"error! Add diffent dimension\n";
		}
		for (int i = 0;i<n;i++)
			for (int j = 0;j<m;j++)
				tmpm[i][j] = m_data[i*m+j]-ma[i][j];
		return tmpm;
	}
	Matrix clone()
	{
		Matrix ma(n,m);
		for (int i = 0;i<n;i++)
			for (int j = 0;j<m;j++)
				ma[i][j] = m_data[i*m+j];
		return ma;
	}
	Matrix operator* (const float f)
	{
		Matrix tmpm(n,m);
		for (int i = 0;i<n;i++)
			for (int j = 0;j<m;j++)
				tmpm[i][j] = m_data[i*m+j]*f;
		return tmpm;
	}
	Matrix operator/ (const float f)
	{
		Matrix tmpm(n,m);
		for (int i = 0;i<n;i++)
			for (int j = 0;j<m;j++)
				tmpm[i][j] = m_data[i*m+j]/f;
		return tmpm;
	}
	Matrix& operator+= (const Matrix& ma)
	{
		if (ma.n!=n || ma.m!=m)
		{
			cout<<"error! Add Equal diffent dimension\n";
		}
		for (int i = 0;i<n;i++)
			for (int j = 0;j<m;j++)
				m_data[i*m+j]+=ma[i][j];
		return *this;
	}
	Matrix& operator-= (const Matrix& ma)
	{
		if (ma.n!=n || ma.m!=m)
		{
			cout<<"error! Add diffent dimension\n";
		}
		for (int i = 0;i<n;i++)
			for (int j = 0;j<m;j++)
				m_data[i*m+j]-=ma[i][j];
		return *this;
	}
	Matrix& operator*= (const float f)
	{
		for (int i = 0;i<n;i++)
			for (int j = 0;j<m;j++)
				m_data[i*m+j]*=f;
		return *this;
	}
	Matrix& operator/= (const float f)
	{
		for (int i = 0;i<n;i++)
			for (int j = 0;j<m;j++)
				m_data[i*m+j]/=f;
		return *this;
	}
	Matrix& operator+=(const float f)
	{
		for (int i = 0;i<n;i++)
			for (int j = 0;j<m;j++)
				m_data[i*m+j]+=f;
		return *this;
	}
	Matrix& operator-=(const float f)
	{
		for (int i = 0;i<n;i++)
			for (int j = 0;j<m;j++)
				m_data[i*m+j]-=f;
		return *this;
	}
	void randomize(float start = 0,float end = 1)
	{
		for (int i = 0;i<n*m;i++)
		{
			m_data[i] = (rand()%1000)/1000.0*(end-start)+start;
		}
	}
	void output(FILE* fp)
	{
		fprintf(fp,"%d %d\n",n,m);
		for (int i = 0;i<n;i++)
		{
			for (int j = 0;j<m;j++)
				fprintf(fp,"%f ",m_data[i*m+j]);
			fprintf(fp,"\n");
		}
	}
	void output(string s)
	{
		FILE* fp;
		fp = fopen(s.c_str(),"w");
		fprintf(fp,"%d %d\n",n,m);
		for (int i = 0;i<n;i++)
		{
			for (int j = 0;j<m;j++)
				fprintf(fp,"%f ",m_data[i*m+j]);
			fprintf(fp,"\n");
		}
		fclose(fp);
	}
	int n,m;
/*	Matrix operator+(Matrix ma)
	{
		for (int i = 0;i<n;i++)
			;
	}
	*/
private:
	float* m_data;
};
class Matrix3
{
public:
	Matrix3()
	{
		m_data = nullptr;
	}
	Matrix3(int _n,int _m,int _l)
	{
		n = _n;
		m = _m;
		l = _l;
		m_data = new float*[n*m];
		for (int i = 0;i<n*m;i++)
			m_data[i] = new float[l];
	}
	void createMatrix(int _n,int _m,int _l)
	{
		if (m_data != nullptr)
		{
			destroyMatrix();
		}
		n = _n;
		m = _m;
		l = _l;
		m_data = new float*[n*m];
		for (int i = 0;i<n*m;i++)
			m_data[i] = new float[l];
	}
	float** operator[](int k)
	{
		return m_data + k*m;
	}
	~Matrix3()
	{
		destroyMatrix();
	}
	void destroyMatrix()
	{
		if (m_data != nullptr)
		{
			for (int i = 0;i<n*m;i++)
				delete[] m_data[i];
			delete[] m_data;
		}
		m_data = nullptr;
	}
	float operator = (float f)
	{
		for (int i = 0;i<n*m;i++)
			for (int j = 0;j<l;j++)
				m_data[i][j] = f;
		return f;
	}
	void randomize(float start = 0,float end = 1)
	{
		for (int i = 0;i<n*m;i++)
			for (int j = 0;j<l;j++)
			{
				m_data[i][j] = (rand()%1000)/1000.0*(end-start)+start;
			}
	}
private:
	float** m_data;
	int n,m,l;	
};
void DotMul(Matrix& x,Matrix& y,Matrix& z);
void Partial(Matrix& x, Matrix& y,int axis);
void Average(Matrix& x,Matrix& y,int axis);
float Difference(Matrix& x,Matrix& y);