#pragma once
#include "stdafx.h"
#include "Filter.h"
#include "Velocity.h"
class DFdwt2D
{
public:
	enum sceneType 
	{
		LOOP,
		BOUNDARY
	}scene;
	Velocity2D	velocityField;
	vector<double**> waveletCoeVHH;
	vector<double**> waveletCoeVHL;
	vector<double**> waveletCoeVLH;
	vector<double**> waveletCoeVLL;
	vector<double**> waveletCoeUHH;
	vector<double**> waveletCoeUHL;
	vector<double**> waveletCoeULH;
	vector<double**> waveletCoeULL;
	vector<int>	waveletCoeSize;
	DFdwt2D(){scene = LOOP;waveletCoeULL.clear();waveletCoeUHH.clear();waveletCoeULH.clear();waveletCoeUHL.clear();
	waveletCoeVLL.clear();waveletCoeVHH.clear();waveletCoeVLH.clear();waveletCoeVHL.clear();waveletCoeSize.clear();}
	void loadData(string u_path,string v_path,string filter0_path,string filter1_path);
	void wavedec2d(int level);
	void waverec2d(int level);
	Filter filter0,filter1;
	void fwt2d_uv(double** u,double** v,double** &ull,double** &uhl,double** &ulh,double** &uhh,double** &vll,double** &vlh,double** &vhl,double** &vhh,int LLsize);
	void ifwt2d_uv(double** ull,double** uhl,double** ulh,double** uhh,double** vll,double** vlh,double** vhl,double** vhh,double** &u,double** &v,int LLsize);
	void fwt2d(double** LL,double** &ll,double** &hl,double** &lh,double** &hh,SubFilter lodx,SubFilter lody,SubFilter hidx,SubFilter hidy,int LLsize);
	void ifwt2d(double** ll,double** hl,double** lh,double** hh,SubFilter lodx,SubFilter lody,SubFilter hidx,SubFilter hidy, double** &LL,int LLsize);
	//void calc();
	void conv_down2d(double** input,double** &output,int dir,SubFilter filter,int input_xsize,int input_ysize);
	void conv_up2d(double** input,double** &output,int dir,SubFilter filter,int input_xsize,int input_ysize);
};