#include "DFdwt2D.h"

void DFdwt2D::loadData(string u_path,string v_path,string filterx_path,string filtery_path)
{
	FILE* fp = fopen(u_path.c_str(),"r");
	for (int i = 0;i<DIM;i++)
		for (int j = 0;j<DIM;j++)
			fscanf(fp,"%lf",&velocityField.u[i][j]);
	fclose(fp);
	fp = fopen(v_path.c_str(),"r");
	for (int i = 0;i<DIM;i++)
		for (int j = 0;j<DIM;j++)
			fscanf(fp,"%lf",&velocityField.v[i][j]);
	fclose(fp);
	filter0.loadFilter(filterx_path);
	filter1.loadFilter(filtery_path);
}

void DFdwt2D::fwt2d_uv(double** u,double** v,double** ull,double** uhl,double** ulh,double** uhh,double** vll,double** vlh,double** vhl,double** vhh,int LLsize)
{
	double** ulltmp,**ulhtmp,**uhltmp,**uhhtmp,**vlltmp,**vhltmp,**vlhtmp,**vhhtmp;
	fwt2d(u,ulltmp,uhltmp,ulhtmp,uhhtmp,filter1.lod,filter0.lod,filter1.hid,filter0.hid,LLsize);
	fwt2d(v,vlltmp,vhltmp,vlhtmp,vhhtmp,filter0.lod,filter1.lod,filter0.hid,filter1.hid,LLsize);
	new2D(ull,LLsize/2,LLsize/2);
	new2D(vll,LLsize/2,LLsize/2);
	new2D(uhl,LLsize/2,LLsize/2);
	new2D(vhl,LLsize/2,LLsize/2);
	new2D(ulh,LLsize/2,LLsize/2);
	new2D(vlh,LLsize/2,LLsize/2);
	new2D(uhh,LLsize/2,LLsize/2);
	new2D(vhh,LLsize/2,LLsize/2);
	for (int i = 0;i<LLsize/2;i++)
		for (int j = 0;j<LLsize/2;j++)
		{
			ull[i][j] = ulltmp[i][j];
			vll[i][j] = vlltmp[i][j];
			//Psi(0,1)
			uhl[i][j] = uhltmp[i][j];
			if (j != 0)
				vhl[i][j] = vhltmp[i][j]+0.25*uhltmp[i][j]-0.25*uhltmp[i][j-1];
			else
				vhl[i][j] = vhltmp[i][j]+0.25*uhltmp[i][j]-0.25*(scene == LOOP?uhltmp[i][LLsize/2-1]:0);
			//Psi(1,0)
			ulh[i][j] = vlhtmp[i][j];
			if (i!=0)
				vlh[i][j] = ulhtmp[i][j]+0.25*vlhtmp[i][j]-0.25*vlhtmp[i-1][j];
			else
				vlh[i][j] = ulhtmp[i][j]+0.25*vlhtmp[i][j]-0.25*(scene == LOOP?uhltmp[LLsize/2-1][j]:0);
			//Psi(1,1)
			uhh[i][j] = 0.5*uhhtmp[i][j]-0.5*vhhtmp[i][j];
			vhh[i][j] = 0.5*uhhtmp[i][j]+0.5*vhhtmp[i][j];
		}
		release2D(ulltmp,LLsize/2);
		release2D(vlltmp,LLsize/2);
		release2D(uhltmp,LLsize/2);
		release2D(vhltmp,LLsize/2);
		release2D(ulhtmp,LLsize/2);
		release2D(vlhtmp,LLsize/2);
		release2D(uhhtmp,LLsize/2);
		release2D(vhhtmp,LLsize/2);
}
void DFdwt2D::ifwt2d_uv(double** ull,double** uhl,double** ulh,double** uhh,double** vll,double** vlh,double** vhl,double** vhh,double** u,double** v,int LLsize)
{
	double** ulltmp,**ulhtmp,**uhltmp,**uhhtmp,**vlltmp,**vhltmp,**vlhtmp,**vhhtmp;
	new2D(ull,LLsize,LLsize);
	new2D(vll,LLsize,LLsize);
	new2D(uhl,LLsize,LLsize);
	new2D(vhl,LLsize,LLsize);
	new2D(ulh,LLsize,LLsize);
	new2D(vlh,LLsize,LLsize);
	new2D(uhh,LLsize,LLsize);
	new2D(vhh,LLsize,LLsize);
	for (int i = 0;i<LLsize;i++)
		for (int j = 0;j<LLsize;j++)
		{
			ulltmp[i][j] = ull[i][j];
			vlltmp[i][j] = vll[i][j];
			//Psi(0,1)
			uhltmp[i][j] = uhl[i][j];
			if (j != 0)
				vhl[i][j] = vhltmp[i][j]+0.25*uhl[i][j]-0.25*uhl[i][j-1];
			else
				vhl[i][j] = vhltmp[i][j]+0.25*uhl[i][j]-0.25*(scene == LOOP?uhl[i][LLsize/2-1]:0);
			//Psi(1,0)
			ulhtmp[i][j] = vlh[i][j];
			if (i!=0)
				vlh[i][j] = ulhtmp[i][j]+0.25*vlhtmp[i][j]-0.25*vlhtmp[i-1][j];
			else
				vlh[i][j] = ulhtmp[i][j]+0.25*vlhtmp[i][j]-0.25*(scene == LOOP?uhl[LLsize/2-1][j]:0);
			//Psi(1,1)
			uhhtmp[i][j] = uhh[i][j]+vhh[i][j];
			vhhtmp[i][j] = vhh[i][j]-uhh[i][j];
		}
	fwt2d(u,ulltmp,uhltmp,ulhtmp,uhhtmp,filter1.lod,filter0.lod,filter1.hid,filter0.hid,LLsize);
	fwt2d(v,vlltmp,vhltmp,vlhtmp,vhhtmp,filter0.lod,filter1.lod,filter0.hid,filter1.hid,LLsize);
		release2D(ulltmp,LLsize/2);
		release2D(vlltmp,LLsize/2);
		release2D(uhltmp,LLsize/2);
		release2D(vhltmp,LLsize/2);
		release2D(ulhtmp,LLsize/2);
		release2D(vlhtmp,LLsize/2);
		release2D(uhhtmp,LLsize/2);
		release2D(vhhtmp,LLsize/2);
}
void DFdwt2D::fwt2d(double** LL,double** &ll,double** &hl,double** &lh,double** &hh,SubFilter lodx,SubFilter lody,SubFilter hidx,SubFilter hidy,int LLsize)
{
	double** tmpLow,**tmpHigh;
	conv2d(LL,tmpLow,0,lodx,LLsize,LLsize);
	conv2d(tmpLow,ll,1,lody,LLsize/2,LLsize);
	conv2d(tmpLow,hl,1,hidy,LLsize/2,LLsize);
	conv2d(LL,tmpHigh,0,hidx,LLsize,LLsize);
	conv2d(tmpHigh,lh,1,lody,LLsize/2,LLsize);
	conv2d(tmpHigh,hh,1,hidy,LLsize/2,LLsize);
	release2D(tmpLow,LLsize);
	release2D(tmpHigh,LLsize);
}
void DFdwt2D::conv2d(double** input,double** &output,int dir,SubFilter filter,int input_xsize,int input_ysize)
{
	int filterSize = filter.filterLen;
	if (dir == 0)
	{
		double** outtmp = new double*[input_ysize];
		for (int i = 0;i<input_ysize;i++)
			outtmp[i] = new double[input_xsize/2];
		for (int i = 0;i<input_ysize;i++)
			for (int j = 0;j<input_xsize/2;j++)
				outtmp[i][j] = 0;
		for (int i = 0;i<input_ysize;i++)
		{
			for (int j = 0;j<input_xsize/2;j++)
			{
				for (int k = 0;k<filter.filterLen;k++)
				{
					int input_id = j*2+1-filter.startid-k;
					if (input_id<0)
					{
						if (scene == LOOP)
							input_id +=input_xsize;
						else if (scene == BOUNDARY)
							continue;
					}
					else if (input_id>=input_xsize)
					{
						if (scene == LOOP)
							input_id -= input_xsize;
						else if (scene == BOUNDARY)
							continue;
					}
					outtmp[i][j] += filter.filter[filter.startid+k]*input[i][input_id];
				}
			}
		}
		output = outtmp;
	}
	else
	{
		double** outtmp = new double*[input_ysize/2];
		for (int i = 0;i<input_ysize;i++)
			outtmp[i] = new double[input_xsize];
		for (int i = 0;i<input_ysize/2;i++)
			for (int j = 0;j<input_xsize;j++)
				outtmp[i][j] = 0;
		for (int i = 0;i<input_xsize;i++)
		{
			for (int j = 0;j<input_ysize/2;j++)
			{
				for (int k = 0;k<filter.filterLen;k++)
				{
					int input_id = j*2+1-filter.startid-k;
					if (input_id<0)
					{
						if (scene == LOOP)
							input_id +=input_ysize;
						else if (scene == BOUNDARY)
							continue;
					}
					else if (input_id>=input_ysize)
					{
						if (scene == LOOP)
							input_id -= input_ysize;
						else if (scene == BOUNDARY)
							continue;
					}
					outtmp[j][i] += filter.filter[filter.startid+k]*input[input_id][i];
				}
			}
		}
		output = outtmp;
	}
}