#include "stdafx.h"
#include "Matrix.h"
struct SFSParam
{
	float entaCenter;
	float entaSigma;
	float Sx,Sy,Sz;
	int	iteration;
};
class SFS
{
public:
	SFSParam param;
	SFS(libconfig::Config* config);
	Mat* img;
	string method;
	int hiddenNodeNum;
	libconfig::Config* config;
	int width,height;
	void initialize(Mat* img);
	void RBFsolve();
//	void Tsaisolve();
	void calcTsai();
	Matrix Zn;
private:
	void update(Matrix I);
	inline float gaussian(int i, int j, int k)
	{
		float temp=exp(-(pow((p[i][j]-center[k][1]),2)+pow((q[i][j]-center[k][2]),2))/(2*pow(sigma[k],2)));
		return temp;
	}
	Matrix p,q,z,f,g;
	Matrix R;
	Matrix dEtdc;
	Matrix3 PHI,dRdc1,dRdc2,dRdd;
	//Gaussian Kernel Parameter;
	Matrix center;
	Vect w,sigma,dEtdd;
	Matrix Zn1,Si,Si1;
};