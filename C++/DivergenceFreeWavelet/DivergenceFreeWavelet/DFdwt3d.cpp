#include "DFdwt3D.h"
#ifdef MEMLEAK
	#include "memleak.h"
#endif
void DFdwt3D::loadData(string u_path, string v_path, string w_path, string filter0_path, string filter1_path)
{
	new3D(velocityField.u,DIM,DIM,DIM);
	new3D(velocityField.v,DIM,DIM,DIM);
	new3D(velocityField.w,DIM,DIM,DIM);
	FILE* fp = fopen(u_path.c_str(), "r");
	for (int i = 0; i<DIM; i++)
	for (int j = 0; j<DIM; j++)
	for (int k = 0; k < DIM;k++)
		fscanf(fp, "%lf", &velocityField.u[i][j][k]);
	fclose(fp);
	fp = fopen(v_path.c_str(), "r");
	for (int i = 0; i<DIM; i++)
	for (int j = 0; j<DIM; j++)
	for (int k = 0; k < DIM; k++)
		fscanf(fp, "%lf", &velocityField.v[i][j][k]);
	fclose(fp);
	fp = fopen(w_path.c_str(), "r");
	for (int i = 0; i<DIM; i++)
	for (int j = 0; j<DIM; j++)
	for (int k = 0; k < DIM; k++)
		fscanf(fp, "%lf", &velocityField.w[i][j][k]);
	fclose(fp);
	filter0.loadFilter(filter0_path);
	filter1.loadFilter(filter1_path);
	cout << calcDivergence_sum(velocityField.u, velocityField.v, velocityField.w, DIM, DIM, DIM,scene) << ' ' << calcDivergence_biggest(velocityField.u, velocityField.v, velocityField.w, DIM, DIM, DIM,scene) << endl;
}
void DFdwt3D::wavedec3d(int level)
{
	double*** lastu, ***lastv, ***lastw;
	lastu = velocityField.u;
	lastv = velocityField.v;
	lastw = velocityField.w;
	//print3d("originu.txt", lastu, DIM, DIM, DIM);
	//print3d("originv.txt", lastv, DIM, DIM, DIM);
	//print3d("originw.txt", lastw, DIM, DIM, DIM);
	int LLsize = DIM;
	for (int i = 0; i<level; i++)
	{
		fwt3d_uvw(lastu,lastv,lastw,LLsize);
		//fwt2d_uv(lastu, lastv, ull, uhl, ulh, uhh, vll, vlh, vhl, vhh, LLsize);
		waveletCoeSize.push_back(LLsize / 2);
		LLsize /= 2;
		lastu = waveletCoeULLL[i];
		lastv = waveletCoeVLLL[i];
		lastw = waveletCoeWLLL[i];
		// 		char name[100];
		/*set_field(waveletCoeDF1HHH[i], LLsize, LLsize,LLsize, 0.00);
		set_field(waveletCoeDF1HHL[i], LLsize, LLsize,LLsize, 0.00);
		set_field(waveletCoeDF1HLH[i], LLsize, LLsize,LLsize, 0.00);
		set_field(waveletCoeDF1LHH[i], LLsize, LLsize,LLsize, 0.00);
		set_field(waveletCoeDF1LLH[i], LLsize, LLsize,LLsize, 0.00);
		set_field(waveletCoeDF1LHL[i], LLsize, LLsize,LLsize, 0.00);
		set_field(waveletCoeDF1HLL[i], LLsize, LLsize,LLsize, 0.00);
		set_field(waveletCoeDF2HHH[i], LLsize, LLsize,LLsize, 0.00);
		set_field(waveletCoeDF2HHL[i], LLsize, LLsize,LLsize, 0.00);
		set_field(waveletCoeDF2HLH[i], LLsize, LLsize,LLsize, 0.00);
		set_field(waveletCoeDF2LHH[i], LLsize, LLsize,LLsize, 0.00);
		set_field(waveletCoeDF2LLH[i], LLsize, LLsize,LLsize, 0.00);
		set_field(waveletCoeDF2LHL[i], LLsize, LLsize,LLsize, 0.00);
		set_field(waveletCoeDF2HLL[i], LLsize, LLsize,LLsize, 0.00);
		set_field(waveletCoeNHHH[i], LLsize, LLsize,LLsize, 0.00);
		set_field(waveletCoeNHHL[i], LLsize, LLsize,LLsize, 0.00);
		set_field(waveletCoeNHLH[i], LLsize, LLsize,LLsize, 0.00);
		set_field(waveletCoeNLHH[i], LLsize, LLsize,LLsize, 0.00);
		set_field(waveletCoeNLLH[i], LLsize, LLsize,LLsize, 0.00);
		set_field(waveletCoeNLHL[i], LLsize, LLsize,LLsize, 0.00);
		set_field(waveletCoeNHLL[i], LLsize, LLsize,LLsize, 0.00);
		set_field(waveletCoeULLL[i],LLsize,LLsize,LLsize,0.00);
		set_field(waveletCoeVLLL[i],LLsize,LLsize,LLsize,0.00);
		set_field(waveletCoeWLLL[i],LLsize,LLsize,LLsize,0.00);*/
// 		set_field(uhl, LLsize, LLsize, 0.00);
// 		set_field(uhh, LLsize, LLsize, 0.00);
// 		set_field(vlh, LLsize, LLsize, 0.00);
// 		set_field(vhl, LLsize, LLsize, 0.00);
//		set_field(vhh, LLsize, LLsize, 0.00);
// 		//cout<<calcDivergence_biggest(ull,vll,LLsize,LLsize)<<endl;
// 		sprintf(name, "ull%04d.txt", i);
// 		print2d(string(name), ull, LLsize, LLsize);
// 		sprintf(name, "uhl%04d.txt", i);
// 		print2d(string(name), uhl, LLsize, LLsize);
// 		sprintf(name, "ulh%04d.txt", i);
// 		print2d(string(name), ulh, LLsize, LLsize);
// 		sprintf(name, "uhh%04d.txt", i);
// 		print2d(string(name), uhh, LLsize, LLsize);
// 		sprintf(name, "vll%04d.txt", i);
// 		print2d(string(name), vll, LLsize, LLsize);
// 		sprintf(name, "vhl%04d.txt", i);
// 		print2d(string(name), vhl, LLsize, LLsize);
// 		sprintf(name, "vlh%04d.txt", i);
// 		print2d(string(name), vlh, LLsize, LLsize);
// 		sprintf(name, "vhh%04d.txt", i);
// 		print2d(string(name), vhh, LLsize, LLsize);
		string s = "Layer0";
		s[5] ='0'+i;
		s+='.';
		double d = 0;
		if (cp.getValue<double>(s+"HHH",d))
		{
			mul(waveletCoeDF1HHH[i],LLsize,LLsize,LLsize,d);
			mul(waveletCoeDF2HHH[i],LLsize,LLsize,LLsize,d);
			cout<<s+"HHH "<<d<<endl;
		}
		if (cp.getValue<double>(s+"HHL",d))
		{
			mul(waveletCoeDF1HHL[i],LLsize,LLsize,LLsize,d);
			mul(waveletCoeDF2HHL[i],LLsize,LLsize,LLsize,d);
			cout<<s+"HHL "<<d<<endl;
		}
		if (cp.getValue<double>(s+"HLH",d))
		{
			mul(waveletCoeDF1HLH[i],LLsize,LLsize,LLsize,d);
			mul(waveletCoeDF2HLH[i],LLsize,LLsize,LLsize,d);
			cout<<s+"HLH "<<d<<endl;
		}
		if (cp.getValue<double>(s+"LHH",d))
		{
			mul(waveletCoeDF1LHH[i],LLsize,LLsize,LLsize,d);
			mul(waveletCoeDF2LHH[i],LLsize,LLsize,LLsize,d);
			cout<<s+"LHH "<<d<<endl;
		}
		if (cp.getValue<double>(s+"HLL",d))
		{
			mul(waveletCoeDF1HLL[i],LLsize,LLsize,LLsize,d);
			mul(waveletCoeDF2HLL[i],LLsize,LLsize,LLsize,d);
			cout<<s+"HLL "<<d<<endl;
		}
		if (cp.getValue<double>(s+"LHL",d))
		{
			mul(waveletCoeDF1LHL[i],LLsize,LLsize,LLsize,d);
			mul(waveletCoeDF2LHL[i],LLsize,LLsize,LLsize,d);
			cout<<s+"LHL "<<d<<endl;
		}
		if (cp.getValue<double>(s+"LLH",d))
		{
			mul(waveletCoeDF1LLH[i],LLsize,LLsize,LLsize,d);
			mul(waveletCoeDF2LLH[i],LLsize,LLsize,LLsize,d);
			cout<<s+"LLH "<<d<<endl;
		}
		//set_field(waveletCoeDF2HLL[i], LLsize, LLsize,LLsize,1.00);
			//set_field(waveletCoeDF1HLL[i], LLsize, LLsize,LLsize,1.00);
			//uhh[0][0] = vhh[0][0] = 1;;
			//set_field(ull, vll, LLsize, LLsize,
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
void DFdwt3D::waverec3d(int level)
{
	double*** lastu = waveletCoeULLL[level], ***lastv = waveletCoeVLLL[level],***lastw = waveletCoeWLLL[level], ***nextu, ***nextv,***nextw;
	char name[100];
	for (int i = level; i >= 0; i--)
	{
		ifwt3d_uvw(i,lastu,lastv,lastw,nextu,nextv,nextw,waveletCoeSize[i]);
		//ifwt2d_uv(lastu, waveletCoeUHL[i], waveletCoeULH[i], waveletCoeUHH[i], lastv, waveletCoeVLH[i], waveletCoeVHL[i], waveletCoeVHH[i], nextu, nextv, waveletCoeSize[i]);
		recWaveletCoeULLL.push_back(nextu);
		recWaveletCoeVLLL.push_back(nextv);
		recWaveletCoeWLLL.push_back(nextw);
		lastu = nextu;
		lastv = nextv;
		lastw = nextw;
		recWaveletCoeSize.push_back(waveletCoeSize[i] * 2);
 		//sprintf(name, "nextu%04d.txt", i);
		//print3d(string(name), nextu, waveletCoeSize[i] * 2, waveletCoeSize[i] * 2, waveletCoeSize[i] * 2);
		//sprintf(name, "nextv%04d.txt", i);
		//print3d(string(name), nextv, waveletCoeSize[i] * 2, waveletCoeSize[i] * 2, waveletCoeSize[i] * 2);
		//sprintf(name, "nextw%04d.txt", i);
		//print3d(string(name), nextw, waveletCoeSize[i] * 2, waveletCoeSize[i] * 2, waveletCoeSize[i] * 2);
	}
	//return ;
	cout << calcDivergence_sum(lastu, lastv, lastw, DIM, DIM, DIM, scene) << ' ' << calcDivergence_biggest(lastu, lastv, lastw, DIM, DIM, DIM, scene) << endl;
	//print3d("lastu.txt", lastu, DIM, DIM, DIM);
	//print3d("lastv.txt", lastv, DIM, DIM, DIM);
	//print3d("lastw.txt", lastw, DIM, DIM, DIM);
	//print2d("..\\..\\..\\mathematica\\lastu.txt", lastu, waveletCoeSize[0] * 2, waveletCoeSize[0] * 2);
	//set_field(lastu,lastv,waveletCoeSize[0]*2,waveletCoeSize[0]*2,1,0);
	//print2d("..\\..\\..\\mathematica\\lastv.txt", lastv, waveletCoeSize[0] * 2, waveletCoeSize[0] * 2);
	//system("pause");

	//print2d("realu.txt",waveletCoeULL[level-1],waveletCoeSize[level]*2,waveletCoeSize[level]*2);
	//print2d("realv.txt",waveletCoeVLL[level-1],waveletCoeSize[level]*2,waveletCoeSize[level]*2);
}
void DFdwt3D::fwt3d_uvw(double*** u, double*** v, double *** w, int LLsize)
{
	double*** ullltmp, *** ullhtmp, *** ulhltmp, *** ulhhtmp, *** uhlltmp, *** uhlhtmp, *** uhhltmp, *** uhhhtmp;
	double*** vllltmp, *** vllhtmp, *** vlhltmp, *** vlhhtmp, *** vhlltmp, *** vhlhtmp, *** vhhltmp, *** vhhhtmp;
	double*** wllltmp, *** wllhtmp, *** wlhltmp, *** wlhhtmp, *** whlltmp, *** whlhtmp, *** whhltmp, *** whhhtmp;
	double*** ulll, *** ullh, *** ulhl, *** ulhh, *** uhll, *** uhlh, *** uhhl, *** uhhh;
	double*** vlll, *** vllh, *** vlhl, *** vlhh, *** vhll, *** vhlh, *** vhhl, *** vhhh;
	double*** wlll, *** wllh, *** wlhl, *** wlhh, *** whll, *** whlh, *** whhl, *** whhh;
	fwt3d(u, ullltmp, ullhtmp, ulhltmp, ulhhtmp, uhlltmp, uhlhtmp, uhhltmp, uhhhtmp, filter1.lod, filter0.lod, filter0.lod, filter1.hid, filter0.hid, filter0.hid, LLsize);
	//print3d("ulll.txt",ullltmp,LLsize/2,LLsize/2,LLsize/2);
	fwt3d(v, vllltmp, vllhtmp, vlhltmp, vlhhtmp, vhlltmp, vhlhtmp, vhhltmp, vhhhtmp, filter0.lod, filter1.lod, filter0.lod, filter0.hid, filter1.hid, filter0.hid, LLsize);
	//print3d("vlll.txt",vllltmp,LLsize/2,LLsize/2,LLsize/2);
	fwt3d(w, wllltmp, wllhtmp, wlhltmp, wlhhtmp, whlltmp, whlhtmp, whhltmp, whhhtmp, filter0.lod, filter0.lod, filter1.lod, filter0.hid, filter0.hid, filter1.hid, LLsize);
	//print3d("wlll.txt",wllltmp,LLsize/2,LLsize/2,LLsize/2);
	new3D(ulll, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(ullh, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(ulhl, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(ulhh, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(uhll, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(uhlh, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(uhhl, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(uhhh, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(vlll, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(vllh, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(vlhl, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(vlhh, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(vhll, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(vhlh, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(vhhl, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(vhhh, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(wlll, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(wllh, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(wlhl, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(wlhh, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(whll, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(whlh, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(whhl, LLsize / 2, LLsize / 2, LLsize / 2);
	new3D(whhh, LLsize / 2, LLsize / 2, LLsize / 2);
// 	print3d("uhhh1.txt",uhhhtmp,2,2,2);
// 	print3d("uhlh1.txt",uhlhtmp,2,2,2);
// 	print3d("ulhh1.txt",ulhhtmp,2,2,2);
// 	print3d("uhhl1.txt",uhhltmp,2,2,2);
// 	print3d("ullh1.txt",ullhtmp,2,2,2);
// 	print3d("ulhl1.txt",ulhltmp,2,2,2);
// 	print3d("uhll1.txt",uhlltmp,2,2,2);
// 	print3d("ulll1.txt",ullltmp,2,2,2);
// 	print3d("vhhh1.txt",vhhhtmp,2,2,2);
// 	print3d("vhlh1.txt",vhlhtmp,2,2,2);
// 	print3d("vlhh1.txt",vlhhtmp,2,2,2);
// 	print3d("vhhl1.txt",vhhltmp,2,2,2);
// 	print3d("vllh1.txt",vllhtmp,2,2,2);
// 	print3d("vlhl1.txt",vlhltmp,2,2,2);
// 	print3d("vhll1.txt",vhlltmp,2,2,2);
// 	print3d("vlll1.txt",vllltmp,2,2,2);
// 	print3d("whhh1.txt",whhhtmp,2,2,2);
// 	print3d("whlh1.txt",whlhtmp,2,2,2);
// 	print3d("wlhh1.txt",wlhhtmp,2,2,2);
// 	print3d("whhl1.txt",whhltmp,2,2,2);
// 	print3d("wllh1.txt",wllhtmp,2,2,2);
// 	print3d("wlhl1.txt",wlhltmp,2,2,2);
// 	print3d("whll1.txt",whlltmp,2,2,2);
// 	print3d("wlll1.txt",wllltmp,2,2,2);
	for (int i = 0; i < LLsize / 2;i++)
	for (int j = 0; j < LLsize / 2;j++)
	for (int k = 0; k < LLsize / 2; k++)
	{
		ulll[i][j][k] = ullltmp[i][j][k];
		vlll[i][j][k] = vllltmp[i][j][k];
		wlll[i][j][k] = wllltmp[i][j][k];
		uhll[i][j][k] = vhlltmp[i][j][k];
		vhll[i][j][k] = whlltmp[i][j][k];
		whll[i][j][k] = uhlltmp[i][j][k];
		if (j != 0)
			whll[i][j][k] += 0.25*vhlltmp[i][j][k] - 0.25*vhlltmp[i][j - 1][k];
		else
			whll[i][j][k] += 0.25*vhlltmp[i][j][k] - 0.25*(scene == LOOP ? vhlltmp[i][LLsize / 2 - 1][k] : 0);
		if (k != 0)
			whll[i][j][k] += 0.25*whlltmp[i][j][k] - 0.25*whlltmp[i][j][k - 1];
		else
			whll[i][j][k] += 0.25*whlltmp[i][j][k] - 0.25*(scene == LOOP ? whlltmp[i][j][LLsize / 2 - 1] : 0);
		ulhl[i][j][k] = ulhltmp[i][j][k];
		vlhl[i][j][k] = wlhltmp[i][j][k];
		wlhl[i][j][k] = vlhltmp[i][j][k];
		if (i!=0)
			wlhl[i][j][k]+=0.25*(ulhltmp[i][j][k] - ulhltmp[i - 1][j][k]);
		else 
			wlhl[i][j][k] += 0.25*ulhltmp[i][j][k] - 0.25*(scene == LOOP ? ulhltmp[LLsize / 2 - 1][j][k] : 0);
		if (k!=0)
			wlhl[i][j][k] += 0.25*wlhltmp[i][j][k]-0.25*wlhltmp[i][j][k-1];
		else
			wlhl[i][j][k] += 0.25*wlhltmp[i][j][k] - 0.25*(scene == LOOP? wlhltmp[i][j][LLsize/2-1]:0);
		ullh[i][j][k] = ullhtmp[i][j][k];
		vllh[i][j][k] = vllhtmp[i][j][k];
		wllh[i][j][k] = wllhtmp[i][j][k];
		if (i!=0)
			wllh[i][j][k] += 0.25*ullhtmp[i][j][k]-0.25*ullhtmp[i-1][j][k];
		else
			wllh[i][j][k] += 0.25*ullhtmp[i][j][k] - 0.25*(scene == LOOP? ullhtmp[LLsize/2-1][j][k]:0);
		if (j!=0)
			wllh[i][j][k] += 0.25*vllhtmp[i][j][k]-0.25*vllhtmp[i][j-1][k];
		else
			wllh[i][j][k] += 0.25*vllhtmp[i][j][k] - 0.25*(scene == LOOP? vllhtmp[i][LLsize/2-1][k]:0);
		uhhl[i][j][k] = 0.5*uhhltmp[i][j][k]-0.5*vhhltmp[i][j][k];
		vhhl[i][j][k] = whhltmp[i][j][k];
		whhl[i][j][k] = 0.5*uhhltmp[i][j][k]+0.5*vhhltmp[i][j][k];
		if (k!=0)
			whhl[i][j][k] += 0.125*whhltmp[i][j][k]-0.125*whhltmp[i][j][k-1];
		else
			whhl[i][j][k] += 0.125*whhltmp[i][j][k] - 0.125*(scene == LOOP? whhltmp[i][j][LLsize/2-1]:0);
		uhlh[i][j][k] =0.5*uhlhtmp[i][j][k] -0.5*whlhtmp[i][j][k];
		vhlh[i][j][k] = vhlhtmp[i][j][k];
		whlh[i][j][k] = 0.5*uhlhtmp[i][j][k]+0.5*whlhtmp[i][j][k];
		if (j!=0)
			whlh[i][j][k] += 0.125*vhlhtmp[i][j][k]-0.125*vhlhtmp[i][j-1][k];
		else
			whlh[i][j][k] += 0.125*vhlhtmp[i][j][k] - 0.125*(scene == LOOP? vhlhtmp[i][LLsize/2-1][k]:0);
		ulhh[i][j][k] = 0.5*vlhhtmp[i][j][k]-0.5*wlhhtmp[i][j][k];
		vlhh[i][j][k] = ulhhtmp[i][j][k];
		wlhh[i][j][k] = 0.5*vlhhtmp[i][j][k] + 0.5*wlhhtmp[i][j][k];
		if (i!=0)
			wlhh[i][j][k] += 0.125*ulhhtmp[i][j][k]-0.125*ulhhtmp[i-1][j][k];
		else
			wlhh[i][j][k] += 0.125*ulhhtmp[i][j][k] - 0.125*(scene == LOOP? ulhhtmp[LLsize/2-1][j][k]:0);
		uhhh[i][j][k] = 1.0/3.0*(-2*uhhhtmp[i][j][k]+vhhhtmp[i][j][k]+whhhtmp[i][j][k]);
		vhhh[i][j][k] = 1.0/3.0*(-uhhhtmp[i][j][k]+2*vhhhtmp[i][j][k]-whhhtmp[i][j][k]);
		whhh[i][j][k] = 1.0/3.0*(uhhhtmp[i][j][k]+vhhhtmp[i][j][k]+whhhtmp[i][j][k]);
	}
	waveletCoeULLL.push_back(ulll);
	waveletCoeDF1LLH.push_back(ullh);
	waveletCoeDF1LHL.push_back(ulhl);
	waveletCoeDF1HLL.push_back(uhll);
	waveletCoeDF1HHL.push_back(uhhl);
	waveletCoeDF1HLH.push_back(uhlh);
	waveletCoeDF1LHH.push_back(ulhh);
	waveletCoeDF1HHH.push_back(uhhh);
	waveletCoeVLLL.push_back(vlll);
	waveletCoeDF2LLH.push_back(vllh);
	waveletCoeDF2LHL.push_back(vlhl);
	waveletCoeDF2HLL.push_back(vhll);
	waveletCoeDF2HHL.push_back(vhhl);
	waveletCoeDF2HLH.push_back(vhlh);
	waveletCoeDF2LHH.push_back(vlhh);
	waveletCoeDF2HHH.push_back(vhhh);
	waveletCoeWLLL.push_back(wlll);
	waveletCoeNLLH.push_back(wllh);
	waveletCoeNLHL.push_back(wlhl);
	waveletCoeNHLL.push_back(whll);
	waveletCoeNHHL.push_back(whhl);
	waveletCoeNHLH.push_back(whlh);
	waveletCoeNLHH.push_back(wlhh);
	waveletCoeNHHH.push_back(whhh);
	release3D(ullltmp,LLsize/2,LLsize/2);
	release3D(ullhtmp,LLsize/2,LLsize/2);
	release3D(ulhltmp,LLsize/2,LLsize/2);
	release3D(uhlltmp,LLsize/2,LLsize/2);
	release3D(ulhhtmp,LLsize/2,LLsize/2);
	release3D(uhlhtmp,LLsize/2,LLsize/2);
	release3D(uhhltmp,LLsize/2,LLsize/2);
	release3D(uhhhtmp,LLsize/2,LLsize/2);
	release3D(vllltmp,LLsize/2,LLsize/2);
	release3D(vllhtmp,LLsize/2,LLsize/2);
	release3D(vlhltmp,LLsize/2,LLsize/2);
	release3D(vhlltmp,LLsize/2,LLsize/2);
	release3D(vlhhtmp,LLsize/2,LLsize/2);
	release3D(vhlhtmp,LLsize/2,LLsize/2);
	release3D(vhhltmp,LLsize/2,LLsize/2);
	release3D(vhhhtmp,LLsize/2,LLsize/2);
	release3D(wllltmp,LLsize/2,LLsize/2);
	release3D(wllhtmp,LLsize/2,LLsize/2);
	release3D(wlhltmp,LLsize/2,LLsize/2);
	release3D(whlltmp,LLsize/2,LLsize/2);
	release3D(wlhhtmp,LLsize/2,LLsize/2);
	release3D(whlhtmp,LLsize/2,LLsize/2);
	release3D(whhltmp,LLsize/2,LLsize/2);
	release3D(whhhtmp,LLsize/2,LLsize/2);
}
void DFdwt3D::test()
{
	double*** tmp, *** tmp1,***tt,***tt1;
	conv_down3d(velocityField.u,tmp,2,filter0.lod,4,4,4);
	conv_down3d(velocityField.u,tmp1,2,filter0.hid,4,4,4);
	conv_up3d(tmp,tt,2,filter0.lor,4,4,2);
	conv_up3d(tmp1,tt1,2,filter0.hir,4,4,2);
	addMatrix_3d(tt,tt1,tt,4,4,4);
	//print3d("tt.txt",tt,4,4,4);
}
void DFdwt3D::ifwt3d_uvw(int level,double*** ull,double*** vll,double*** wll, double*** &u, double*** &v, double*** &w, int LLsize)
{
	double*** ullltmp = ull, *** ullhtmp=waveletCoeDF1LLH[level], *** ulhltmp=waveletCoeDF1LHL[level], *** ulhhtmp=waveletCoeDF1LHH[level], *** uhlltmp=waveletCoeDF1HLL[level], *** uhlhtmp=waveletCoeDF1HLH[level], *** uhhltmp=waveletCoeDF1HHL[level], *** uhhhtmp=waveletCoeDF1HHH[level];
	double*** vllltmp = vll, *** vllhtmp=waveletCoeDF2LLH[level], *** vlhltmp=waveletCoeDF2LHL[level], *** vlhhtmp=waveletCoeDF2LHH[level], *** vhlltmp=waveletCoeDF2HLL[level], *** vhlhtmp=waveletCoeDF2HLH[level], *** vhhltmp=waveletCoeDF2HHL[level], *** vhhhtmp=waveletCoeDF2HHH[level];
	double*** wllltmp = wll, *** wllhtmp=waveletCoeNLLH[level], *** wlhltmp=waveletCoeNLHL[level], *** wlhhtmp=waveletCoeNLHH[level], *** whlltmp=waveletCoeNHLL[level], *** whlhtmp=waveletCoeNHLH[level], *** whhltmp=waveletCoeNHHL[level], *** whhhtmp=waveletCoeNHHH[level];
	double*** ulll, *** ullh, *** ulhl, *** ulhh, *** uhll, *** uhlh, *** uhhl, *** uhhh;
	double*** vlll, *** vllh, *** vlhl, *** vlhh, *** vhll, *** vhlh, *** vhhl, *** vhhh;
	double*** wlll, *** wllh, *** wlhl, *** wlhh, *** whll, *** whlh, *** whhl, *** whhh;
//	new3D(u, LLsize*2, LLsize*2, LLsize*2);
//	new3D(v, LLsize*2, LLsize*2, LLsize*2);
//	new3D(w, LLsize*2, LLsize*2, LLsize*2);
	new3D(ulll, LLsize, LLsize, LLsize);
	new3D(ullh, LLsize, LLsize, LLsize);
	new3D(ulhl, LLsize, LLsize, LLsize);
	new3D(ulhh, LLsize, LLsize, LLsize);
	new3D(uhll, LLsize, LLsize, LLsize);
	new3D(uhlh, LLsize, LLsize, LLsize);
	new3D(uhhl, LLsize, LLsize, LLsize);
	new3D(uhhh, LLsize, LLsize, LLsize);
	new3D(vlll, LLsize, LLsize, LLsize);
	new3D(vllh, LLsize, LLsize, LLsize);
	new3D(vlhl, LLsize, LLsize, LLsize);
	new3D(vlhh, LLsize, LLsize, LLsize);
	new3D(vhll, LLsize, LLsize, LLsize);
	new3D(vhlh, LLsize, LLsize, LLsize);
	new3D(vhhl, LLsize, LLsize, LLsize);
	new3D(vhhh, LLsize, LLsize, LLsize);
	new3D(wlll, LLsize, LLsize, LLsize);
	new3D(wllh, LLsize, LLsize, LLsize);
	new3D(wlhl, LLsize, LLsize, LLsize);
	new3D(wlhh, LLsize, LLsize, LLsize);
	new3D(whll, LLsize, LLsize, LLsize);
	new3D(whlh, LLsize, LLsize, LLsize);
	new3D(whhl, LLsize, LLsize, LLsize);
	new3D(whhh, LLsize, LLsize, LLsize);
	for (int i = 0;i<LLsize;i++)
		for (int j = 0;j<LLsize;j++)
			for (int k = 0;k<LLsize;k++)
			{
				ulll[i][j][k] = ullltmp[i][j][k];
				vlll[i][j][k] = vllltmp[i][j][k];
				wlll[i][j][k] = wllltmp[i][j][k];
				uhll[i][j][k] = whlltmp[i][j][k];
				if (j!=0)
					uhll[i][j][k] -= 0.25*uhlltmp[i][j][k]-0.25*uhlltmp[i][j-1][k];
				else
					uhll[i][j][k] -= 0.25*uhlltmp[i][j][k]-0.25*(scene == LOOP?uhlltmp[i][LLsize-1][k]:0);
				if (k!=0)
					uhll[i][j][k] -= 0.25*vhlltmp[i][j][k]-0.25*vhlltmp[i][j][k-1];
				else
					uhll[i][j][k] -= 0.25*vhlltmp[i][j][k]-0.25*(scene == LOOP?vhlltmp[i][j][LLsize-1]:0);
				vhll[i][j][k] = uhlltmp[i][j][k];
				whll[i][j][k] = vhlltmp[i][j][k];
				ulhl[i][j][k] = ulhltmp[i][j][k];
				vlhl[i][j][k] = wlhltmp[i][j][k];
				if (i!=0)
					vlhl[i][j][k] -= 0.25*ulhltmp[i][j][k]-0.25*ulhltmp[i-1][j][k];
				else
					vlhl[i][j][k] -= 0.25*ulhltmp[i][j][k]-0.25*(scene == LOOP?ulhltmp[LLsize-1][j][k]:0);
				if (k!=0)
					vlhl[i][j][k] -= 0.25*vlhltmp[i][j][k]-0.25*vlhltmp[i][j][k-1];
				else
					vlhl[i][j][k] -= 0.25*vlhltmp[i][j][k]-0.25*(scene == LOOP?vlhltmp[i][j][LLsize-1]:0);
				wlhl[i][j][k]=vlhltmp[i][j][k];
				ullh[i][j][k] = ullhtmp[i][j][k];
				vllh[i][j][k] = vllhtmp[i][j][k];
				wllh[i][j][k] = wllhtmp[i][j][k];
				if (i!=0)
					wllh[i][j][k] -= 0.25*ullhtmp[i][j][k]-0.25*ullhtmp[i-1][j][k];
				else
					wllh[i][j][k] -= 0.25*ullhtmp[i][j][k]-0.25*(scene == LOOP?ullhtmp[LLsize-1][j][k]:0);
				if (j!=0)
					wllh[i][j][k] -= 0.25*vllhtmp[i][j][k]-0.25*vllhtmp[i][j-1][k];
				else
					wllh[i][j][k] -= 0.25*vllhtmp[i][j][k]-0.25*(scene == LOOP?vllhtmp[i][LLsize-1][k]:0);
				uhhl[i][j][k] = uhhltmp[i][j][k] + whhltmp[i][j][k];
				if (k!=0)
					uhhl[i][j][k] -= 0.125*(vhhltmp[i][j][k] - vhhltmp[i][j][k-1]);
				else
					uhhl[i][j][k] -= 0.125*(vhhltmp[i][j][k] - (scene == LOOP?vhhltmp[i][j][LLsize - 1]:0));
				vhhl[i][j][k] = -uhhltmp[i][j][k] + whhltmp[i][j][k];
				if (k!=0)
					vhhl[i][j][k] -= 0.125*(vhhltmp[i][j][k] - vhhltmp[i][j][k-1]);
				else
					vhhl[i][j][k] -= 0.125*(vhhltmp[i][j][k] - (scene == LOOP?vhhltmp[i][j][LLsize - 1]:0));
				whhl[i][j][k] = vhhltmp[i][j][k];
				uhlh[i][j][k] = uhlhtmp[i][j][k]+whlhtmp[i][j][k];
				if (j!=0)
					uhlh[i][j][k] -= 0.125*(vhlhtmp[i][j][k] - vhlhtmp[i][j-1][k]);
				else
					uhlh[i][j][k] -= 0.125*(vhlhtmp[i][j][k] - (scene == LOOP?vhlhtmp[i][LLsize - 1][k]:0));
				vhlh[i][j][k] = vhlhtmp[i][j][k];
				whlh[i][j][k] = -uhlhtmp[i][j][k]+whlhtmp[i][j][k];
				if (j!=0)
					whlh[i][j][k] -= 0.125*(vhlhtmp[i][j][k] - vhlhtmp[i][j-1][k]);
				else
					whlh[i][j][k] -= 0.125*(vhlhtmp[i][j][k] - (scene == LOOP?vhlhtmp[i][LLsize - 1][k]:0));
				ulhh[i][j][k] = vlhhtmp[i][j][k];
				vlhh[i][j][k] = ulhhtmp[i][j][k] + wlhhtmp[i][j][k];
				if (i!=0)
					vlhh[i][j][k] -= 0.125*(vlhhtmp[i][j][k] - vlhhtmp[i-1][j][k]);
				else
					vlhh[i][j][k] -= 0.125*(vlhhtmp[i][j][k] - (scene == LOOP?vlhhtmp[LLsize - 1][j][k]:0));
				wlhh[i][j][k] = -ulhhtmp[i][j][k] + wlhhtmp[i][j][k];
				if (i!=0)
					wlhh[i][j][k] -= 0.125*(vlhhtmp[i][j][k] - vlhhtmp[i-1][j][k]);
				else
					wlhh[i][j][k] -= 0.125*(vlhhtmp[i][j][k] - (scene == LOOP?vlhhtmp[LLsize - 1][j][k]:0));
				uhhh[i][j][k] = - uhhhtmp[i][j][k]+whhhtmp[i][j][k];
				vhhh[i][j][k] = vhhhtmp[i][j][k]+whhhtmp[i][j][k];
				whhh[i][j][k] = uhhhtmp[i][j][k]-vhhhtmp[i][j][k]+whhhtmp[i][j][k];

			}

// 			print3d("uhhhtmp.txt",uhhhtmp,2,2,2);
// 			print3d("vlhhtmp.txt",vlhhtmp,2,2,2);
// 			print3d("uhhh.txt",uhhh,2,2,2);
// 			print3d("uhlh.txt",uhlh,2,2,2);
// 			print3d("ulhh.txt",ulhh,2,2,2);
// 			print3d("uhhl.txt",uhhl,2,2,2);
// 			print3d("ullh.txt",ullh,2,2,2);
// 			print3d("ulhl.txt",ulhl,2,2,2);
// 			print3d("uhll.txt",uhll,2,2,2);
// 			print3d("ulll.txt",ulll,2,2,2);
// 			print3d("vhhh.txt",vhhh,2,2,2);
// 			print3d("vhlh.txt",vhlh,2,2,2);
// 			print3d("vlhh.txt",vlhh,2,2,2);
// 			print3d("vhhl.txt",vhhl,2,2,2);
// 			print3d("vllh.txt",vllh,2,2,2);
// 			print3d("vlhl.txt",vlhl,2,2,2);
// 			print3d("vhll.txt",vhll,2,2,2);
// 			print3d("vlll.txt",vlll,2,2,2);
// 			print3d("whhh.txt",whhh,2,2,2);
// 			print3d("whlh.txt",whlh,2,2,2);
// 			print3d("wlhh.txt",wlhh,2,2,2);
// 			print3d("whhl.txt",whhl,2,2,2);
// 			print3d("wllh.txt",wllh,2,2,2);
// 			print3d("wlhl.txt",wlhl,2,2,2);
// 			print3d("whll.txt",whll,2,2,2);
// 			print3d("wlll.txt",wlll,2,2,2);
			
	ifwt3d(ulll,ullh,ulhl,ulhh,uhll,uhlh,uhhl,uhhh,filter1.lor, filter0.lor, filter0.lor, filter1.hir, filter0.hir, filter0.hir,u, LLsize);
	ifwt3d(vlll,vllh,vlhl,vlhh,vhll,vhlh,vhhl,vhhh,filter0.lor, filter1.lor, filter0.lor, filter0.hir, filter1.hir, filter0.hir,v, LLsize);
	ifwt3d(wlll,wllh,wlhl,wlhh,whll,whlh,whhl,whhh,filter0.lor, filter0.lor, filter1.lor, filter0.hir, filter0.hir, filter1.hir,w, LLsize);
	release3D(ulll,LLsize,LLsize);
	release3D(ullh,LLsize,LLsize);
	release3D(ulhl,LLsize,LLsize);
	release3D(uhll,LLsize,LLsize);
	release3D(ulhh,LLsize,LLsize);
	release3D(uhlh,LLsize,LLsize);
	release3D(uhhl,LLsize,LLsize);
	release3D(uhhh,LLsize,LLsize);
	release3D(vlll,LLsize,LLsize);
	release3D(vllh,LLsize,LLsize);
	release3D(vlhl,LLsize,LLsize);
	release3D(vhll,LLsize,LLsize);
	release3D(vlhh,LLsize,LLsize);
	release3D(vhlh,LLsize,LLsize);
	release3D(vhhl,LLsize,LLsize);
	release3D(vhhh,LLsize,LLsize);
	release3D(wlll,LLsize,LLsize);
	release3D(wllh,LLsize,LLsize);
	release3D(wlhl,LLsize,LLsize);
	release3D(whll,LLsize,LLsize);
	release3D(wlhh,LLsize,LLsize);
	release3D(whlh,LLsize,LLsize);
	release3D(whhl,LLsize,LLsize);
	release3D(whhh,LLsize,LLsize);
}
void DFdwt3D::fwt3d(double*** LLL, double*** &lll, double*** &llh, double*** &lhl, double*** &lhh, double*** &hll, double*** &hlh, double*** &hhl, double*** &hhh, SubFilter lodx, SubFilter lody, SubFilter lodz, SubFilter hidx, SubFilter hidy, SubFilter hidz, int LLsize)
{
	double*** tmpL, ***tmpH,***tmpHL,***tmpLH,***tmpLL,***tmpHH;
	//print2d("LL2.txt", LL, LLsize, LLsize);
	conv_down3d(LLL, tmpL, 0, lodx, LLsize, LLsize, LLsize);
	conv_down3d(tmpL, tmpLL, 1, lody, LLsize / 2, LLsize, LLsize);
	conv_down3d(tmpL, tmpLH, 1, hidy, LLsize / 2, LLsize, LLsize);
	conv_down3d(tmpLL, lll, 2, lodz, LLsize / 2, LLsize / 2, LLsize);
	conv_down3d(tmpLL, llh, 2, hidz, LLsize / 2, LLsize / 2, LLsize);
	conv_down3d(tmpLH, lhl, 2, lodz, LLsize / 2, LLsize / 2, LLsize);
	conv_down3d(tmpLH, lhh, 2, hidz, LLsize / 2, LLsize / 2, LLsize);
	conv_down3d(LLL, tmpH, 0, hidx, LLsize, LLsize, LLsize);
	conv_down3d(tmpH, tmpHL, 1, lody, LLsize / 2, LLsize, LLsize);
	conv_down3d(tmpH, tmpHH, 1, hidy, LLsize / 2, LLsize, LLsize);
	conv_down3d(tmpHL, hll, 2, lodz, LLsize / 2, LLsize / 2, LLsize);
	conv_down3d(tmpHL, hlh, 2, hidz, LLsize / 2, LLsize / 2, LLsize);
	conv_down3d(tmpHH, hhl, 2, lodz, LLsize / 2, LLsize / 2, LLsize);
	conv_down3d(tmpHH, hhh, 2, hidz, LLsize / 2, LLsize / 2, LLsize);
	release3D(tmpL, LLsize / 2, LLsize);
	release3D(tmpH, LLsize / 2, LLsize);
	release3D(tmpLH, LLsize / 2, LLsize/2);
	release3D(tmpHL, LLsize / 2, LLsize/2);
	release3D(tmpLL, LLsize / 2, LLsize/2);
	release3D(tmpHH, LLsize / 2, LLsize/2);
}
void DFdwt3D::ifwt3d(double*** lll, double*** llh, double*** lhl, double*** lhh, double*** hll, double*** hlh, double*** hhl, double*** hhh, SubFilter lorx, SubFilter lory, SubFilter lorz, SubFilter hirx, SubFilter hiry, SubFilter hirz, double*** &LLL, int LLsize)
{
	double*** tmplll, ***tmpllh, ***tmplhl, ***tmplhh, ***tmphll, ***tmphlh, ***tmphhl, ***tmphhh;
	double*** tmpll, ***tmphl, ***tmplh, ***tmphh,***tmpl,***tmph;
	
	conv_up3d(lll, tmplll, 2, lorz, LLsize, LLsize, LLsize);
	conv_up3d(llh, tmpllh, 2, hirz, LLsize, LLsize, LLsize);
	addMatrix_3d(tmplll, tmpllh, tmplll, LLsize, LLsize, LLsize * 2);

	conv_up3d(lhl, tmplhl, 2, lorz, LLsize, LLsize, LLsize);
	conv_up3d(lhh, tmplhh, 2, hirz, LLsize, LLsize, LLsize);
	addMatrix_3d(tmplhl, tmplhh, tmplhl, LLsize, LLsize, LLsize * 2);

	conv_up3d(hll, tmphll, 2, lorz, LLsize, LLsize, LLsize);
	conv_up3d(hlh, tmphlh, 2, hirz, LLsize, LLsize, LLsize);
	addMatrix_3d(tmphll, tmphlh, tmphll, LLsize, LLsize, LLsize * 2);

	conv_up3d(hhl, tmphhl, 2, lorz, LLsize, LLsize, LLsize);
	conv_up3d(hhh, tmphhh, 2, hirz, LLsize, LLsize, LLsize);
	addMatrix_3d(tmphhl, tmphhh, tmphhl, LLsize, LLsize, LLsize * 2);

	conv_up3d(tmphll, tmphl, 1, lory, LLsize, LLsize, LLsize * 2);
	conv_up3d(tmphhl, tmphh, 1, hiry, LLsize, LLsize, LLsize * 2);
	addMatrix_3d(tmphl, tmphh, tmphl, LLsize, LLsize * 2, LLsize * 2);

	conv_up3d(tmplll, tmpll, 1, lory, LLsize, LLsize, LLsize * 2);
	conv_up3d(tmplhl, tmplh, 1, hiry, LLsize, LLsize, LLsize * 2);
	addMatrix_3d(tmpll, tmplh, tmpll , LLsize, LLsize * 2, LLsize * 2);

	conv_up3d(tmpll, LLL, 0, lorx, LLsize, LLsize * 2, LLsize * 2);
	conv_up3d(tmphl, tmph, 0, hirx, LLsize, LLsize * 2, LLsize * 2);
	addMatrix_3d(LLL, tmph, LLL, LLsize*2, LLsize * 2, LLsize * 2);
	release3D(tmph, LLsize * 2, LLsize * 2);
	release3D(tmpll, LLsize, LLsize * 2);
	release3D(tmphl, LLsize, LLsize * 2);
	release3D(tmplh, LLsize, LLsize * 2);
	release3D(tmphh, LLsize, LLsize * 2);
	release3D(tmphhh, LLsize, LLsize);
	release3D(tmphhl, LLsize, LLsize);
	release3D(tmphlh, LLsize, LLsize);
	release3D(tmphll, LLsize, LLsize);
	release3D(tmplll, LLsize, LLsize);
	release3D(tmpllh, LLsize, LLsize);
	release3D(tmplhl, LLsize, LLsize);
	release3D(tmplhh, LLsize, LLsize);
}
void DFdwt3D::conv_down3d(double*** input, double*** &output, int dir, SubFilter filter, int input_xsize, int input_ysize, int input_zsize)
{
	int filterSize = filter.filterLen;
	if (dir == 0)
	{
		double*** outtmp;// = new double**[input_xsize];
		new3D(outtmp, input_xsize / 2, input_ysize, input_zsize);
		reset3D(outtmp, input_xsize / 2, input_ysize, input_zsize);
		for (int i = 0; i<input_xsize/2; i++)
		{
			for (int j = 0; j<input_ysize; j++)
			{
				for (int k = 0; k < input_zsize;k++)
				for (int l = 0; l<filter.filterLen; l++)
				{
#ifdef DUAL_CONVOLUTION
					int input_id = i * 2 + 1 +/*(filterSize-1)+*/l + filter.startid;
#else
					int input_id = i * 2 + 1 -/*(filterSize-1)+*/l - filter.startid;
#endif
					if (input_id<0)
					{
						if (scene == LOOP)
							input_id = (input_id + 10 * input_xsize) % input_xsize;
						else if (scene == BOUNDARY)
							continue;
					}
					else if (input_id >= input_xsize)
					{
						if (scene == LOOP)
							input_id %= input_xsize;
						else if (scene == BOUNDARY)
							continue;
					}
					outtmp[i][j][k] += filter.filter[/*(filterSize-1)-*/l] * input[input_id][j][k];
				}
			}
		}
		output = outtmp;
	}
	else if (dir == 1)
	{
		double*** outtmp;// = new double**[input_xsize];
		new3D(outtmp, input_xsize, input_ysize/2, input_zsize);
		reset3D(outtmp, input_xsize, input_ysize/2, input_zsize);
		for (int i = 0; i<input_xsize; i++)
		{
			for (int j = 0; j<input_ysize / 2; j++)
			{
				for (int k = 0; k < input_zsize;k++)
				for (int l = 0; l < filter.filterLen; l++)
				{
#ifdef DUAL_CONVOLUTION
					int input_id = j * 2 + 1 +/*(filterSize-1)+*/l + filter.startid;
#else
					int input_id = j * 2 + 1 -/*(filterSize-1)+*/l - filter.startid;
#endif
					if (input_id<0)
					{
						if (scene == LOOP)
							input_id = (input_id + 10 * input_ysize) % input_ysize;
						else if (scene == BOUNDARY)
							continue;
					}
					else if (input_id >= input_ysize)
					{
						if (scene == LOOP)
							input_id %= input_ysize;
						else if (scene == BOUNDARY)
							continue;
					}
					outtmp[i][j][k] += filter.filter[/*(filterSize-1)-*/l] * input[i][input_id][k];
				}
			}
		}
		output = outtmp;
	}
	else if (dir == 2)
	{
		double*** outtmp;// = new double**[input_xsize];
		new3D(outtmp, input_xsize, input_ysize, input_zsize / 2);
		reset3D(outtmp, input_xsize, input_ysize, input_zsize / 2);
		for (int i = 0; i<input_xsize; i++)
		{
			for (int j = 0; j<input_ysize; j++)
			{
				for (int k = 0; k < input_zsize / 2;k++)
				for (int l = 0; l<filter.filterLen; l++)
				{
#ifdef DUAL_CONVOLUTION
					int input_id = k * 2 + 1 +/*(filterSize-1)+*/l + filter.startid;
#else
					int input_id = k * 2 + 1 -/*(filterSize-1)+*/l - filter.startid;
#endif
					if (input_id<0)
					{
						if (scene == LOOP)
							input_id = (input_id + 10 * input_zsize) % input_zsize;
						else if (scene == BOUNDARY)
							continue;
					}
					else if (input_id >= input_zsize)
					{
						if (scene == LOOP)
							input_id %= input_zsize;
						else if (scene == BOUNDARY)
							continue;
					}
					outtmp[i][j][k] += filter.filter[/*(filterSize-1)-*/l] * input[i][j][input_id];
				}
			}
		}
		output = outtmp;

	}
}
void DFdwt3D::conv_up3d(double*** input, double*** &output, int dir, SubFilter filter, int input_xsize, int input_ysize, int input_zsize)
{
	int filterSize = filter.filterLen;
	if (dir == 0)
	{
		double*** outtmp;// = new double*[input_ysize];
		new3D(outtmp, input_xsize * 2, input_ysize, input_zsize);
		reset3D(outtmp, input_xsize * 2, input_ysize, input_zsize);
		for (int i = 0; i<input_xsize*2; i++)
		{
			for (int j = 0; j<input_ysize; j++)
			{
				for (int k = 0; k < input_zsize;k++)
				for (int l = 0; l<filter.filterLen; l++)
				{
#ifdef DUAL_CONVOLUTION
					int input_id = i -/*(filterSize-1)+*/l - filter.startid;
#else
					int input_id = i +/*(filterSize-1)+*/l + filter.startid;
#endif
					if (input_id<0)
					{
						if (scene == LOOP)
							input_id = (input_id + input_xsize * 20) % (input_xsize * 2);
						else if (scene == BOUNDARY)
							continue;
					}
					else if (input_id >= input_xsize * 2)
					{
						if (scene == LOOP)
							input_id %= input_xsize * 2;
						else if (scene == BOUNDARY)
							continue;
					}
					if (input_id % 2 == 0)
						continue;
					input_id /= 2;
					outtmp[i][j][k] += filter.filter[/*(filterSize-1)-*/l] * input[input_id][j][k];
				}
			}
		}
		output = outtmp;
	}
	else if (dir == 1)
	{
		double*** outtmp;// = new double*[input_ysize];
		new3D(outtmp, input_xsize, input_ysize*2, input_zsize);
		reset3D(outtmp, input_xsize, input_ysize*2, input_zsize);
		for (int i = 0; i<input_xsize; i++)
		{
			for (int j = 0; j<input_ysize * 2; j++)
			{
				for (int k = 0; k < input_zsize;k++)
				for (int l = 0; l<filter.filterLen; l++)
				{
#ifdef DUAL_CONVOLUTION
					int input_id = j -/*(filterSize-1)+*/l - filter.startid;
#else
					int input_id = j +/*(filterSize-1)+*/l + filter.startid;
#endif
					if (input_id<0)
					{
						if (scene == LOOP)
							input_id = (input_id + input_ysize * 20) % (input_ysize * 2);
						else if (scene == BOUNDARY)
							continue;
					}
					else if (input_id >= input_ysize * 2)
					{
						if (scene == LOOP)
							input_id %= input_ysize * 2;
						else if (scene == BOUNDARY)
							continue;
					}
					if (input_id % 2 == 0)
						continue;
					input_id /= 2;
					outtmp[i][j][k] += filter.filter[/*(filterSize-1)-*/l] * input[i][input_id][k];
				}
			}
		}
		output = outtmp;
	}
	else if (dir == 2)
	{
		double*** outtmp;// = new double*[input_ysize];
		new3D(outtmp, input_xsize, input_ysize, input_zsize*2);
		reset3D(outtmp, input_xsize, input_ysize, input_zsize*2);
		for (int i = 0; i<input_xsize; i++)
		{
			for (int j = 0; j<input_ysize; j++)
			{
				for (int k = 0; k < input_zsize*2; k++)
				for (int l = 0; l<filter.filterLen; l++)
				{
#ifdef DUAL_CONVOLUTION
					int input_id = k -/*(filterSize-1)+*/l - filter.startid;
#else
					int input_id = k +/*(filterSize-1)+*/l + filter.startid;
#endif
					if (input_id<0)
					{
						if (scene == LOOP)
							input_id = (input_id + input_zsize * 20) % (input_zsize * 2);
						else if (scene == BOUNDARY)
							continue;
					}
					else if (input_id >= input_zsize * 2)
					{
						if (scene == LOOP)
							input_id %= input_zsize * 2;
						else if (scene == BOUNDARY)
							continue;
					}
					if (input_id % 2 == 0)
						continue;
					input_id /= 2;
					outtmp[i][j][k] += filter.filter[/*(filterSize-1)-*/l] * input[i][j][input_id];
				}
			}
		}
		output = outtmp;

	}
}


void DFdwt3D::release()
{
	for (int i = 0;i<waveletCoeDF1HHH.size();i++)
	{
		release3D(waveletCoeDF1HHH[i],waveletCoeSize[i],waveletCoeSize[i]);
		release3D(waveletCoeDF1HHL[i],waveletCoeSize[i],waveletCoeSize[i]);
		release3D(waveletCoeDF1HLH[i],waveletCoeSize[i],waveletCoeSize[i]);
		release3D(waveletCoeDF1HLL[i],waveletCoeSize[i],waveletCoeSize[i]);
		release3D(waveletCoeDF1LHH[i],waveletCoeSize[i],waveletCoeSize[i]);
		release3D(waveletCoeDF1LHL[i],waveletCoeSize[i],waveletCoeSize[i]);
		release3D(waveletCoeDF1LLH[i],waveletCoeSize[i],waveletCoeSize[i]);
		release3D(waveletCoeDF2HHH[i],waveletCoeSize[i],waveletCoeSize[i]);
		release3D(waveletCoeDF2HHL[i],waveletCoeSize[i],waveletCoeSize[i]);
		release3D(waveletCoeDF2HLH[i],waveletCoeSize[i],waveletCoeSize[i]);
		release3D(waveletCoeDF2HLL[i],waveletCoeSize[i],waveletCoeSize[i]);
		release3D(waveletCoeDF2LHH[i],waveletCoeSize[i],waveletCoeSize[i]);
		release3D(waveletCoeDF2LHL[i],waveletCoeSize[i],waveletCoeSize[i]);
		release3D(waveletCoeDF2LLH[i],waveletCoeSize[i],waveletCoeSize[i]);
		release3D(waveletCoeNHHH[i],waveletCoeSize[i],waveletCoeSize[i]);
		release3D(waveletCoeNHHL[i],waveletCoeSize[i],waveletCoeSize[i]);
		release3D(waveletCoeNHLH[i],waveletCoeSize[i],waveletCoeSize[i]);
		release3D(waveletCoeNHLL[i],waveletCoeSize[i],waveletCoeSize[i]);
		release3D(waveletCoeNLHH[i],waveletCoeSize[i],waveletCoeSize[i]);
		release3D(waveletCoeNLHL[i],waveletCoeSize[i],waveletCoeSize[i]);
		release3D(waveletCoeNLLH[i],waveletCoeSize[i],waveletCoeSize[i]);
	}
	for (int i = 0;i<waveletCoeULLL.size();i++)
	{
		release3D(waveletCoeULLL[i],waveletCoeSize[i],waveletCoeSize[i]);
		release3D(waveletCoeVLLL[i],waveletCoeSize[i],waveletCoeSize[i]);
		release3D(waveletCoeWLLL[i],waveletCoeSize[i],waveletCoeSize[i]);

	}
	for (int i = 0;i<recWaveletCoeULLL.size();i++)
	{
		release3D(recWaveletCoeULLL[i],recWaveletCoeSize[i],recWaveletCoeSize[i]);
		release3D(recWaveletCoeVLLL[i],recWaveletCoeSize[i],recWaveletCoeSize[i]);
		release3D(recWaveletCoeWLLL[i],recWaveletCoeSize[i],recWaveletCoeSize[i]);
	}
	release3D(velocityField.u,DIM,DIM);
	release3D(velocityField.v,DIM,DIM);
	release3D(velocityField.w,DIM,DIM);
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
	recWaveletCoeULLL.clear();
	recWaveletCoeVLLL.clear();
	recWaveletCoeWLLL.clear();
}