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
	vector<double**> waveletCoeHH;
	vector<double**> waveletCoeHL;
	vector<double**> waveletCoeLH;
	vector<double**> waveletCoeLL;
	vector<int>	waveletCoeSize;
	DFdwt2D(){scene = LOOP;waveletCoeLL.clear();waveletCoeHH.clear();waveletCoeLH.clear();waveletCoeHL.clear();waveletCoeSize.clear();}
	void loadData(string u_path,string v_path,string filterx_path,string filtery_path);
	Filter filter0,filter1;
	void fwt2d_uv(double** u,double** v,double** ull,double** uhl,double** ulh,double** uhh,double** vll,double** vlh,double** vhl,double** vhh,int LLsize);
	void fwt2d(double** LL,double** &ll,double** &hl,double** &lh,double** &hh,SubFilter lodx,SubFilter lody,SubFilter hidx,SubFilter hidy,int LLsize);
	void calc();
	void conv2d(double** input,double** &output,int dir,SubFilter filter,int input_xsize,int input_ysize);
};