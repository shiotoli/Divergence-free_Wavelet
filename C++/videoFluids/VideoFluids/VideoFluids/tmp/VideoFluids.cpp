#include "VideoFluids.h"
void VideoFluids::processSFS()
{
	sfs_list.clear();
	for (int i = 0;i<img_list.size();i++)
	{
		sfs_list.push_back(Matrix(height,width));
		sfs->initialize(&img_list[i]);
		sfs->calcTsai();
		sfs_list[i] = sfs->Zn;
		FILE* fp;
		fp = fopen("test.txt","w");
		//For jyt:
		//show sfs_list[i] as picture.
		/*
		cv::Mat showH(height,width,CV_8UC1);
		int low=10000000,high=-1000000;
		for(int j=0;j<height;j++){
			for(int k=0;k<width;k++){
				if(sfs_list[i][j][k]>high)
					high=sfs_list[i][j][k];
				if(sfs_list[i][j][k]<low)
					low=sfs_list[i][j][k];
			}
		}
		for(int j=0;j<height;j++){
			for(int k=0;k<width;k++){
				//printf("%d %d\n",j,k);
				//if(sfs_list[i][j][k]<low)
				//	showH.at<uchar>(j,k)=0;
				//else
					showH.at<uchar>(j,k)=((float)(sfs_list[i][j][k]-low))/(high-low)*255;
			}
		}*/
		//cv::imwrite("testH.bmp",showH);
		printf("%d is over",i);
	}
}
void VideoFluids::calcFluid()
{
	Matrix U(height,width),V(height,width);
	for (int i = 1;i<img_list.size();i++)
	{
		trackVelocity(sfs_list[i-1],sfs_list[i],U,V);
		system("pause");
	}

}
void VideoFluids::addPic(Mat m) 
{
	if (height == -1)
		setSize(m.rows,m.cols);
	if (height!=m.rows||width!=m.cols)
		cv::resize(m,m,cv::Size(width,height));
	img_list.push_back(m);
}
void VideoFluids::filter()
{
	int len=2;
	float sigma=3;
	float a[3]={exp(-1*1/2.0/sigma/sigma),exp(-2*2/2.0/sigma/sigma),exp(-3*3/2.0/sigma/sigma)};
	//Gaussian process
	for (int i = 0;i<sfs_list.size();i++)
	{
		for(int j=len;j<height-len;j++){
			for(int k=0;k<width;k++){
				float res=sfs_list[i][j][k];
				for(int m=1;m<=len;m++)
					res += (sfs_list[i][j-len][k]+sfs_list[i][j+len][k])*a[len-1];
				sfs_list[i][j][k] = res/(2*(a[0]+a[1])+1);
			}
		}
		for(int j=0;j<height;j++){
			for(int k=len;k<width-len;k++){
				float res=sfs_list[i][j][k];
				for(int m=1;m<=len;m++)
					res += (sfs_list[i][j][k-len]+sfs_list[i][j][k+len])*a[len-1];
				sfs_list[i][j][k] = res/(2*(a[0]+a[1])+1);
			}
		}
	}
	float totalAve = 0;
	vector<float> localAve;
	localAve.clear();
	for (int i = 0;i<sfs_list.size();i++)
	{
		localAve.push_back(0);
		for (int j = 0;j<sfs_list[i].n;j++)
			for (int k = 0;k<sfs_list[i].m;k++)
				localAve[i]+=sfs_list[i][j][k];
		totalAve+=localAve[i];
		localAve[i]/=width*height;
	}
	totalAve/=width*height*sfs_list.size();
	for (int i = 0;i<sfs_list.size();i++)
	{
		for (int j = 0;j<sfs_list[i].n;j++)
			for (int k = 0;k<sfs_list[i].m;k++)
				sfs_list[i][j][k]-=localAve[i]-totalAve;

		//output as obj files:
		char t[256];
		sprintf(t,"%d",i);
		string s=t;
		s="objfile_afterfilter"+s+".obj";
		FILE* fwrite=fopen(s.c_str(),"w+");
		for(int j=0;j<height;j++){
			for(int k=0;k<width;k++){
				fprintf(fwrite,"v %d %d %f\n",k,height-j,sfs_list[i][j][k]);
			}
		}
		for(int j=1;j<height;j++){
			for(int k=1;k<width;k++){
				fprintf(fwrite,"f %d %d %d\n", (j-1)*width+k,(j-1)*width+k+1,j*width+k);
				fprintf(fwrite,"f %d %d %d\n", j*width+k,(j-1)*width+k+1,j*width+k+1);
			}
		}
		fclose(fwrite);
		printf("finish one frame");
	}	
	sfs_list[0].output("test.txt");
}
void VideoFluids::trackVelocity(Matrix& Zn1,Matrix& Zn,Matrix& U,Matrix& V)
{
	Matrix Zx(height,width),Zy(height,width),ZZx(height,width),ZZy(height,width),Zt(height,width),ZZt(height,width),ZZtx(height,width),ZZty(height,width);
	Matrix Au1(height,width),Au2(height,width),Av1(height,width),Av2(height,width);
	Matrix Z2x(height,width),Z2y(height,width),Z2(height,width);
	Matrix Cu(height,width),Cv(height,width);
	Matrix tmp(height,width),tmp1(height,width);
	Matrix U_old(height,width),V_old(height,width),Ux(height,width),Uy(height,width),Vx(height,width),Vy(height,width),Uax(height,width),Uay(height,width),Vax(height,width),Vay(height,width),Uxy(height,width),Vxy(height,width);
	Matrix Coe(height,width);

	Zt = Zn;
	Zt -= Zn1;
	DotMul(Zn,Zt,ZZt);
	Zn.output("Zn.txt");
	Zn1.output("Zn1.txt");
	Zt.output("Zt.txt");
	Partial(ZZt,ZZtx,AXIS_X);
	Partial(ZZt,ZZty,AXIS_Y);
	Partial(Zn,Zx,AXIS_X);
	Partial(Zn,Zy,AXIS_Y);
	DotMul(Zn,Zx,ZZx);
	DotMul(Zn,Zy,ZZy);
	DotMul(Zx,Zx,Au1);
	Partial(ZZx,tmp,AXIS_X);
	Au1-=tmp;
	DotMul(Zn,Zn,tmp);
	Au1+=tmp;
	Au1+=2*alpha*alpha;
	DotMul(Zx,Zy,Au2);
	Partial(ZZy,tmp,AXIS_X);
	Au2-=tmp;
	DotMul(Zx,Zy,Av1);
	Partial(ZZx,tmp,AXIS_Y);
	Av1-=tmp;
	DotMul(Zy,Zy,Av2);
	Partial(ZZy,tmp,AXIS_Y);
	Av2-=tmp;
	DotMul(Zn,Zn,tmp);
	Av2+=tmp;
	Av2+=2*alpha*alpha;
	DotMul(Zn,Zn,Z2);
	Partial(Z2,Z2x,AXIS_X);
	Partial(Z2,Z2y,AXIS_Y);
	for (int i = 0;i<height;i++)
		for (int j = 0;j<width;j++)
			Coe[i][j] = 1.0/(Au1[i][j]*Av2[i][j]-Au2[i][j]*Av1[i][j]);

	U = 0.0;
	V = 0.0;
	for (int iter_time = 0;iter_time<iterationTime;iter_time++)
	{
		V_old = V;
		U_old = U;
		Partial(U,Ux,AXIS_X);
		Partial(U,Uy,AXIS_Y);
		Partial(V,Vx,AXIS_X);
		Partial(V,Vy,AXIS_Y);
		Partial(Vx,Vxy,AXIS_Y);
		Partial(Ux,Uxy,AXIS_Y);
		Average(U,Uax,AXIS_X);
		Average(U,Uay,AXIS_Y);
		Average(V,Vax,AXIS_X);
		Average(V,Vay,AXIS_Y);
		DotMul(Z2x,Ux,Cu);
		DotMul(ZZy,Vx,tmp);
		Cu += tmp;
		tmp = ZZx*-1;
		tmp+=Z2x;
		DotMul(tmp,Vy,tmp1);
		Cu+=tmp1;
		tmp = Z2;
		tmp+=alpha*alpha;
		DotMul(tmp,Uax,tmp1);
		Cu+=tmp1;
		tmp1=Uay;
		tmp1*=alpha*alpha;
		Cu+=tmp1;
		DotMul(Z2,Vxy,tmp1);
		Cu+=tmp1;
		DotMul(Zx,Zt,tmp);
		Cu-=tmp;
		Cu+=ZZtx;


		DotMul(Z2y,Vy,Cv);
		DotMul(ZZx,Uy,tmp);
		Cv += tmp;
		tmp = ZZy;
		tmp*=-1;
		tmp+=Z2y;
		DotMul(tmp,Ux,tmp1);
		Cv+=tmp1;
		tmp = Z2;
		tmp+=alpha*alpha;
		DotMul(tmp,Vay,tmp1);
		Cv+=tmp1;
		tmp1=Vax;
		tmp1*=alpha*alpha;
		Cv+=tmp1;
		DotMul(Z2,Uxy,tmp1);
		Cv+=tmp1;
		DotMul(Zy,Zt,tmp);
		Cv-=tmp;
		Cv+=ZZty;
		for (int i = 0;i<height;i++)
			for (int j = 0;j<width;j++)
			{
				U[i][j] = Coe[i][j]*(Av2[i][j]*Cu[i][j]-Au2[i][j]*Cv[i][j]);
				V[i][j] = Coe[i][j]*(-Av1[i][j]*Cu[i][j]+Au1[i][j]*Cv[i][j]);
			}	
		for (int i = 0;i<height;i++)
		{
			U[i][0] = U[i][1];
			U[i][width-1] = U[i][width-2];
			V[i][0] = V[i][1];
			V[i][width-1] =V[i][width-2];
		}
		for (int i = 0;i<width;i++)
		{
			U[0][i] = U[1][i];
			U[height-1][i] = U[height-2][i];
			V[0][i] = V[1][i];
			V[height-1][i] =V[height-2][i];
		}
		FILE* fp;
// 		Au1.output("Au1.txt");
// 		Au2.output("Au2.txt");
// 		Av1.output("Av1.txt");
// 		Av2.output("Av2.txt");
// 		Cu.output("Cu.txt");
// 		Cv.output("Cv.txt");
		float d1 = Difference(U,U_old);
		float d2 = Difference(V,V_old);
// 		U.output("U.txt");
// 		U_old.output("U_old.txt");
// 		V.output("V.txt");
		cout<<d1<<' '<<d2<<endl;
		if (d1<iterationTorlerance && d2<iterationTorlerance)
			break;
	}
	U.output("U.txt");
	
		cv::Mat showV(height,width,CV_8UC3);
		float lowv=10000000,lowu=10000000,highu=-10000000,highv=-1000000;
		for(int j=0;j<height;j++){
			for(int k=0;k<width;k++){
				if(U[j][k]>highu)
					highu=U[j][k];
				if(U[j][k]<lowu)
					lowu=U[j][k];
				if(V[j][k]>highv)
					highv=V[j][k];
				if(V[j][k]<lowv)
					lowv=V[j][k];
			}
		}
		for(int j=0;j<height;j++){
			for(int k=0;k<width;k++){
				//printf("%d %d\n",j,k);
				//if(sfs_list[i][j][k]<low)
				//	showH.at<uchar>(j,k)=0;
				//else
				float u=(U[j][k]-lowu)/(highu-lowu);
				float v=(V[j][k]-lowv)/(highv-lowv);
				if(u>0.5)
					showV.at<cv::Vec3b>(j,k)[2]=255;
				else
					showV.at<cv::Vec3b>(j,k)[2]=255*u;
				if(v>0.5){
					showV.at<cv::Vec3b>(j,k)[0]=255;
					showV.at<cv::Vec3b>(j,k)[1]=255*(1-v);
				}
				else{
					showV.at<cv::Vec3b>(j,k)[1]=255;
					showV.at<cv::Vec3b>(j,k)[0]=255*v;
				}
			}
		}
		cv::imwrite("testV.bmp",showV);
		printf("show you");
		

}
void VideoFluids::updateField(Matrix& Zn,Matrix& U,Matrix& V)
{
	Matrix tmp(height,width),tmp1(height,width);
	Matrix result(height,width);
	Matrix Ux(height,width),Vy(height,width);
	Partial(Zn,tmp,AXIS_X);
	DotMul(tmp,U,result);
	Partial(Zn,tmp,AXIS_Y);
	DotMul(tmp,V,tmp1);
	result+=tmp1;
	Partial(U,tmp,AXIS_X);
	Partial(V,Vy,AXIS_Y);
	tmp+=Vy;
	DotMul(Zn,tmp,tmp1);
	result+=tmp1;
	result*=-1;
}