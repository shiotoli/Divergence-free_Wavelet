#pragma once
#include "stdafx.h"
#include "SFS.h"
class VideoFluids
{
public:
	VideoFluids()
	{
		config = new libconfig::Config();
		config->readFile("..\\VideoFluids.cfg");
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
	void addPic(Mat m);
	void updateField(Matrix& Zn,Matrix& U,Matrix& V);
	void trackVelocity(Matrix& Zn1,Matrix& Zn,Matrix& U,Matrix& V);
	void calcFluid();
	~VideoFluids()
	{
		delete sfs;
		delete config;
	}
};