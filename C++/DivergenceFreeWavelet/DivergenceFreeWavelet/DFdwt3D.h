#pragma once
#include "stdafx.h"
#include "Filter.h"
#include "Velocity.h"

class DFdwt3D
{
public:
	SceneType scene;
	Velocity3D	velocityField;
	vector<double***> waveletCoeDF1HHH, waveletCoeDF1HHL, waveletCoeDF1HLH, waveletCoeDF1HLL, waveletCoeDF1LHH, waveletCoeDF1LHL, waveletCoeDF1LLH;
	vector<double***> waveletCoeDF2HHH, waveletCoeDF2HHL, waveletCoeDF2HLH, waveletCoeDF2HLL, waveletCoeDF2LHH, waveletCoeDF2LHL, waveletCoeDF2LLH;
	vector<double***> waveletCoeNHHH, waveletCoeNHHL, waveletCoeNHLH, waveletCoeNHLL, waveletCoeNLHH, waveletCoeNLHL, waveletCoeNLLH;
	vector<double***> waveletCoeULLL, waveletCoeVLLL, waveletCoeWLLL,recWaveletCoeULLL,recWaveletCoeVLLL,recWaveletCoeWLLL;
	vector<int>	waveletCoeSize;
	vector<int> recWaveletCoeSize;
	DFdwt3D(){
		scene = BOUNDARY;
		waveletCoeDF1HHH.clear();
		waveletCoeDF1HHL.clear();
		waveletCoeDF1HLH.clear();
		waveletCoeDF1HLL.clear();
		waveletCoeDF1LHH.clear();
		waveletCoeDF1LHL.clear();
		waveletCoeDF1LLH.clear();
		waveletCoeDF2HHH.clear();
		waveletCoeDF2HHL.clear();
		waveletCoeDF2HLH.clear();
		waveletCoeDF2HLL.clear();
		waveletCoeDF2LHH.clear();
		waveletCoeDF2LHL.clear();
		waveletCoeDF2LLH.clear();
		waveletCoeNHHH.clear();
		waveletCoeNHHL.clear();
		waveletCoeNHLH.clear();
		waveletCoeNHLL.clear();
		waveletCoeNLHH.clear();
		waveletCoeNLHL.clear();
		waveletCoeNLLH.clear();
		waveletCoeULLL.clear();
		waveletCoeVLLL.clear();
		waveletCoeWLLL.clear();
	}
	void loadData(string u_path, string v_path,string w_path, string filter0_path, string filter1_path);
	void wavedec3d(int level);
	void waverec3d(int level);
	Filter filter0, filter1;
	//void shift(double** input, int dir, int input_xsize, int input_ysize, int offset);
	void fwt3d_uvw(double*** u, double*** v, double *** w,int LLsize);// double*** &ull, double*** &uhl, double*** &ulh, double*** &uhh, double*** &vll, double*** &vlh, double*** &vhl, double*** &vhh, int LLsize);
	void ifwt3d_uvw(int level, double*** &u, double*** &v, double*** &w, int LLsize); //(double*** ull, double*** uhl, double*** ulh, double*** uhh, double*** vll, double*** vlh, double*** vhl, double*** vhh, double*** &u, double*** &v, int LLsize);
	void fwt3d(double*** LLL, double*** &lll, double*** &llh, double*** &lhl, double*** &lhh, double*** &hll, double*** &hlh, double*** &hhl, double*** &hhh, SubFilter lodx, SubFilter lody, SubFilter lodz, SubFilter hidx, SubFilter hidy, SubFilter hidz, int LLsize);
	void ifwt3d(double*** lll, double*** llh, double*** lhl, double*** lhh, double*** hll, double*** hlh, double*** hhl, double*** hhh, SubFilter lorx, SubFilter lory, SubFilter lorz, SubFilter hirx, SubFilter hiry, SubFilter hirz, double*** &LLL, int LLsize);
	//void calc();
	void conv_down3d(double*** input, double*** &output, int dir, SubFilter filter, int input_xsize, int input_ysize,int input_zsize);
	void conv_up3d(double*** input, double*** &output, int dir, SubFilter filter, int input_xsize, int input_ysize, int input_zsize);
};