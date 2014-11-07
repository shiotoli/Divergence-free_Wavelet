#include "DFdwt2D.h"

void DFdwt2D::loadData(string u_path,string v_path,string filter0_path,string filter1_path)
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
	filter0.loadFilter(filter0_path);
	filter1.loadFilter(filter1_path);
}
void DFdwt2D::wavedec2d(int level)
{
	double** lastu,**lastv;
	lastu = velocityField.u;
	lastv = velocityField.v;
	int LLsize = DIM;
	for (int i = 0;i<level;i++)
	{
		double** ull,**vll,**uhl,**vhl,**ulh,**vlh,**vhh,**uhh;
		fwt2d_uv(lastu,lastv,ull,uhl,ulh,uhh,vll,vlh,vhl,vhh,LLsize);
		waveletCoeULL.push_back(ull);
		waveletCoeULH.push_back(ulh);
		waveletCoeUHL.push_back(uhl);
		waveletCoeUHH.push_back(uhh);
		waveletCoeVLL.push_back(vll);
		waveletCoeVLH.push_back(vlh);
		waveletCoeVHL.push_back(vhl);
		waveletCoeVHH.push_back(vhh);
		waveletCoeSize.push_back(LLsize/2);
		LLsize/=2;
		lastu = ull;
		lastv = vll;
		char name[100];
		set_field(ulh,vlh,LLsize,LLsize,0.00,0.00);
		set_field(uhl,vhl,LLsize,LLsize,0.00,0.00);
		set_field(uhh,vhh,LLsize,LLsize,0.00,0.00);
		set_field(ull,vll,LLsize,LLsize,0.00,0.00);
		//cout<<calcDivergence_biggest(ull,vll,LLsize,LLsize)<<endl;
			sprintf(name,"ull%04d.txt",i);
			print2d(string(name),ull,LLsize,LLsize);
			sprintf(name,"uhl%04d.txt",i);
			print2d(string(name),uhl,LLsize,LLsize);
			sprintf(name,"ulh%04d.txt",i);
			print2d(string(name),ulh,LLsize,LLsize);
			sprintf(name,"uhh%04d.txt",i);
			print2d(string(name),uhh,LLsize,LLsize);
			sprintf(name,"vll%04d.txt",i);
			print2d(string(name),vll,LLsize,LLsize);
			sprintf(name,"vhl%04d.txt",i);
			print2d(string(name),vhl,LLsize,LLsize);
			sprintf(name,"vlh%04d.txt",i);
			print2d(string(name),vlh,LLsize,LLsize);
			sprintf(name,"vhh%04d.txt",i);
			print2d(string(name),vhh,LLsize,LLsize);
		if (i==level-1)
		{
			ull[0][0] = vll[0][0]=1;;
			//uhh[0][0] = 0.3;
			//vhh[0][0] = 0.0;
			//mul(uhh,vhh,LLsize,LLsize,1);
			//mul(uhl,vhl,LLsize,LLsize,1);
			//mul(ulh,vlh,LLsize,LLsize,1);
			//uhh[3][3] = 0.3;
			//vhh[0][0] = 0.0;
			//uhh[2][2] = 5;
			//set_field_rand(uhl,vhl,LLsize,LLsize);
			//set_field(ulh,vlh,LLsize,LLsize,0.05,0.00);
			//set_field(uhl,vhl,LLsize,LLsize,0.06,0.00);
		}
	}
}
void DFdwt2D::waverec2d(int level)
{
	double** lastu=waveletCoeULL[level],**lastv=waveletCoeVLL[level],**nextu,**nextv;
	char name[100];
	for (int i = level;i>=0;i--)
	{
		ifwt2d_uv(lastu,waveletCoeUHL[i],waveletCoeULH[i],waveletCoeUHH[i],lastv,waveletCoeVLH[i],waveletCoeVHL[i],waveletCoeVHH[i],nextu,nextv,waveletCoeSize[i]);
		recWaveletCoeULL.push_back(nextu);
		recWaveletCoeVLL.push_back(nextv);
		lastu = nextu;
		lastv = nextv;
		recWaveletCoeSize.push_back(waveletCoeSize[i]*2);
		sprintf(name,"nextu%04d.txt",i);
		print2d(string(name),nextu,waveletCoeSize[i]*2,waveletCoeSize[i]*2);
		sprintf(name,"nextv%04d.txt",i);
		print2d(string(name),nextv,waveletCoeSize[i]*2,waveletCoeSize[i]*2);
	}
	//return ;
	cout<<calcDivergence_sum(lastu,lastv,DIM,DIM)<<' '<<calcDivergence_biggest(lastu,lastv,DIM,DIM)<<endl;
	print2d("..\\..\\..\\mathematica\\lastu.txt",lastu,waveletCoeSize[0]*2,waveletCoeSize[0]*2);
	//set_field(lastu,lastv,waveletCoeSize[0]*2,waveletCoeSize[0]*2,1,0);
	print2d("..\\..\\..\\mathematica\\lastv.txt",lastv,waveletCoeSize[0]*2,waveletCoeSize[0]*2);
	system("pause");
		
	//print2d("realu.txt",waveletCoeULL[level-1],waveletCoeSize[level]*2,waveletCoeSize[level]*2);
	//print2d("realv.txt",waveletCoeVLL[level-1],waveletCoeSize[level]*2,waveletCoeSize[level]*2);
}
void DFdwt2D::fwt2d_uv(double** u,double** v,double** &ull,double** &uhl,double** &ulh,double** &uhh,double** &vll,double** &vlh,double** &vhl,double** &vhh,int LLsize)
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
// 	print2d("t1.txt",vlltmp,LLsize/2,LLsize/2);
// 	print2d("t2.txt",vlhtmp,LLsize/2,LLsize/2);
// 	print2d("t3.txt",vhltmp,LLsize/2,LLsize/2);
// 	print2d("t4.txt",vhhtmp,LLsize/2,LLsize/2);
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
				vlh[i][j] = ulhtmp[i][j]+0.25*vlhtmp[i][j]-0.25*(scene == LOOP?vlhtmp[LLsize/2-1][j]:0);
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
void DFdwt2D::ifwt2d_uv(double** ull,double** uhl,double** ulh,double** uhh,double** vll,double** vlh,double** vhl,double** vhh,double** &u,double** &v,int LLsize)
{
	double** ulltmp,**ulhtmp,**uhltmp,**uhhtmp,**vlltmp,**vhltmp,**vlhtmp,**vhhtmp;
	new2D(ulltmp,LLsize,LLsize);
	new2D(vlltmp,LLsize,LLsize);
	new2D(uhltmp,LLsize,LLsize);
	new2D(vhltmp,LLsize,LLsize);
	new2D(ulhtmp,LLsize,LLsize);
	new2D(vlhtmp,LLsize,LLsize);
	new2D(uhhtmp,LLsize,LLsize);
	new2D(vhhtmp,LLsize,LLsize);
	new2D(u,LLsize*2,LLsize*2);
	new2D(v,LLsize*2,LLsize*2);
	for (int i = 0;i<LLsize;i++)
		for (int j = 0;j<LLsize;j++)
		{
			ulltmp[i][j] = ull[i][j];
			vlltmp[i][j] = vll[i][j];
			//Psi(0,1)
			uhltmp[i][j] = uhl[i][j];
			//Psi(1,0)
			vlhtmp[i][j] = ulh[i][j];
			//Psi(1,1)
			uhhtmp[i][j] = uhh[i][j]+vhh[i][j];
			vhhtmp[i][j] = vhh[i][j]-uhh[i][j];
		}
	for (int i = 0;i<LLsize;i++)
		for (int j = 0;j<LLsize;j++)
		{
			if (j != 0)
				vhltmp[i][j] = vhl[i][j]-0.25*uhltmp[i][j]+0.25*uhltmp[i][j-1];
			else
				vhltmp[i][j] = vhl[i][j]-0.25*uhltmp[i][j]+0.25*(scene == LOOP?uhltmp[i][LLsize-1]:0);
			if (i!=0)
				ulhtmp[i][j] = vlh[i][j]-0.25*vlhtmp[i][j]+0.25*vlhtmp[i-1][j];
			else
				ulhtmp[i][j] = vlh[i][j]-0.25*vlhtmp[i][j]+0.25*(scene == LOOP?vlhtmp[LLsize-1][j]:0);

		}
// 		print2d("s1.txt",vlltmp,LLsize,LLsize);
// 		print2d("s2.txt",vlhtmp,LLsize,LLsize);
// 		print2d("s3.txt",vhltmp,LLsize,LLsize);
// 		print2d("s4.txt",vhhtmp,LLsize,LLsize);
	ifwt2d(ulltmp,uhltmp,ulhtmp,uhhtmp,filter1.lor,filter0.lor,filter1.hir,filter0.hir,u,LLsize);
	ifwt2d(vlltmp,vhltmp,vlhtmp,vhhtmp,filter0.lor,filter1.lor,filter0.hir,filter1.hir,v,LLsize);
		release2D(ulltmp,LLsize);
		release2D(vlltmp,LLsize);
		release2D(uhltmp,LLsize);
		release2D(vhltmp,LLsize);
		release2D(ulhtmp,LLsize);
		release2D(vlhtmp,LLsize);
		release2D(uhhtmp,LLsize);
		release2D(vhhtmp,LLsize);
}
void DFdwt2D::fwt2d(double** LL,double** &ll,double** &hl,double** &lh,double** &hh,SubFilter lodx,SubFilter lody,SubFilter hidx,SubFilter hidy,int LLsize)
{
	double** tmpLow,**tmpHigh;
	print2d("LL2.txt",LL,LLsize,LLsize);
	conv_down2d(LL,tmpLow,0,lodx,LLsize,LLsize);
	conv_down2d(tmpLow,ll,1,lody,LLsize/2,LLsize);
	conv_down2d(tmpLow,hl,1,hidy,LLsize/2,LLsize);
	conv_down2d(LL,tmpHigh,0,hidx,LLsize,LLsize);
	conv_down2d(tmpHigh,lh,1,lody,LLsize/2,LLsize);
	conv_down2d(tmpHigh,hh,1,hidy,LLsize/2,LLsize);
	print2d("low.txt",tmpLow,LLsize,LLsize/2);
	//print2d("ll.txt",ll,LLsize/2,LLsize/2);
	//print2d("hl.txt",hl,LLsize/2,LLsize/2);
	print2d("high.txt",tmpHigh,LLsize,LLsize/2);
	release2D(tmpLow,LLsize);
	release2D(tmpHigh,LLsize);
}
void DFdwt2D::ifwt2d(double** ll,double** hl,double** lh,double** hh,SubFilter lorx,SubFilter lory,SubFilter hirx,SubFilter hiry, double** &LL,int LLsize)
{
	double** tmpll,**tmphl,**tmplh,**tmphh,**HH;
	conv_up2d(ll,tmpll,1,lory,LLsize,LLsize);
	conv_up2d(hl,tmphl,1,hiry,LLsize,LLsize);
	conv_up2d(lh,tmplh,1,lory,LLsize,LLsize);
	conv_up2d(hh,tmphh,1,hiry,LLsize,LLsize);
	for (int i = 0;i<LLsize*2;i++)
		for (int j = 0;j<LLsize;j++)
		{
			tmpll[i][j] +=tmphl[i][j];
			tmplh[i][j] += tmphh[i][j];
		}
		//shift(tmplh,1,LLsize,LLsize*2,1);
		//shift(tmpll,1,LLsize,LLsize*2,1);
		//print2d("low1.txt",tmpll,LLsize*2,LLsize);
		//print2d("high1.txt",tmplh,LLsize*2,LLsize);
	conv_up2d(tmpll,LL,0,lorx,LLsize,LLsize*2);
	conv_up2d(tmplh,HH,0,hirx,LLsize,LLsize*2);
	for (int i = 0;i<LLsize*2;i++)
		for (int j = 0;j<LLsize*2;j++)
		{
			LL[i][j]+=HH[i][j];
		}
		//shift(LL,0,LLsize*2,LLsize*2,(lorx.filterLen-1));
		//print2d("LL1.txt",LL,LLsize*2,LLsize*2);
	release2D(HH,LLsize*2);
	release2D(tmpll,LLsize*2);
	release2D(tmphl,LLsize*2);
	release2D(tmplh,LLsize*2);
	release2D(tmphh,LLsize*2);
}
void DFdwt2D::conv_down2d(double** input,double** &output,int dir,SubFilter filter,int input_xsize,int input_ysize)
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
#ifdef DUAL_CONVOLUTION
					int input_id = j*2+1+/*(filterSize-1)+*/k+filter.startid;
#else
					int input_id = j*2+1-/*(filterSize-1)+*/k-filter.startid;
#endif
					if (input_id<0)
					{
						if (scene == LOOP)
							input_id =(input_id+10*input_xsize)%input_xsize;
						else if (scene == BOUNDARY)
							continue;
					}
					else if (input_id>=input_xsize)
					{
						if (scene == LOOP)
							input_id %= input_xsize;
						else if (scene == BOUNDARY)
							continue;
					}
					outtmp[i][j] += filter.filter[/*(filterSize-1)-*/k]*input[i][input_id];
				}
			}
		}
		output = outtmp;
	}
	else
	{
		double** outtmp = new double*[input_ysize/2];
		for (int i = 0;i<input_ysize/2;i++)
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
#ifdef DUAL_CONVOLUTION
					int input_id = j*2+1+/*(filterSize-1)+*/k+filter.startid;
#else
					int input_id = j*2+1-/*(filterSize-1)+*/k-filter.startid;
#endif
					if (input_id<0)
					{
						if (scene == LOOP)
							input_id = (input_id+10*input_ysize)%input_ysize;
						else if (scene == BOUNDARY)
							continue;
					}
					else if (input_id>=input_ysize)
					{
						if (scene == LOOP)
							input_id %= input_ysize;
						else if (scene == BOUNDARY)
							continue;
					}
					outtmp[j][i] += filter.filter[/*(filterSize-1)-*/k]*input[input_id][i];
				}
			}
		}
		output = outtmp;
	}
}
void DFdwt2D::shift(double** input,int dir,int input_xsize,int input_ysize,int offset)
{
	double tmp[DIM];
	if (dir == 0)
	{
		for (int i = 0;i<input_ysize;i++)
		{
			for (int j = 0;j<input_xsize;j++)
				tmp[(j-offset+input_xsize*10)%input_xsize] = input[i][j];
			for (int j = 0;j<input_xsize;j++)
				input[i][j] = tmp[j];
		}
	}
	else if (dir == 1)
	{
		for (int i = 0;i<input_xsize;i++)
		{
			for (int j = 0;j<input_ysize;j++)
				tmp[(j-offset+input_ysize*10)%input_ysize] = input[j][i];
			for (int j = 0;j<input_ysize;j++)
				input[j][i] = tmp[j];
		}

	}
}
void DFdwt2D::conv_up2d(double** input,double** &output,int dir,SubFilter filter,int input_xsize,int input_ysize)
{
	int filterSize = filter.filterLen;
	if (dir == 0)
	{
		double** outtmp = new double*[input_ysize];
		for (int i = 0;i<input_ysize;i++)
			outtmp[i] = new double[input_xsize*2];
		for (int i = 0;i<input_ysize;i++)
			for (int j = 0;j<input_xsize*2;j++)
				outtmp[i][j] = 0;
		for (int i = 0;i<input_ysize;i++)
		{
			for (int j = 0;j<input_xsize*2;j++)
			{
				for (int k = 0;k<filter.filterLen;k++)
				{
#ifdef DUAL_CONVOLUTION
					int input_id = j-/*(filterSize-1)+*/k-filter.startid;
#else
					int input_id = j+/*(filterSize-1)+*/k+filter.startid;
#endif
					if (input_id<0)
					{
						if (scene == LOOP)
							input_id =(input_id+input_xsize*20)%(input_xsize*2);
						else if (scene == BOUNDARY)
							continue;
					}
					else if (input_id>=input_xsize*2)
					{
						if (scene == LOOP)
							input_id %= input_xsize*2;
						else if (scene == BOUNDARY)
							continue;
					}
					if (input_id%2==0)
						continue;
					input_id/=2;
					outtmp[i][j] += filter.filter[/*(filterSize-1)-*/k]*input[i][input_id];
				}
			}
		}
		output = outtmp;
	}
	else
	{
		double** outtmp = new double*[input_ysize*2];
		for (int i = 0;i<input_ysize*2;i++)
			outtmp[i] = new double[input_xsize];
		for (int i = 0;i<input_ysize*2;i++)
			for (int j = 0;j<input_xsize;j++)
				outtmp[i][j] = 0;
		for (int i = 0;i<input_xsize;i++)
		{
			for (int j = 0;j<input_ysize*2;j++)
			{
				for (int k = 0;k<filter.filterLen;k++)
				{
#ifdef DUAL_CONVOLUTION
					int input_id = j-/*(filterSize-1)+*/k-filter.startid;
#else
					int input_id = j+/*(filterSize-1)+*/k+filter.startid;
#endif
					if (input_id<0)
					{
						if (scene == LOOP)
							input_id =(input_id+input_ysize*20)%(input_ysize*2);
						else if (scene == BOUNDARY)
							continue;
					}
					else if (input_id>=input_ysize*2)
					{
						if (scene == LOOP)
							input_id %= input_ysize*2;
						else if (scene == BOUNDARY)
							continue;
					}
					if (input_id%2==0)
						continue;
					input_id/=2;
					outtmp[j][i] += filter.filter[/*(filterSize-1)-*/k]*input[input_id][i];
				}
			}
		}
		output = outtmp;
	}
}