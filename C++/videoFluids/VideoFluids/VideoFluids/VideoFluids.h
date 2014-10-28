#pragma once
#include "stdafx.h"
#include "SFS.h"

void meshlize(string s,Matrix& m);
class VideoFluids
{
public:
	VideoFluids()
	{
		config = new libconfig::Config();
		config->readFile("../VideoFluids.cfg");
		sfs = new SFS(config);
		img_list.clear();
		sfs_list.clear();
		CONFIG_SAFE_CALL(config->lookupValue("Fitting.Alpha", alpha));
		CONFIG_SAFE_CALL(config->lookupValue("Fitting.IterationTime",iterationTime));
		CONFIG_SAFE_CALL(config->lookupValue("Fitting.IterationTorlerance",iterationTorlerance));
		height = width = -1;
	};
	int height,width;
	inline void setSize(int h,int w)
	{
		height = h;
		width = w;
	}
	libconfig::Config* config;
	float iterationTorlerance;
	SFS*	sfs;
	float alpha;
	int iterationTime;
	vector<Matrix> sfs_list;
	vector<Mat> img_list;
	
	void processSFS();
	void filter();
	void meshResult();
	void meshRec();
	void speedColor(int number,Matrix& u,Matrix& v);
	void addPic(Mat m);
	void updateField(Matrix& Zn,Matrix& U,Matrix& V,Matrix& Z_result);
	void trackVelocity(Matrix& Zn1,Matrix& Zn,Matrix& U,Matrix& V);
	void calcFluid();

	vector<vector<Matrix> > Layers;
	vector<Matrix> result;
	
	void calcLayers();
	~VideoFluids()
	{
		delete sfs;
		delete config;
	}
};