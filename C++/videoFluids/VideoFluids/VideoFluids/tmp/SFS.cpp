#include "stdafx.h"
#include "SFS.h"
SFS::SFS(libconfig::Config* config)
{
	CONFIG_SAFE_CALL(config->lookupValue("SFS.Method", method));
	if (method == "RBF")
	{
		CONFIG_SAFE_CALL(config->lookupValue("SFS.RBF.HiddenNodeNum", hiddenNodeNum));
		CONFIG_SAFE_CALL(config->lookupValue("SFS.RBF.EntaCenter", param.entaCenter));
		CONFIG_SAFE_CALL(config->lookupValue("SFS.RBF.EntaSigma", param.entaSigma));
	}
	else if (method == "Tsai")
	{
		CONFIG_SAFE_CALL(config->lookupValue("SFS.Tsai.Sx", param.Sx));
		CONFIG_SAFE_CALL(config->lookupValue("SFS.Tsai.Sy", param.Sy));
		CONFIG_SAFE_CALL(config->lookupValue("SFS.Tsai.Sz", param.Sz));
		CONFIG_SAFE_CALL(config->lookupValue("SFS.Tsai.iteration", param.iteration));
	}
}
void SFS::initialize(Mat* img)
{
	this->img = img;
	this->width = img->cols;
	this->height = img->rows;
	if (method == "RBF")
	{
		p.createMatrix(height,width);
		q.createMatrix(height,width);
		f.createMatrix(height,width);
		g.createMatrix(height,width);
		z.createMatrix(height,width);
		R.createMatrix(height,width);
		PHI.createMatrix(height,width,hiddenNodeNum);
		dRdc1.createMatrix(height,width,hiddenNodeNum);
		dRdc2.createMatrix(height,width,hiddenNodeNum);
		dRdd.createMatrix(height,width,hiddenNodeNum);
		dEtdc.createMatrix(hiddenNodeNum,2);
		dEtdd.createVector(hiddenNodeNum);
		p = q = f = g = z = 0.0;
		w.createVector(hiddenNodeNum);
		w.randomize();
		sigma.createVector(hiddenNodeNum);
		sigma.randomize();
		center.createMatrix(hiddenNodeNum,2);
		center.randomize();
	}
	else if (method == "Tsai")
	{
		Zn.createMatrix(height,width);
		Zn1.createMatrix(height,width);
		Si.createMatrix(height,width);
		Si1.createMatrix(height,width);
	}
}
void SFS::calcTsai()
{
	float p,q,pq,PQs,Eij,fZ,Ps,Qs,dfZ,Y,K,Wn = 1e-8;
	if(param.Sx == 0 && param.Sy == 0) param.Sx = param.Sy = 0.01;
	Ps = param.Sx/param.Sz;
	Qs = param.Sy/param.Sz;
	Zn1 = 0.0;
	Si1 = 1.0;
	for (int iter = 0;iter<param.iteration;iter++)
	{
		for (int i = 0;i<height;i++)
			for (int j = 0;j<width;j++)
			{
				if (j-1<0||i-1<0)
					p = q = 0.0;
				else
				{
					p = Zn1[i][j] - Zn1[i][j-1];
					q = Zn1[i][j] - Zn1[i-1][j];
				}
				pq = 1.0 + p*p + q*q;
				PQs = 1.0 + Ps*Ps + Qs*Qs;
				Eij = this->img->at<unsigned char>(i,j)/255.0;
				fZ = -1.0*(Eij - MAX(0.0,(1+p*Ps+q*Qs)/(sqrt(pq)*sqrt(PQs))));
				dfZ = -1.0*((Ps+Qs)/(sqrt(pq)*sqrt(PQs))-(p+q)*(1.0+p*Ps+q*Qs)/(sqrt(pq*pq*pq)*sqrt(PQs))) ;
				Y = fZ + dfZ*Zn1[i][j];
				K = Si1[i][j]*dfZ/(Wn+dfZ*Si1[i][j]*dfZ);
				Si[i][j] = (1.0 - K*dfZ)*Si1[i][j]; 
				Zn[i][j] = Zn1[i][j] + K*(Y-dfZ*Zn1[i][j]);
			}
		Zn1 = Zn;
		Si1 = Si;
//		for (int i = 0;i<height;i++)
//			for (int j = 0;j<width;j++)
//			{
//				Zn1[i][j] = Zn[i][j];
//				Si1[i][j] = Si[i][j];
//			}
	}
	for (int i = 0;i<height;i++)
		for (int j = 0;j<width;j++)
		{
//			cout<<i<<' '<<j<<endl;
			Zn[i][j] +=300;
		}
//	Zn.destroyMatrix();
	Zn1.destroyMatrix();
	Si.destroyMatrix();
	Si1.destroyMatrix();
}
void SFS::update(Matrix I)
{
	for (int i = 0;i<height;i++)
		for (int j = 0;j<width;j++)
			for (int k = 0;k<hiddenNodeNum;k++)
				PHI[i][j][k] = gaussian(i,j,k);
	for (int i = 0;i<height;i++)
		for(int j = 0;j<width;j++)
			for (int k = 0;k<hiddenNodeNum;k++)
			{
				dRdc1[i][j][k]=w[k]*gaussian(i,j,k)*((p[i][j]-center[k][1])/pow(sigma[k],2));
				dRdc2[i][j][k]=w[k]*gaussian(i,j,k)*((q[i][j]-center[k][2])/pow(sigma[k],2));
				dRdc1[i][j][k]/=(height*width*hiddenNodeNum);
				dRdc2[i][j][k]/=(height*width*hiddenNodeNum);
			}
	float tmp_r;
	float v0 = 0;
	for (int i = 0;i<height;i++)
		for (int j = 0;j<width;j++)
		{
			tmp_r = 0;
			for (int k = 0;k<hiddenNodeNum;k++)
				tmp_r += w[k]*gaussian(i,j,k);
			R[i][j] = tmp_r+v0;
		}
	float temp_E1=0;
    float temp_E2=0;
	for(int k=0;k<hiddenNodeNum;k++)
	{
		 temp_E1=0;
         temp_E2=0;
		 for(int i=0;i<height;i++)
			 for(int j=0;j<width;j++)
			 {
				temp_E1=temp_E1+2*(I[i][j]-R[i][j])*(-dRdc1[i][j][k]);
                temp_E2=temp_E2+2*(I[i][j]-R[i][j])*(-dRdc2[i][j][k]);

			 }
		dEtdc[k][1]=temp_E1/(height*width);
        dEtdc[k][2]=temp_E2/(height*width);
	}
    for (int k=0;k<hiddenNodeNum;k++)
	{
		center[k][1]=center[k][1]-dEtdc[k][1]*param.entaCenter;
        center[k][2]=center[k][2]-dEtdc[k][2]*param.entaCenter;
	}
	for(int i=0;i<height;i++)
		for(int j=0;j<width;j++)
			for(int k=0;k<hiddenNodeNum;k++)
			{
				dRdd[i][j][k]=w[k]*gaussian(i,j,k)*(pow(p[i][j]-center[k][1],2)+pow(q[i][j]-center[k][2],2))/pow(sigma[k],3);
				dRdd[i][j][k]/=(height*width*hiddenNodeNum);
			}
	float temp_E=0;
    for(int k=0;k<hiddenNodeNum;k++)
	{
		 temp_E=0;
        
		 for(int i=0;i<height;i++)
			 for(int j=0;j<width;j++)
			 {
				temp_E=temp_E+2*(I[i][j]-R[i][j])*(-dRdd[i][j][k]);
			 }
		  dEtdd[k]=temp_E/(width*height);   
	
	}
    
	for(int k=0;k<hiddenNodeNum;k++)
	{
		sigma[k]=sigma[k]-param.entaSigma*dEtdd[k];
	}

}